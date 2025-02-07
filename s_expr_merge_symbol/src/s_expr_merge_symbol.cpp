#include "s_expr_merge_symbol.h"
#include "s_expr_node_symbol.h"
#include "s_expr_parser_symbol.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "config.h"

namespace SeExprMergeSymbol {

/**************************************************************
 *             Helper Functions / Internal Logic
 **************************************************************/

/**
 * Check if node is a list with at least one child whose value == target.
 */
static bool isListNodeWithFirstAtom(const std::shared_ptr<SeExprNodeSymbol>& node,
                                    const std::string& target)
{
    if (!node || node->type != SeExprNodeSymbol::NodeType::List || node->children.empty()) return false;

    auto firstChild = node->children[0];
    return firstChild && firstChild->type == SeExprNodeSymbol::NodeType::Atom && firstChild->value == target;
}

/**
 * Find the top-level node that starts with (kicad_symbol_lib ...).
 */
static std::shared_ptr<SeExprNodeSymbol> findKicadSymbolLib(const std::shared_ptr<SeExprNodeSymbol>& root)
{
    if (!root) return nullptr;

    if (isListNodeWithFirstAtom(root, "kicad_symbol_lib")) {
        return root;
    }

    if (root->type == SeExprNodeSymbol::NodeType::List) {
        for (auto& child : root->children) {
            auto found = findKicadSymbolLib(child);
            if (found) return found;
        }
    }
    return nullptr;
}

/**
 * Gather all (symbol "Name" ...) children of a (kicad_symbol_lib ...) node.
 */
static std::vector<std::shared_ptr<SeExprNodeSymbol>> getAllSymbolNodes(const std::shared_ptr<SeExprNodeSymbol>& libNode)
{
    std::vector<std::shared_ptr<SeExprNodeSymbol>> symbols;
    if (!libNode || libNode->type != SeExprNodeSymbol::NodeType::List) return symbols;

    for (auto& child : libNode->children) {
        if (isListNodeWithFirstAtom(child, "symbol")) {
            symbols.push_back(child);
        }
    }
    return symbols;
}

/**
 * Extract the symbol name from (symbol "NAME" ...).
 */
static std::string getSymbolName(const std::shared_ptr<SeExprNodeSymbol>& symbolNode)
{
    if (!symbolNode || symbolNode->type != SeExprNodeSymbol::NodeType::List || symbolNode->children.size() < 2) {
        return "";
    }

    auto nameNode = symbolNode->children[1];
    if (!nameNode || nameNode->type != SeExprNodeSymbol::NodeType::Atom) return "";

    std::string raw = nameNode->value;
    if (raw.size() >= 2 && raw.front() == '"' && raw.back() == '"') {
        return raw.substr(1, raw.size() - 2); // Remove leading/trailing quotes
    }
    return raw;
}

/**
 * Remove a symbol by name from the library node (if it exists).
 */
static void removeSymbolByName(const std::shared_ptr<SeExprNodeSymbol>& libNode,
                               const std::string& symbolName)
{
    if (!libNode || libNode->type != SeExprNodeSymbol::NodeType::List) return;
    auto& kids = libNode->children;

    kids.erase(std::remove_if(kids.begin(), kids.end(),
                              [&](std::shared_ptr<SeExprNodeSymbol> c) {
                                  return isListNodeWithFirstAtom(c, "symbol") && getSymbolName(c) == symbolName;
                              }),
               kids.end());
}

/**
 * Merge all symbols from targetLib into masterLib:
 *   - For each symbol in targetLib, remove any symbol with the same name in masterLib, then append.
 *   - Overwrites without validation, as per user preference.
 */
static void mergeSymbolLibrariesAST(const std::shared_ptr<SeExprNodeSymbol>& masterLib,
                                    const std::shared_ptr<SeExprNodeSymbol>& targetLib)
{
    if (!masterLib || !targetLib) return;

    auto targetSymbols = getAllSymbolNodes(targetLib);
    for (auto& sym : targetSymbols) {
        std::string name = getSymbolName(sym);
        if (!name.empty()) {
            removeSymbolByName(masterLib, name); // Always overwrite
            masterLib->addChild(sym);
        }
    }
}

/**************************************************************
 *           Public Function: mergeSymbolLibraryFiles
 **************************************************************/

bool mergeSymbolLibraryFiles(const std::string& masterPath,
                             const std::string& targetPath,
                             const std::string& outputPath)
{
    // Load & parse the master file
    std::ifstream masterFile(masterPath);
    if (!masterFile.is_open()) {
        std::cerr << "[ERROR] Failed to open master file: " << masterPath << "\n";
        return false;
    }
    std::string masterContent((std::istreambuf_iterator<char>(masterFile)), std::istreambuf_iterator<char>());
    masterFile.close();

    SeExprParserSymbol masterParser;
    std::shared_ptr<SeExprNodeSymbol> masterRoot;
    try {
        masterRoot = masterParser.parse(masterContent);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Parsing master file '" << masterPath << "': " << e.what() << "\n";
        return false;
    }

    // Load & parse the target file
    std::ifstream targetFile(targetPath);
    if (!targetFile.is_open()) {
        std::cerr << "[ERROR] Failed to open target file: " << targetPath << "\n";
        return false;
    }
    std::string targetContent((std::istreambuf_iterator<char>(targetFile)), std::istreambuf_iterator<char>());
    targetFile.close();

    SeExprParserSymbol targetParser;
    std::shared_ptr<SeExprNodeSymbol> targetRoot;
    try {
        targetRoot = targetParser.parse(targetContent);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Parsing target file '" << targetPath << "': " << e.what() << "\n";
        return false;
    }

    // Find the (kicad_symbol_lib ...) node in each
    auto masterLib = findKicadSymbolLib(masterRoot);
    auto targetLib = findKicadSymbolLib(targetRoot);
    if (!masterLib || !targetLib) {
        std::cerr << "[ERROR] Could not find (kicad_symbol_lib ...) in master or target.\n";
        return false;
    }

    // Merge all symbols from targetLib into masterLib
    mergeSymbolLibrariesAST(masterLib, targetLib);

    // Write the merged master to outputPath
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "[ERROR] Failed to open output file: " << outputPath << "\n";
        return false;
    }

    outFile << masterRoot->toString() << "\n";
    outFile.close();

    std::cout << CYAN << "[INFO] Successfully merged symbols from:\n"
              << RED << "\nSOURCE:             " << YELLOW << targetPath << RESET
              << RED << "\nDESTINATION:        " << YELLOW << masterPath << RESET
              << "\n\n";
    return true;
}

} // namespace SeExprMergeSymbol
