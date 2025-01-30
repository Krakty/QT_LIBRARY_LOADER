// Kicad_Mod_FileHandler.cpp
#include "Kicad_Mod_FileHandler.h"
#include "s_expr_parser_mod.h"
#include <fstream>
#include <stdexcept>

// Load an S-expression tree from a file
std::shared_ptr<SExprNodeMod> KicadModFileHandler::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file '" + filePath + "'");
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return SExprParserMod::parse(content);
}

// Save an S-expression tree to a file
void KicadModFileHandler::saveToFile(const std::shared_ptr<SExprNodeMod>& root, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for writing '" + filePath + "'");
    }

    file << root->toString();
    file.close();
}

// Update a property in a KiCad mod file
void KicadModFileHandler::updateProperty(std::shared_ptr<SExprNodeMod>& root, const std::string& key, const std::string& newValue) {
    auto targetNode = root->findKey(key);
    if (targetNode && targetNode->type == SExprNodeMod::NodeType::Atom) {
        if (auto ptr = targetNode->shared_from_this(); ptr) {
            ptr->value = newValue;
        }
    } else {
        throw std::runtime_error("Error: Key not found or invalid key type - '" + key + "'");
    }
}
