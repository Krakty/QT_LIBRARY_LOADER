#include "file_manager.h"
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

void FileManager::searchFiles(const std::string &directory, const std::vector<std::string> &extensions, std::vector<std::string> &foundFiles) {
    for (const auto &entry : fs::recursive_directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path())) {
            std::string extension = entry.path().extension().string();
            if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
                foundFiles.push_back(entry.path().string());
            }
        }
    }
}

void FileManager::copyFiles(const std::vector<std::string> &files, const std::string &destination) {
    for (const auto &file : files) {
        fs::path sourcePath(file);
        fs::path destinationPath = fs::path(destination) / sourcePath.filename();
        fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
    }
}

void FileManager::renameFiles(const std::string &directory, const std::string &newBaseName) {
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path())) {
            std::string extension = entry.path().extension().string();
            fs::path newPath = fs::path(directory) / (newBaseName + extension);
            fs::rename(entry.path(), newPath);
        }
    }
}

std::string FileManager::generateRandomString(size_t length) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    const size_t maxIndex = sizeof(charset) - 1;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, maxIndex);

    std::stringstream ss;
    for (size_t i = 0; i < length; ++i) {
        ss << charset[distribution(generator)];
    }

    return ss.str();
}

std::string FileManager::createTempDir(const std::string& tmpFolderRoot, const std::string& baseName, const std::string& randomString, const std::string& suffix) {
    std::string dirName = tmpFolderRoot + "/" + randomString + "_" + baseName + "_" + suffix;

    fs::create_directories(dirName);
    return dirName;
}
