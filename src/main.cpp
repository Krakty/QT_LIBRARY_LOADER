#include <iostream>
#include <string>
#include <vector>
#include "json_loader.h"
#include "unzipper.h"
#include "file_manager.h"
#include "kicad_parser.h"
#include "config.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <zip_file_path>" << std::endl;
        return 1;
    }

    std::string zipFilePath = argv[1];

    try {
        // Load the configuration
        JSONLoader loader("QT_LIBRARY_LOADER.json");
        loader.loadConfig();

        // Extract the basename of the zip file
        std::string baseName = fs::path(zipFilePath).stem().string();

        // Generate random string
        std::string randomString = FileManager::generateRandomString(24);

        // Create temporary directories using TMP_FOLDER_ROOT from config
        ExtractedLibrary = FileManager::createTempDir(globalConfig.tmpFolderRoot, baseName, randomString, "kicad_orig");
        ModifiedLibrary = FileManager::createTempDir(globalConfig.tmpFolderRoot, baseName, randomString, "kicad_working");

        std::cout << "Original directory: " << ExtractedLibrary << std::endl;
        std::cout << "Working directory: " << ModifiedLibrary << std::endl;

        // Extract the contents of the zip file to the original directory
        Unzipper::extractToDir(zipFilePath, ExtractedLibrary);

        // Search for specified file types in the ExtractedLibrary directory
        std::vector<std::string> foundFiles;
        FileManager::searchFiles(ExtractedLibrary, FileExtensions, foundFiles);

        // Copy found files to the ModifiedLibrary location
        FileManager::copyFiles(foundFiles, ModifiedLibrary);

        // Rename files in the ModifiedLibrary location
        FileManager::renameFiles(ModifiedLibrary, baseName);

        // Print found files
        std::cout << "Found files:" << std::endl;
        for (const auto &file : foundFiles) {
            std::cout << file << std::endl;
        }

        std::cout << "Files extracted, searched, copied, and renamed successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
