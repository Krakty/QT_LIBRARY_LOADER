#include "kicad_sym_parser.h"
#include "s_expr_node_symbol.h"
#include "Kicad_Sym_FileHandler.h"
#include "display_info.h"
#include <sstream>
#include <filesystem>
#include <vector>
#include <memory>
#include <iostream>

namespace fs = std::filesystem;

/**
 * Parses all `.kicad_sym` files in the specified directory.
 * Returns a vector of parsed root nodes for further processing.
 */
std::vector<std::shared_ptr<SeExprNodeSymbol>> parseKicadSymFiles(const std::string& directory) {
    std::vector<std::shared_ptr<SeExprNodeSymbol>> parsedFiles;

    if (!fs::exists(directory)) {
        std::cerr << "[ERROR] Directory does not exist: " << directory << "\n";
        return parsedFiles;
    }

    std::cout << "[INFO] Parsing `.kicad_sym` files in directory: " << directory << "\n";

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".kicad_sym") {
            try {
                auto root = KicadSymFileHandler::loadFromFile(entry.path().string());
                parsedFiles.push_back(root);
                std::ostringstream oss; //Used to pass information to DisplayMessage functions
                oss << "[DEBUG] Successfully parsed: " << entry.path().filename() << "\n";
                std::string message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Failed to parse " << entry.path().filename()
                << ": " << e.what() << "\n";
            }
        }
    }

    if (parsedFiles.empty()) {
        std::cout << "[INFO] No `.kicad_sym` files found in directory: " << directory << "\n";
    }

    return parsedFiles;
}
