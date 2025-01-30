#include "recompress_library.h"
#include "config.h"            // Includes globalConfig.tmpFolderRoot and ModifiedLibrary
#include <minizip/zip.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

void Recompress_Library(const std::string& baseName) {
    std::string zipPath = globalConfig.tmpFolderRoot + "/" + baseName + ".zip";
    std::string sourceDir = ModifiedLibrary;

    zipFile zf = zipOpen(zipPath.c_str(), APPEND_STATUS_CREATE);
    if (!zf) {
        std::cerr << "Error: Could not create ZIP file: " << zipPath << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string relativePath = std::filesystem::relative(entry.path(), sourceDir).string();

            zip_fileinfo zi = {};
            if (zipOpenNewFileInZip(zf, relativePath.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK) {
                std::cerr << "Error: Could not add file to ZIP: " << filePath << std::endl;
                continue;
            }

            std::ifstream file(filePath, std::ios::binary);
            if (file) {
                std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
                zipWriteInFileInZip(zf, buffer.data(), buffer.size());
                zipCloseFileInZip(zf);
            }
        }
    }

    zipClose(zf, NULL);
}
