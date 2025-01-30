#include "kicad_sym_processor.h"
#include "kicad_sym_parser.h"
#include "s_expr_node_symbol.h"
#include "config.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <fstream>

namespace fs = std::filesystem;

void updateFootprintProperty(std::shared_ptr<SeExprNodeSymbol> root, const Config& config) {
    // Search for the .kicad_sym file in the ModifiedLibrary directory
    std::string filename;
    for (const auto& entry : fs::directory_iterator(ModifiedLibrary)) {
        if (entry.path().extension() == ".kicad_mod") {
            filename = entry.path().filename().string();
            break; // We only need one match
        }
    }

    if (filename.empty()) {
        throw std::runtime_error("No .kicad_sym file found in the specified directory: " + ModifiedLibrary);
    }

    // Construct the new Footprint value using the passed `config`
    std::string newFootprintValue = config.footprint + "/" + filename;

    // Find the "Footprint" property node
    auto footprintProperty = root->findPropertyByName("Footprint");
    if (footprintProperty) {
        if (footprintProperty->children.size() > 2) {
            // Store the original value before updating
            std::string originalValue = footprintProperty->children[2]->value;

            // Log the current value and the new value
            std::cout << GREEN << "Found Footprint property node: " << BOLD_PURPLE << footprintProperty->toString() << RESET << std::endl;
            std::cout << GREEN << "Updating Footprint value from: " << BOLD_PURPLE << originalValue << GREEN
                      << " to: " << BOLD_PURPLE << "\"" + newFootprintValue + "\"" << RESET << std::endl;

            // Update the Footprint property with the constructed value
            footprintProperty->children[2]->value = "\"" + newFootprintValue + "\"";
        } else {
            std::cerr << RED << "Error: Footprint property node does not have enough children to update." << RESET << std::endl;
        }
    } else {
        std::cerr << RED << "Error: No Footprint property node found." << RESET << std::endl;
    }
}


void processKicadSymFiles(const std::string& directory, const Config& config) {
    if (!fs::exists(directory)) {
        throw std::runtime_error("Error: Directory does not exist: " + directory);
    }

    auto parsedFiles = parseKicadSymFiles(directory);

    for (const auto& root : parsedFiles) {
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.path().extension() == ".kicad_sym") {
                    fs::path originalFile = entry.path();

                    // Update the "Footprint" property
                    updateFootprintProperty(root, config);

                    // Serialize the updated tree
                    std::string updatedContent = root->toString();

                    // Write the updated content back to the original file
                    try {
                        std::ofstream outFile(originalFile);
                        if (!outFile) {
                            throw std::runtime_error("Failed to open file for writing: " + originalFile.string());
                        }

                        outFile << updatedContent;
                        outFile.close();
                        //std::cout << GREEN << "Renamed original file to backup: " << BOLD_PURPLE << backupFile << RESET << std::endl;
                        std::cout << GREEN << "Wrote updated file: " << BOLD_PURPLE << originalFile << RESET << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << RED << "Error writing updated file: " << e.what() << RESET << std::endl;
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << RED << "Error processing .kicad_sym file: " << e.what() << RESET << std::endl;
        }
    }
}
