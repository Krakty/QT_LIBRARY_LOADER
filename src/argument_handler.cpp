#include "argument_handler.h"
#include "json_loader.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ProgramExecutionState handleCommandLineArguments(int argc, char* argv[], std::string& zipFilePath) {
    const std::string configFile = "QT_LIBRARY_LOADER.json";

    // 1. Handle -Defaults option
    if (argc == 2 && std::string(argv[1]) == "-Defaults") {
        if (JSONLoader::createDefaultConfig(configFile)) {
            std::cout << "Default configuration file created successfully.\n";
            return ProgramExecutionState::EXIT_SUCCESS_STATE;  // Fully qualified name
        } else {
            std::cerr << "Failed to create default configuration file.\n";
            return ProgramExecutionState::EXIT_FAILURE_STATE;  // Fully qualified name
        }
    }
    // 2. No arguments provided
    if (argc != 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " <zip_file_path>\n"
                  << "  " << argv[0] << " -Defaults (to create default config)\n";
        return ProgramExecutionState::EXIT_FAILURE_STATE;  // Fully qualified name
    }
    // 3. ZIP file provided as argument
    zipFilePath = argv[1];

    // 4. Check if JSON config exists
    if (!fs::exists(configFile)) {
        std::cerr << "Error: " << configFile << " not found.\n";
        std::cerr << "Run the program with '-Defaults' to create a basic configuration file.\n";
        return ProgramExecutionState::EXIT_FAILURE_STATE;  // Fully qualified name
    }
    return ProgramExecutionState::PROCESS_ZIP_STATE;  // Fully qualified name
}
