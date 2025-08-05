#include "file_manager.h"
#include "config.h"
#include "display_info.h"
#include "file_utils.h"
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

void FileManager::searchFiles(const std::string &directory,
                              const std::vector<std::string> &extensions,
                              std::vector<std::string> &foundFiles) {
    try {
        for (const auto &entry : fs::recursive_directory_iterator(directory)) {
            if (fs::is_regular_file(entry.path())) {
                std::string extension = entry.path().extension().string();
                if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
                    foundFiles.push_back(entry.path().string());
                    std::ostringstream oss;
                    oss  << BRIGHT_CYAN << "    Found file: " << entry.path() << " (extension: " << extension << ")\n"<< RESET;
                    std::string message = oss.str();
                    DisplayMessage(message);
                    oss.str(""); // Clear the stringstream
                    oss.clear(); // Reset the flags
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error searching files in directory: " << directory
                  << ". Reason: " << e.what() << std::endl;
        throw;
    }
}


void FileManager::copyFiles(const std::vector<std::string> &files, const std::string &destination) {
    for (const auto &file : files) {
        fs::path sourcePath(file);
        fs::path destinationPath = fs::path(destination) / sourcePath.filename();
        try {
            fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
            std::ostringstream oss;
            oss  << BRIGHT_CYAN << "    Copied: " << sourcePath << " -> " << destinationPath << std::endl << RESET;
            std::string message = oss.str();
            DisplayMessage(message);
            oss.str(""); // Clear the stringstream
            oss.clear(); // Reset the flags
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error copying file: " << sourcePath
                      << ". Reason: " << e.what() << std::endl;
        }
    }
}

void FileManager::renameFiles(const std::string &directory, const std::string &newBaseName) {
    // Collect all entries first
    std::vector<fs::directory_entry> entries;
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path())) {
            entries.push_back(entry);
        }
    }

    // Pass 1: rename .kicad_mod files with special logic
    int modFileCount = 0;
    for (auto &entry : entries) {
        fs::path oldPath = entry.path();
        if (oldPath.extension() == ".kicad_mod") {
            fs::path newPath;

            if (modFileCount == 0) {
                // first -> basename.kicad_mod
                newPath = fs::path(directory) / (newBaseName + ".kicad_mod");
            } else {
                // subsequent -> basename_A.kicad_mod, etc.
                // note: only handles up to 26 files (A-Z). For more, expand logic.
                char suffix = static_cast<char>('A' + (modFileCount - 1));
                newPath = fs::path(directory) / (newBaseName + std::string("_") + suffix + ".kicad_mod");
            }

            try {
                fs::rename(oldPath, newPath);
                std::ostringstream oss;
                oss << BRIGHT_CYAN << "    Renamed: " << oldPath << " -> " << newPath << std::endl;
                std::string message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
                modFileCount++;
            } catch (const fs::filesystem_error &e) {
                std::cerr << "Error renaming file: " << oldPath
                          << ". Reason: " << e.what() << std::endl;
            }
        }
    }

    // Pass 2: rename all other files as before
    for (auto &entry : entries) {
        fs::path oldPath = entry.path();
        if (oldPath.extension() != ".kicad_mod") {
            fs::path newPath = fs::path(directory) / (newBaseName + oldPath.extension().string());
            try {
                fs::rename(oldPath, newPath);
                std::ostringstream oss; //Used to pass information to DisplayMessage functions
                oss << "    Renamed: " << oldPath << " -> " << newPath << std::endl;
                std::string message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
            } catch (const fs::filesystem_error &e) {
                std::cerr << "Error renaming file: " << oldPath
                          << ". Reason: " << e.what() << std::endl;
            }
        }
    }
}

std::string FileManager::createTempDir(const std::string& root,
                                       const std::string& baseName,
                                       const std::string& suffix) {
    std::string fullPath = root + "/" + baseName + "_" + suffix;
    try {
        fs::create_directories(fullPath);
        if (!fs::exists(fullPath)) {
            throw std::runtime_error("Failed to create directory: " + fullPath);
        }

        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << BOLD_CYAN << "Successfully created directory: " << fullPath << RESET << std::endl;
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << fullPath
                  << ". Reason: " << e.what() << std::endl;
        throw;
    }
    return fullPath;
}

void FileManager::clearTempDir(const std::string& root) {
    try {
        if (fs::exists(root)) {
            fs::remove_all(root);
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error clearing temporary directory: " << root
                  << ". Reason: " << e.what() << std::endl;
        throw;
    }
}

void FileManager::copyKicadModFiles() {
    std::string destination = globalConfig.libraryLocation + globalConfig.footprintLocation;
    if (!fs::exists(destination)) {
        fs::create_directories(destination);
    }

    for (const auto& entry : fs::directory_iterator(ModifiedLibrary)) {
        if (entry.path().extension() == ".kicad_mod") {
            fs::path destPath = fs::path(destination) / entry.path().filename();
            // Use NFS-safe copy for .kicad_mod files
            if (nfsSafeCopyFile(entry.path().string(), destPath.string())) {
                std::ostringstream oss; //Used to pass information to DisplayMessage functions
                oss << CYAN << "Copy .kicad_mod files:\n" << RESET;
                std::string message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
                oss << RED << "  SOURCE:      " << YELLOW << entry.path() << RESET << "\n";
                message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
                oss << RED << "  DESTINATION: " << YELLOW << destPath << RESET << "\n\n";
                message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
            } else {
                std::cerr << "Error copying .kicad_mod file using NFS-safe copy method" << std::endl;
            }
        }
    }
}

void FileManager::copy3DModelFiles() {
    std::string destination = globalConfig.libraryLocation + globalConfig.model3DLocation;
    if (!fs::exists(destination)) {
        fs::create_directories(destination);
    }

    for (const auto& ext : FileTypes3D) {
        for (const auto& entry : fs::directory_iterator(ModifiedLibrary)) {
            if (entry.path().extension() == ext) {
                fs::path destPath = fs::path(destination) / entry.path().filename();
                // Use NFS-safe copy for 3D model files
                if (nfsSafeCopyFile(entry.path().string(), destPath.string())) {
                    std::ostringstream oss; //Used to pass information to DisplayMessage functions
                    oss << CYAN << "Copy 3D files:\n" << RESET;
                    std::string message = oss.str();
                    DisplayMessage(message);
                    oss.str(""); // Clear the stringstream
                    oss.clear(); // Reset the flags
                    oss << RED << "  SOURCE:      " << YELLOW << entry.path() << RESET << "\n";
                    message = oss.str();
                    DisplayMessage(message);
                    oss.str(""); // Clear the stringstream
                    oss.clear(); // Reset the flags
                    oss << RED << "  DESTINATION: " << YELLOW << destPath << RESET << "\n\n";
                    message = oss.str();
                    DisplayMessage(message);
                    oss.str(""); // Clear the stringstream
                    oss.clear(); // Reset the flags
                    return; // Only copy the first valid 3D model found
                } else {
                    std::cerr << "Error copying 3D model file using NFS-safe copy method" << std::endl;
                }
            }
        }
    }

}

void FileManager::copyMergedSymbolFile() {
    fs::path sourcePath = fs::path(ModifiedLibrary) / globalConfig.symbolLibraryName;
    fs::path destination = fs::path(globalConfig.libraryLocation) / globalConfig.symbolLibraryName;

    if (fs::exists(sourcePath)) {
        // Use NFS-safe copy function instead of fs::copy_file
        if (nfsSafeCopyFile(sourcePath.string(), destination.string())) {
            std::ostringstream oss; //Used to pass information to DisplayMessage functions
            oss << CYAN << "Copy merged .kicad_sym files:\n" << RESET;
            std::string message = oss.str();
            DisplayMessage(message);

            oss.str(""); // Clear the stringstream
            oss.clear(); // Reset the flags
            oss << RED << "  SOURCE:      " << YELLOW << sourcePath << RESET << "\n";
            message = oss.str();
            DisplayMessage(message);

            oss.str(""); // Clear the stringstream
            oss.clear(); // Reset the flags
            oss << RED << "  DESTINATION: " << YELLOW << destination << RESET << "\n\n";
            message = oss.str();
            DisplayMessage(message);
            oss.str(""); // Clear the stringstream
            oss.clear(); // Reset the flags;
        } else {
            std::cerr << "Error copying .kicad_sym file using NFS-safe copy method" << std::endl;
        }
    } else {
        std::cerr << "Error: Merged .kicad_sym file not found in " << ModifiedLibrary << std::endl;
    }
}

void FileManager::copyMasterSymbolFile() {
    std::string source = globalConfig.libraryLocation + globalConfig.symbolLibraryName;
    std::string destination = ModifiedLibrary + "/" + globalConfig.symbolLibraryName;
    std::ostringstream oss; //Used to pass information to DisplayMessage functions
    oss << CYAN << "[INFO] Copying master .kicad_sym:\n"
        << BOLD_BRIGHT_GREEN << "SOURCE:      " << BOLD_CYAN << source << "\n"
        << BOLD_BRIGHT_GREEN << "DESTINATION: " << BOLD_CYAN << destination
        << RESET << std::endl;
    std::string message = oss.str();
    DisplayMessage(message);
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags
    
    // Use regular copy here since source is NFS and destination is local temp
    fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
}


void FileManager::BackupMasterKicadSym() {
    fs::path masterPath = fs::path(globalConfig.libraryLocation) / globalConfig.symbolLibraryName;
    fs::path backupPath = masterPath;
    backupPath.replace_extension(".bak");

    // Use NFS-safe copy for backup since both source and destination are on NFS
    if (nfsSafeCopyFile(masterPath.string(), backupPath.string())) {
        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << CYAN << "[INFO] Backup created at: " << BRIGHT_CYAN << backupPath << RESET << "\n";
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags
    } else {
        std::cerr << "[ERROR] Error creating backup using NFS-safe copy method\n";
    }
}

// namespace FileManager
