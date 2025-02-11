#include "kicad_sym_processor.h"
#include "Kicad_Sym_FileHandler.h"
#include "s_expr_node_symbol.h"
#include "config.h"
#include "display_info.h"
#include <sstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

/**
 * Updates the "Footprint" property for a given root symbol node.
 *
 * If the property doesn't exist, it's created. If it exists but
 * doesn't have enough child nodes, new ones are appended.
 */
void updateFootprintProperty(std::shared_ptr<SeExprNodeSymbol> root, const std::string& basename)
{
    std::ostringstream oss; //Used to pass information to DisplayMessage functions
    oss << CYAN << "[DEBUG] Attempting to update 'Footprint' property with basename: " << BOLD_BRIGHT_CYAN <<  basename << RESET <<"\n";
    std::string message = oss.str();
    DisplayMessage(message);
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags


    auto footprintProperty = root->findPropertyByName("Footprint");
    std::string newFootprint = "0_Custom-Library:" + basename;

    if (footprintProperty) {
        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << CYAN << "[DEBUG] Found 'Footprint' property" <<"\n";
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags

        oss << BOLD_BRIGHT_PURPLE << "    Current value = " << BOLD_BRIGHT_CYAN << (footprintProperty->children.size() > 2 ? footprintProperty->children[2]->value : "N/A")<< RESET << "\n";
        message =oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags

        oss << BOLD_BRIGHT_PURPLE << "    New value     = " << BOLD_BRIGHT_CYAN << newFootprint << RESET << "\n";
        message =oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags

        if (footprintProperty->children.size() > 2) {
            footprintProperty->children[2]->value = "\"" + newFootprint + "\"";
        } else {
            footprintProperty->addChild(std::make_shared<SeExprNodeSymbol>("\"" + newFootprint + "\""));
        }
    } else {
        DisplayMessage("[DEBUG] 'Footprint' property not found. Creating new property.\n");
        auto newProperty = std::make_shared<SeExprNodeSymbol>();
        newProperty->addChild(std::make_shared<SeExprNodeSymbol>("property"));
        newProperty->addChild(std::make_shared<SeExprNodeSymbol>("\"Footprint\""));
        newProperty->addChild(std::make_shared<SeExprNodeSymbol>("\"" + newFootprint + "\""));
        root->addChild(newProperty);
    }
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags
    oss << BOLD_BRIGHT_YELLOW << "[DEBUG] Update 'Footprint' property COMPLETE " << RESET << "\n";
    message = oss.str();
    DisplayMessage(message);
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags
}

/**
 * Updates the "ki_fp_filters" property for a given root symbol node,
 * combining multiple basenames into a single string separated by spaces.
 *
 * This is only done when there are multiple `.kicad_mod` files.
 */
void updateKiFpFiltersProperty(std::shared_ptr<SeExprNodeSymbol> root, const std::vector<std::string>& basenames)
{
    if (basenames.size() > 1) {
        DisplayMessage("[DEBUG] Updating 'ki_fp_filters' property with basenames:\n");
        for (const auto& name : basenames) {
            std::cout << "    - " << name << "\n";
        }

        root->clearPropertyValues("ki_fp_filters");

        std::string combinedFilters;
        for (size_t i = 0; i < basenames.size(); ++i) {
            combinedFilters += basenames[i];
            if (i != basenames.size() - 1) {
                combinedFilters += " ";
            }
        }

        root->addPropertyValues("ki_fp_filters", {combinedFilters});
        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << "[DEBUG] Combined 'ki_fp_filters' value: " << combinedFilters << "\n";
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags

    } else {
        DisplayMessage("[DEBUG] Only one .kicad_mod file found. Skipping 'ki_fp_filters' update.\n");
    }
}

/**
 * Main function for processing a .kicad_sym file (e.g., updating
 * the Footprint and ki_fp_filters properties).
*/

void processKicadSym(const std::string& symFilePath, const Config& config)
{
    std::ostringstream oss; //Used to pass information to DisplayMessage functions
    oss << CYAN << "[INFO] Processing file: " << BOLD_BRIGHT_CYAN << symFilePath << RESET << "\n";
    std::string message = oss.str();
    DisplayMessage(message);
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags

    if (!fs::exists(symFilePath)) {
        throw std::runtime_error("Specified .kicad_sym file does not exist: " + symFilePath);
    }

    auto root = KicadSymFileHandler::loadFromFile(symFilePath);

    // Look for `.kicad_mod` files in the same directory
    std::vector<std::string> modFiles;
    fs::path directory = fs::path(symFilePath).parent_path();

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".kicad_mod") {
            modFiles.push_back(entry.path().stem().string());
        }
    }

    if (!modFiles.empty()) {
        updateFootprintProperty(root, modFiles.front());

        if (modFiles.size() > 1) {
            updateKiFpFiltersProperty(root, modFiles);
        }
    }

    KicadSymFileHandler::saveToFile(root, symFilePath);
    oss << YELLOW << "[INFO] Finished processing " << BOLD_BRIGHT_YELLOW << fs::path(symFilePath).filename().string() << RESET <<"\n";
    message = oss.str();
    DisplayMessage(message);
    oss.str(""); // Clear the stringstream
    oss.clear(); // Reset the flags
}
