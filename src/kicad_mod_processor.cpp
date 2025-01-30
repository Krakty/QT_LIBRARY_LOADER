#include "kicad_mod_processor.h"
#include "kicad_mod_parser.h"
#include "s_expr_node_mod.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include "config.h"
#include <fstream>


std::string find3DFile(const std::string& directory, const std::string& baseName) {
    std::cout << GREEN << "Value of directory variable: " << BOLD_PURPLE << directory << RESET << "\n";
    std::cout << GREEN << "Value of baseName  variable: " << BOLD_PURPLE << baseName << RESET << "\n";
    std::cout << GREEN << "Found 3D Files" << RESET << "\n";

    std::vector<std::string> prioritizedExtensions = {".stp", ".step"};
    std::vector<std::string> remainingExtensions;

    // Separate prioritized extensions from the rest
    for (const auto& ext : FileTypes3D) {
        if (ext != ".stp" && ext != ".step") {
            remainingExtensions.push_back(ext);
        }
    }

    // Check prioritized extensions first
    for (const auto& ext : prioritizedExtensions) {
        std::filesystem::path fullFilePath = std::filesystem::path(directory) / (baseName + ext);
        std::cout << "Checking file: " << fullFilePath.string() << std::endl;
        if (std::filesystem::exists(fullFilePath)) {
            std::cout << RED << "File found: " << fullFilePath.string() << RESET << "\n";
            return fullFilePath.filename().string(); // ✅ Return only the filename
        }
    }

    // If no prioritized extension is found, check remaining extensions
    for (const auto& ext : remainingExtensions) {
        std::filesystem::path fullFilePath = std::filesystem::path(directory) / (baseName + ext);
        std::cout << "Checking file: " << fullFilePath.string() << std::endl;
        if (std::filesystem::exists(fullFilePath)) {
            std::cout << RED << "File found: " << fullFilePath.string() << RESET << "\n";
            return fullFilePath.filename().string(); // ✅ Return only the filename
        }
    }

    throw std::runtime_error("\n3D file not found with any known extension for base name: " + baseName);
}

// Function to update the 3D model path in the 'model' node
void updateModelPath(std::shared_ptr<SExprNodeMod> modelNode, const Config& config, const std::string& baseName) {
    if (!modelNode || modelNode->type != SExprNodeMod::NodeType::List) {
        std::cerr << "Error: 'modelNode' is invalid or not a list!" << std::endl;
        return;
    }

    // Traverse the children of the model node
    for (auto& child : modelNode->children) {
        if (child->type == SExprNodeMod::NodeType::Atom && child->value != "model") {
            try {
                // Use the global ModifiedLibrary instead of config.ModifiedLibrary
                std::string fileName = find3DFile(ModifiedLibrary, baseName);

                // Construct the new path dynamically using `config`
                std::string newPath = config.model3D + "/" + std::filesystem::path(fileName).filename().string();
                std::cout << GREEN << "Updating model path from: " << BOLD_PURPLE << child->value << GREEN << " to: " << BOLD_PURPLE << newPath << RESET << std::endl;

                // Update the Atom value with the new path
                child->value = newPath;
                return; // Exit once updated
            } catch (const std::exception& e) {
                std::cerr << RED << "Error finding 3D file: " << e.what() << RESET << std::endl;
            }
        }
    }

    std::cerr << RED << "No valid model path found to update!" << RESET << std::endl;
}

void processKicadModFiles(const std::string& directory, const Config& config, const std::string& baseName) {
    if (!std::filesystem::exists(directory)) {
        std::cout << GREEN << "Directory does not exist. Creating: " << directory << RESET << std::endl;
        std::filesystem::create_directory(directory);
    }

    auto parsedFiles = parseKicadModFiles(directory);

    for (const auto& root : parsedFiles) {
        try {
            //Debugging
            //std::cout << GREEN << ".kcad_mod Parsed tree:\n" << BOLD_PURPLE << root->toString() << RESET << "\n";

            // Find the "model" node
            auto modelNode = root->findKey("model");
            if (modelNode) {
                std::cout << GREEN << "Found model node: " << BOLD_PURPLE << modelNode->toString() << RESET << std::endl;

                // Update the model path dynamically based on `config`
                updateModelPath(modelNode, config, baseName);

                // Serialize the updated tree
                std::string updatedContent = root->toString();

                // Iterate over files in the directory to locate the corresponding .kicad_mod file
                for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                    if (entry.path().extension() == ".kicad_mod") {
                        std::filesystem::path originalFile = entry.path();

                        // Write the updated content to the original file
                        std::ofstream outFile(originalFile);
                        if (outFile) {
                            outFile << updatedContent;
                            outFile.close();
                            std::cout << GREEN << "Wrote updated file: " <<BOLD_PURPLE << originalFile.string() << RESET << std::endl;
                        } else {
                            std::cerr << RED << "Failed to write updated file: " << originalFile.string() << RESET << std::endl;
                        }
                    }
                }
            } else {
                std::cerr << RED << "No model node found in this file." << RESET << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << RED << "Error processing file: " << e.what() << RESET << std::endl;
        }
    }
}
