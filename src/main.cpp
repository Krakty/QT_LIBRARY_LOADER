#include <iostream>
#include <string>
#include <vector>
#include "file_manager.h"
#include "json_loader.h"
#include "unzipper.h"
#include "config.h"
#include <filesystem>
#include "kicad_sym_processor.h"
#include "kicad_mod_processor.h"
#include "recompress_library.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <zip_file_path>" << std::endl;
        return 1;
    }

    std::string zipFilePath = argv[1];

    try {
        // Clear /tmp directory
        FileManager::clearTempDir("./tmp");

        // Load the configuration
        JSONLoader loader("QT_LIBRARY_LOADER.json");
        loader.loadConfig();

        // Extract the basename of the zip file
        std::string baseName = fs::path(zipFilePath).stem().string();

        // Create temporary directories
        ExtractedLibrary = FileManager::createTempDir("./tmp", baseName, "kicad_orig");
        ModifiedLibrary = FileManager::createTempDir("./tmp", baseName, "kicad_working");

        std::cout << "Original directory: " << ExtractedLibrary << std::endl;
        std::cout << "Working directory: " << ModifiedLibrary << std::endl;

        // Extract the contents of the zip file to the original directory
        Unzipper::extractToDir(zipFilePath, ExtractedLibrary);

        // Search for specified file types in the extracted library
        std::vector<std::string> foundFiles;
        FileManager::searchFiles(ExtractedLibrary, FileExtensions, foundFiles);

        // Copy found files to the modified library location
        FileManager::copyFiles(foundFiles, ModifiedLibrary);

        // Rename files in the modified library location
        FileManager::renameFiles(ModifiedLibrary, baseName);

        std::cout << "Files extracted, searched, copied, and renamed successfully." << std::endl;
        std::cout << std::endl << std::endl;
        std::cout << "************************************************************" << std::endl;

        std::cout << GREEN << "=-=-=-=-=-=- UPDATING .kicad_mod =-=-=-=-=-=-\n" << RESET;
        std::cout << BOLD_PURPLE << "modifiedLibrary : " << GREEN << ModifiedLibrary << RESET << std::endl;
        // Process the KiCad .mod files in ModifiedLibrary
        try {
            // Pass the dynamic values into processKicadModFiles
            processKicadModFiles(ModifiedLibrary, globalConfig, baseName);
        } catch (const std::exception& e) {

            std::cerr << RED << "Error during processing: " << e.what() << RESET << std::endl;
            return 1;
        }
        std::cout << "\n\n";
        std::cout << GREEN << "=-=-=-=-=-=- UPDATING .kicad_sym =-=-=-=-=-=-\n" << RESET;
        // Process the KiCad .kicad_sym files in ModifiedLibrary
        try {
            processKicadSymFiles(ModifiedLibrary,globalConfig);
        } catch (const std::exception& e) {
            std::cerr << "Error during processing: " << e.what() << std::endl;
            return 1;
        }
        // Process the files in  ModifiedLibrary recompress them into a zip file in ./tmp
        try {
            Recompress_Library(baseName);
        } catch (const std::exception& e) {
            std::cerr << "Error during processing: " << e.what() << std::endl;
            return 1;
        }



    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
