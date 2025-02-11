
#include <iostream>
#include <filesystem>
#include "argument_handler.h"
#include "json_loader.h"
#include "config.h"
#include "display_info.h"
namespace fs = std::filesystem;

ProgramExecutionState handleCommandLineArguments(int argc, char* argv[], std::string& zipFilePath) {
    const std::string configFile = "QT_LIBRARY_LOADER.json";

    // Ensure at least one argument (the executable name) is passed
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " <zip_file_path>\n"
                  << "  " << argv[0] << " -Defaults (to create default config)\n"
                  << "  " << argv[0] << " -SupressOutput <zip_file_path>\n";
        return ProgramExecutionState::EXIT_FAILURE_STATE;
    }

    // Handle -Defaults flag (only as the first argument)
    if (std::string(argv[1]) == "-Defaults") {
        if (JSONLoader::createDefaultConfig(configFile)) {
            DisplayMessage("Default configuration file created sucessfully.\n");
            return ProgramExecutionState::EXIT_SUCCESS_STATE;
        } else {
            std::cerr << "Failed to create default configuration file.\n";
            return ProgramExecutionState::EXIT_FAILURE_STATE;
        }
    }

    // Handle -SupressOutput flag (only as the first argument)
    if (std::string(argv[1]) == "-SupressOutput") {
        suppress_output = true;  // Set global flag to suppress output
        if (argc < 3) {
            std::cerr << "Error: No zip file provided.\n";
            return ProgramExecutionState::EXIT_FAILURE_STATE;
        }
        zipFilePath = argv[2];  // The second argument is the zip file path
    } else {
        zipFilePath = argv[1];  // First argument is the zip file path
    }

    // Validate zip file exists
    if (!fs::exists(zipFilePath)) {
        std::cerr << "Error: The specified zip file does not exist: " << zipFilePath << "\n";
        return ProgramExecutionState::EXIT_FAILURE_STATE;
    }

    // Check if JSON config exists
    if (!fs::exists(configFile)) {
        std::cerr << "Error: " << configFile << " not found.\n";
        std::cerr << "Run the program with '-Defaults' to create a basic configuration file.\n";
        return ProgramExecutionState::EXIT_FAILURE_STATE;
    }

    return ProgramExecutionState::PROCESS_ZIP_STATE;  // Proceed to process the ZIP file
}
