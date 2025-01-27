#include "json_loader.h"
#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>

Config globalConfig;

using json = nlohmann::json;

JSONLoader::JSONLoader(const std::string &filePath) : filePath(filePath) {}

void JSONLoader::loadConfig() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open JSON file: " + filePath);
    }

    json j;
    file >> j;

    globalConfig.libraryLocation = j.at("Library_Location").get<std::string>();
    globalConfig.footprintLocation = j.at("Footprint_Location").get<std::string>();
    globalConfig.footprint = j.at("Footprint").get<std::string>();
    globalConfig.model3D = j.at("3D_Model").get<std::string>();
    globalConfig.symbolLibraryName = j.at("Symbol_Library_Name").get<std::string>();
    globalConfig.tmpFolderRoot = j.at("TMP_FOLDER_ROOT").get<std::string>();
}
