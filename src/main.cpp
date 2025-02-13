#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include "file_manager.h"
#include "json_loader.h"
#include "unzipper.h"
#include "config.h"
#include "kicad_sym_processor.h"
#include "kicad_mod_processor.h"
#include "recompress_library.h"
#include "Kicad_Sym_FileHandler.h"
#include "s_expr_merge_symbol.h"
#include "display_info.h"
#include "argument_handler.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string header; //Used to display simple headers prior to actions
    std::ostringstream oss; //Used to pass information to DisplayMessage functions

    std::string zipFilePath;
    ProgramExecutionState state = handleCommandLineArguments(argc, argv, zipFilePath);

    // If -Defaults was used, handle it and exit early
    if (state == ProgramExecutionState::EXIT_SUCCESS_STATE) {
        return 0; // Exit after handling -Defaults
    } else if (state == ProgramExecutionState::EXIT_FAILURE_STATE) {
        return 1; // Exit on failure
    }

    // Check that we have a valid zip file path after command-line parsing
    if (zipFilePath.empty()) {
        std::cerr << "Error: No zip file path provided.\n";
        return 1;
    }

    try {
        // Extract baseName from the zip file name
        std::string baseName = fs::path(zipFilePath).stem().string();
        // Initialize the vector of found files
        std::vector<std::string> foundFiles;

        DisplayInfo("Starting KiCad Library Processor");
        // Clear the ./tmp directory
        FileManager::clearTempDir("./tmp");
        DisplayInfo("Temporary directory cleared");

        DisplayInfo("Loading configuration...");
        JSONLoader loader("QT_LIBRARY_LOADER.json");
        loader.loadConfig(); // Load the configuration file
        DisplayInfo("Configuration loaded successfully");

        DisplayInfo("Creating temporary directories");
        ExtractedLibrary = FileManager::createTempDir("./tmp", baseName, "kicad_orig");
        ModifiedLibrary = FileManager::createTempDir("./tmp", baseName, "kicad_working");

        DisplayInfo("Extracting files from zip archive...");
        Unzipper::extractToDir(zipFilePath, ExtractedLibrary);
        DisplayInfo("Files extracted successfully");

        header = "Searching for KiCad files in " + ExtractedLibrary;
        DisplayInfo_hdr(header);
        FileManager::searchFiles(ExtractedLibrary, FileExtensions, foundFiles);

        header = "Copying KiCad Files in " + ExtractedLibrary + " TO >> " + ModifiedLibrary;
        DisplayInfo_hdr(header);
        FileManager::copyFiles(foundFiles, ModifiedLibrary);

        header = "Renaming KiCad Files in " + ExtractedLibrary;
        DisplayInfo_hdr(header);
        FileManager::renameFiles(ModifiedLibrary, baseName);
        DisplayInfo("Files extracted, searched, copied, and renamed successfully.");

        DisplayInfo("Backing up master .kicad_sym file");
        FileManager::BackupMasterKicadSym();

        DisplayInfo("Copying master .kicad_sym to ModifiedLibrary...");
        FileManager::copyMasterSymbolFile();

        DisplayInfo("Environment setup complete.");
        //pauseExecution("Press Enter to continue...");

        // Begin .kicad_mod file modifications
        DisplayInfo("Updating .kicad_mod files");
        header = "ModifiedLibrary : " + ModifiedLibrary;
        DisplayInfo_hdr(header);

        try {
            KicadModProcessor modProcessor;
            modProcessor.processKicadModFiles(ModifiedLibrary, globalConfig, baseName);
        } catch (const std::exception &e) {
            std::cerr << "Error during processing: " << e.what() << "\n";
            return 1;
        }
        // End .kicad_mod file modifications

        // Begin .kicad_sym file modifications
        DisplayInfo("Updating .kicad_sym files");
        try {
            // Step 1: Load the .kicad_sym file before modification
            fs::path symFilePath = fs::path(ModifiedLibrary) / (baseName + ".kicad_sym");

            auto root = KicadSymFileHandler::loadFromFile(symFilePath.string());

            if (suppress_output) {
                std::cout << "Output suppressed.\n";
            } else {
                DisplayInfo_hdr("Listing properties before modification:\n");
                root->printAllProperties();
            }


            // Step 2: Modify the .kicad_sym file
            DisplayInfo_hdr("Modifying .kicad_sym file...");

            // The Following function modifies the .kicad_sym file and saves the changes to the same file
            processKicadSym(symFilePath.string(), globalConfig);
            DisplayInfo_hdr("\n");
            // Step 3: Re-read the file to confirm changes
            auto updatedRoot = KicadSymFileHandler::loadFromFile(symFilePath.string());
            if (suppress_output) {
                std::cout << "Output suppressed.\n";
            } else {
                DisplayInfo_hdr("Listing properties after modification:\n");
                updatedRoot->printAllProperties();
            }

            DisplayInfo_hdr("\n");

        } catch (const std::exception &e) {
            std::cerr << "Error during processing: " << e.what() << "\n";
            return 1;
        }
        // End .kicad_sym file modifications

        // Begin Merge Operation of Library into the defined .kicad_sym file
        DisplayInfo("Beginning merge operation...");
        std::string masterSymPath = ModifiedLibrary + "/" + globalConfig.symbolLibraryName;
        fs::path localSymPath = fs::path(ModifiedLibrary) / (baseName + ".kicad_sym");
        if (fs::exists(localSymPath)) {
            oss << CYAN << "\nMerging " << BOLD_BRIGHT_YELLOW << localSymPath.filename().string()
                << BOLD_BRIGHT_PURPLE << " into " << BOLD_BRIGHT_YELLOW
                << fs::path(masterSymPath).filename().string() << RESET << "...\n";

            std::string message = oss.str();
            DisplayMessage(message);

            bool success = SeExprMergeSymbol::mergeSymbolLibraryFiles(masterSymPath, localSymPath.string(), masterSymPath);
            oss.str(""); // Clear the stringstream
            oss.clear(); // Reset the flags
            oss << (success ? BOLD_BRIGHT_PURPLE "Symbol-level merging completed successfully.\n"
                            : BOLD_BRIGHT_RED "Symbol-level merge failed.\n")
                << RESET;
            message = oss.str();
            DisplayMessage(message);

        } else {
            std::cout << "No local " << baseName << ".kicad_sym found. Skipping merge...\n";
        }
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags
        oss << BOLD_BRIGHT_PURPLE << "\nCopying processed files to library locations...\n" << RESET;
        std::string message = oss.str();
        DisplayMessage(message);

        FileManager::copyKicadModFiles();
        FileManager::copy3DModelFiles();
        FileManager::copyMergedSymbolFile();

        // Recompress the library
        try {
            Recompress_Library(baseName);
        } catch (const std::exception& e) {
            std::cerr << "Error during recompressing the library: " << e.what() << "\n";
            return 1;
        }
        DisplayInfo("Library processing complete!");
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
