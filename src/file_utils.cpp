#include "file_utils.h"
#include <filesystem>


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
