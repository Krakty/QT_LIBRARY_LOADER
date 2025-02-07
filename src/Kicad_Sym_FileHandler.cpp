#include "Kicad_Sym_FileHandler.h"
#include "s_expr_parser_symbol.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "config.h"
/**
 * Load an S-expression tree from a `.kicad_sym` file.
 */
std::shared_ptr<SeExprNodeSymbol> KicadSymFileHandler::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file '" + filePath + "'");
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::cout << CYAN << "[DEBUG] Loaded file: " << BOLD_BRIGHT_CYAN << filePath << RESET <<"\n";
    return SeExprParserSymbol::parse(content);
}

/**
 * Save an S-expression tree to a `.kicad_sym` file.
 */
void KicadSymFileHandler::saveToFile(const std::shared_ptr<SeExprNodeSymbol>& root, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for writing '" + filePath + "'");
    }

    file << root->toString();
    file.close();

    std::cout << YELLOW << "[DEBUG] Saved modified file: " << BOLD_BRIGHT_YELLOW <<  filePath << RESET << "\n";
}

/**
 * Update a property in a KiCad symbol file.
 *
 * This function updates the value of a key if found.
 */
void KicadSymFileHandler::updateProperty(std::shared_ptr<SeExprNodeSymbol>& root, const std::string& key, const std::string& newValue) {
    auto targetNode = root->findKey(key);
    if (targetNode && targetNode->type == SeExprNodeSymbol::NodeType::Atom) {
        if (auto ptr = targetNode->shared_from_this(); ptr) {
            std::cout << "[DEBUG] Updating property '" << key << "' from '" << ptr->value << "' to '" << newValue << "'\n";
            ptr->value = newValue;
        }
    } else {
        std::cerr << "[WARNING] Key not found or invalid key type - '" << key << "'\n";
    }
}
