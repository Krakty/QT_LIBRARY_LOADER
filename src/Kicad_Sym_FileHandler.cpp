// Kicad_Sym_FileHandler.cpp
#include "Kicad_Sym_FileHandler.h"
#include "s_expr_parser_symbol.h"
#include <fstream>
#include <stdexcept>

// Load an S-expression tree from a file
std::shared_ptr<SeExprNodeSymbol> KicadSymFileHandler::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file '" + filePath + "'");
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return SeExprParserSymbol::parse(content);
}

// Save an S-expression tree to a file
void KicadSymFileHandler::saveToFile(const std::shared_ptr<SeExprNodeSymbol>& root, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for writing '" + filePath + "'");
    }

    file << root->toString();
    file.close();
}

// Update a property in a KiCad symbol file
void KicadSymFileHandler::updateProperty(std::shared_ptr<SeExprNodeSymbol>& root, const std::string& key, const std::string& newValue) {
    auto targetNode = root->findKey(key);
    if (targetNode && targetNode->type == SeExprNodeSymbol::NodeType::Atom) {
        if (auto ptr = targetNode->shared_from_this(); ptr) {
            ptr->value = newValue;
        }
    } else {
        throw std::runtime_error("Error: Key not found or invalid key type - '" + key + "'");
    }
}
