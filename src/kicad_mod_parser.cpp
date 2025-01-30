#include "kicad_mod_parser.h"
#include "file_utils.h" // For getFilesWithExtension
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "s_expr_parser_mod.h"

// Parses all .kicad_mod files in the specified directory
std::vector<std::shared_ptr<SExprNodeMod>> parseKicadModFiles(const std::string& directory) {
    std::vector<std::shared_ptr<SExprNodeMod>> parsedFiles;

    // Check if the directory exists
    if (!std::filesystem::exists(directory)) {
        throw std::runtime_error("Error: Directory does not exist: " + directory);
    }

    // Get all .kicad_mod files in the directory
    std::vector<std::string> modFiles = getFilesWithExtension(directory, ".kicad_mod");

    // Parse each file
    for (const auto& file : modFiles) {
        std::ifstream inFile(file);
        if (!inFile.is_open()) {
            std::cerr << "Error: Unable to open file " << file << std::endl;
            continue;
        }

        // Read file content into a string
        std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        try {
            // Parse the S-expression content
            auto parsed = SExprParserMod::parse(content);
            parsedFiles.push_back(parsed);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing file " << file << ": " << e.what() << std::endl;
        }
    }

    return parsedFiles;
}
