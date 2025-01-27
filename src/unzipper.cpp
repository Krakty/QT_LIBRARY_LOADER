#include "unzipper.h"
#include <filesystem>
#include <iostream>
#include <fstream> // Include this header for std::ofstream
#include <minizip/unzip.h>

namespace fs = std::filesystem;

Unzipper::Unzipper(const std::string &zipFilePath, const std::string &destinationFolder)
    : zipFilePath(zipFilePath), destinationFolder(destinationFolder) {}

void Unzipper::unzip() {
    // Ensure destinationFolder exists
    if (!fs::exists(destinationFolder)) {
        fs::create_directories(destinationFolder);
    }

    // Open the zip file
    unzFile zipFile = unzOpen(zipFilePath.c_str());
    if (!zipFile) {
        throw std::runtime_error("Unable to open zip file: " + zipFilePath);
    }

    // Extract each file in the zip
    unz_global_info globalInfo;
    if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
        unzClose(zipFile);
        throw std::runtime_error("Unable to get global info of zip file: " + zipFilePath);
    }

    char readBuffer[8192];

    for (uLong i = 0; i < globalInfo.number_entry; ++i) {
        unz_file_info fileInfo;
        char fileName[256];
        if (unzGetCurrentFileInfo(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) != UNZ_OK) {
            unzClose(zipFile);
            throw std::runtime_error("Unable to get current file info: " + zipFilePath);
        }

        // Create the full path for the extracted file
        std::string fullPath = destinationFolder + "/" + fileName;

        // Create directories if necessary
        std::string directoryPath = fullPath.substr(0, fullPath.find_last_of('/'));
        if (!fs::exists(directoryPath)) {
            fs::create_directories(directoryPath);
        }

        // Open the file for writing
        if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
            unzClose(zipFile);
            throw std::runtime_error("Unable to open file in zip: " + std::string(fileName));
        }

        std::ofstream outFile(fullPath, std::ios::binary);
        if (!outFile) {
            unzCloseCurrentFile(zipFile);
            unzClose(zipFile);
            throw std::runtime_error("Unable to open output file: " + fullPath);
        }

        // Read the file from the zip and write it to disk
        int bytesRead;
        while ((bytesRead = unzReadCurrentFile(zipFile, readBuffer, sizeof(readBuffer))) > 0) {
            outFile.write(readBuffer, bytesRead);
        }

        outFile.close();
        unzCloseCurrentFile(zipFile);

        if (bytesRead < 0) {
            unzClose(zipFile);
            throw std::runtime_error("Error reading file from zip: " + std::string(fileName));
        }

        // Go to the next file in the zip
        if (i + 1 < globalInfo.number_entry) {
            if (unzGoToNextFile(zipFile) != UNZ_OK) {
                unzClose(zipFile);
                throw std::runtime_error("Unable to go to next file in zip: " + zipFilePath);
            }
        }
    }

    unzClose(zipFile);
}

void Unzipper::extractToDir(const std::string &zipFilePath, const std::string &destinationFolder) {
    Unzipper unzipper(zipFilePath, destinationFolder);
    unzipper.unzip();
}
