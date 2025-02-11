#include "kicad_mod_processor.h"
#include "s_expr_node_mod.h"
#include "s_expr_parser_mod.h"
#include "config.h"
#include "display_info.h"
#include <sstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

namespace fs = std::filesystem;

// Optional single-file parsing helper
std::vector<std::shared_ptr<SExprNodeMod>> parseKicadModFileSingle(const std::string &filePath)
{
    std::vector<std::shared_ptr<SExprNodeMod>> result;
    try {
        std::ifstream inFile(filePath);
        if (!inFile.is_open()) {
            throw std::runtime_error("Unable to open file: " + filePath);
        }
        std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        auto root = SExprParserMod::parse(content);
        if (root) {
            result.push_back(root);
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception in parseKicadModFileSingle: " << e.what() << std::endl;
    }
    return result;
}


void KicadModProcessor::processKicadModFiles(const std::string &directory,
                                             const Config &config,
                                             const std::string &baseName)
{
    if (!fs::exists(directory)) {
        std::cout << "Directory does not exist. Creating: " << directory << std::endl;
        fs::create_directory(directory);
    }

    // Collect all .kicad_mod files
    std::vector<std::string> modFiles;
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".kicad_mod") {
            modFiles.push_back(entry.path().string());
        }
    }

    if (modFiles.empty()) {
        std::cout << "No .kicad_mod files found in directory: " << directory << std::endl;
        return;
    }

    // We'll process each .kicad_mod file in a separate thread
    std::vector<std::thread> threads;
    threads.reserve(modFiles.size());

    static std::mutex ioMutex; // for thread-safe logging

    for (const auto &filePath : modFiles) {
        threads.emplace_back([&, filePath]() {
            try {
                // Parse the file
                auto roots = parseKicadModFileSingle(filePath);
                if (roots.empty()) {
                    std::lock_guard<std::mutex> lock(ioMutex);
                    std::cerr << "Error: Could not parse " << filePath << std::endl;
                    return;
                }

                // Assume one root per file. If multiple, handle them similarly in a loop.
                auto root = roots.front();

                // Check for existing (model ...) node
                auto modelNode = root->findKey("model");
                if (modelNode) {
                    updateModelPath(modelNode, config, baseName);
                } else {
                    insertModelNode(root, config, baseName);
                }

                // Write updated content back to the same file
                std::string updatedContent = root->toString();
                {
                    std::lock_guard<std::mutex> lock(ioMutex);
                    std::ofstream outFile(filePath);
                    if (!outFile) {
                        std::cerr << "Failed to write updated file: " << filePath << std::endl;
                    } else {
                        outFile << updatedContent;
                        outFile.close();
                        std::ostringstream oss; //Used to pass information to DisplayMessage functions
                        oss << "Successfully updated: " << filePath << std::endl;
                        std::string message = oss.str();
                        DisplayMessage(message);
                        oss.str(""); // Clear the stringstream
                        oss.clear(); // Reset the flags
                    }
                }
            } catch (const std::exception &e) {
                std::lock_guard<std::mutex> lock(ioMutex);
                std::cerr << "Error processing file " << filePath << ": " << e.what() << std::endl;
            }
        });
    }

    // Join all threads before returning
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void KicadModProcessor::insertModelNode(std::shared_ptr<SExprNodeMod> root,
                                        const Config &config,
                                        const std::string &baseName)
{
    // 1) Determine which 3D file to use
    std::string localFileName;
    try {
        localFileName = find3DFile(ModifiedLibrary, baseName);
        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << RED << "\n3D File Identified... " << GREEN << localFileName << RESET << std::endl;
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags
        oss << RED << "Complete Path...      " << GREEN << ModifiedLibrary + "/" + localFileName << RESET << std::endl;
        message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags
    } catch (const std::exception& e) {
        std::cerr << "Error finding 3D file: " << e.what()
        << " Defaulting to .stp" << std::endl;
        localFileName = baseName + ".stp";
    }

    // 2) Check if a (model ...) node already exists
    if (root->toString().find("(model ") != std::string::npos) {
        DisplayMessage("Model node already exists. Updating path instead.\n");
        updateModelPath(root, config, baseName);
        return;
    }

    // 3) Build the final KiCad path using config.model3D
    //    e.g. "${SAMACSYS_3D_SHAPES}/myPart.stp"
    std::string finalModelPath = config.model3D + "/" + localFileName;

    // 4) Create the text for the new (model ...) node
    std::string modelNodeSExpr =
        "(model " + finalModelPath + "\n"
                                     "  (at (xyz 0 0 0))\n"
                                     "  (scale (xyz 1 1 1))\n"
                                     "  (rotate (xyz 0 0 0))\n"
                                     ")";

    // 5) Parse that text into a new S-expression subtree
    auto newModelNode = SExprParserMod::parse(modelNodeSExpr);
    if (!newModelNode) {
        std::cerr << "Error: Failed to parse the new model node.\n";
        return;
    }

    // 6) Append this new (model ...) node to the existing (module ...) root
    root->addChild(newModelNode);
    DisplayMessage("Inserted missing model node into the S-expression tree.\n");
}

void KicadModProcessor::updateModelPath(std::shared_ptr<SExprNodeMod> modelNode,
                                        const Config& config,
                                        const std::string& baseName)
{
    if (!modelNode || modelNode->type != SExprNodeMod::NodeType::List) {
        std::cerr << "Error: 'modelNode' is invalid or not a list!\n";
        return;
    }

    for (auto& child : modelNode->children) {
        if (child->type == SExprNodeMod::NodeType::Atom && child->value != "model") {
            try {
                std::string fileName = find3DFile(ModifiedLibrary, baseName);
                std::string newPath = config.model3D + "/" + fileName;
                std::ostringstream oss; //Used to pass information to DisplayMessage functions
                oss << "Updating model path from: " << child->value << " to: " << newPath << std::endl;
                std::string message = oss.str();
                DisplayMessage(message);
                oss.str(""); // Clear the stringstream
                oss.clear(); // Reset the flags

                child->value = newPath;
                return;
            } catch (const std::exception& e) {
                std::cerr << "Error finding 3D file:\n" << e.what() << std::endl;
            }
        }
    }

    std::cerr << "No valid model path found to update!\n";
}

std::string KicadModProcessor::find3DFile(const std::string& directory,
                                          const std::string& baseName)
{
    // Prefer .stp, .step, else fallback to .wrl, etc.
    // This is consistent with FileTypes3D but we can refine if needed.

    for (const auto& ext : FileTypes3D) {
        fs::path filePath = fs::path(directory) / (baseName + ext);
        std::ostringstream oss; //Used to pass information to DisplayMessage functions
        oss << RED << "\nSearching for...      " << GREEN << filePath << RESET << std::endl;
        std::string message = oss.str();
        DisplayMessage(message);
        oss.str(""); // Clear the stringstream
        oss.clear(); // Reset the flags

        if (fs::exists(filePath)) {
            return filePath.filename().string();
        }
    }

    throw std::runtime_error("3D file not found with any known extension for base name: " + baseName);
}
