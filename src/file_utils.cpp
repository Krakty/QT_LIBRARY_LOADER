#include "file_utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::vector<std::string> getFilesWithExtension(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            files.push_back(entry.path().string());
        }
    }

    return files;
}

bool nfsSafeCopyFile(const std::string& source, const std::string& destination) {
    try {
        // First, try to remove the destination file if it exists
        // This helps with NFS permission issues
        if (fs::exists(destination)) {
            fs::remove(destination);
        }
        
        // Open source file for reading
        std::ifstream src(source, std::ios::binary);
        if (!src.is_open()) {
            std::cerr << "Error: Cannot open source file: " << source << std::endl;
            return false;
        }
        
        // Open destination file for writing (create new file with default permissions)
        std::ofstream dst(destination, std::ios::binary);
        if (!dst.is_open()) {
            std::cerr << "Error: Cannot create destination file: " << destination << std::endl;
            return false;
        }
        
        // Copy file contents
        dst << src.rdbuf();
        
        // Check for errors
        if (src.bad() || dst.bad()) {
            std::cerr << "Error: File copy operation failed" << std::endl;
            return false;
        }
        
        // Close files explicitly
        src.close();
        dst.close();
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in nfsSafeCopyFile: " << e.what() << std::endl;
        return false;
    }
}
