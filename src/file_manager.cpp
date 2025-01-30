#include "file_manager.h"
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

void FileManager::searchFiles(const std::string &directory, const std::vector<std::string> &extensions, std::vector<std::string> &foundFiles) {
    try {
        for (const auto &entry : fs::recursive_directory_iterator(directory)) {
            if (fs::is_regular_file(entry.path())) {
                std::string extension = entry.path().extension().string();
                if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
                    foundFiles.push_back(entry.path().string());
                    std::cout << "Found file: " << entry.path() << " (extension: " << extension << ")" << std::endl;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error searching files in directory: " << directory << ". Reason: " << e.what() << std::endl;
        throw;
    }
}


void FileManager::copyFiles(const std::vector<std::string> &files, const std::string &destination) {
    for (const auto &file : files) {
        fs::path sourcePath(file);
        fs::path destinationPath = fs::path(destination) / sourcePath.filename();
        try {
            fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
            std::cout << "Copied: " << sourcePath << " -> " << destinationPath << std::endl;
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error copying file: " << sourcePath << ". Reason: " << e.what() << std::endl;
        }
    }
}

void FileManager::renameFiles(const std::string &directory, const std::string &newBaseName) {
    size_t counter = 0;
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path())) {
            std::string extension = entry.path().extension().string();
            //fs::path newPath = fs::path(directory) / (newBaseName + "_" + std::to_string(counter++) + extension);
            fs::path newPath = fs::path(directory) / (newBaseName + extension);
            fs::rename(entry.path(), newPath);
            std::cout << "Renamed: " << entry.path() << " -> " << newPath << std::endl;
        }
    }
}

std::string FileManager::createTempDir(const std::string& root, const std::string& baseName, const std::string& suffix) {
    std::string fullPath = root + "/" + baseName + "_" + suffix;
    try {
        fs::create_directories(fullPath);
        if (!fs::exists(fullPath)) {
            throw std::runtime_error("Failed to create directory: " + fullPath);
        }
        std::cout << "Successfully created directory: " << fullPath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << fullPath << ". Reason: " << e.what() << std::endl;
        throw;
    }
    return fullPath;
}

void FileManager::clearTempDir(const std::string& root) {
    try {
        if (fs::exists(root)) {
            fs::remove_all(root);
            std::cout << "Temporary directory cleared: " << root << std::endl;
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error clearing temporary directory: " << root << ". Reason: " << e.what() << std::endl;
        throw;
    }
}
