#include "json_loader.h"
#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

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
    globalConfig.model3DLocation = j.at("3D_Model_Location").get<std::string>();
}

bool JSONLoader::createDefaultConfig(const std::string &filePath) {
    json defaultConfig = {
        {"Library_Location", "/home/tlindell/CVS/Kicad_Libraries/"},
        {"Footprint_Location", "SamacSys_Parts.pretty/"},
        {"Footprint", "${SAMACSYS_FP}"},
        {"3D_Model", "${SAMACSYS_3D_SHAPES}"},
        {"Symbol_Library_Name", "example.kicad_sym"},
        {"TMP_FOLDER_ROOT", "./tmp"},
        {"3D_Model_Location", "SamacSys_Parts.3dshapes/"}
    };

    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Error: Unable to create " << filePath << std::endl;
        return false;
    }

    file << defaultConfig.dump(4);  // Pretty print with 4-space indentation
    file.close();

    std::cout << "Default configuration created at " << filePath << std::endl;
    return true;
}
