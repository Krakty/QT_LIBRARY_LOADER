// Kicad_Sym_FileHandler.h
#ifndef KICAD_SYM_FILEHANDLER_H
#define KICAD_SYM_FILEHANDLER_H

#include "s_expr_node_symbol.h"
#include <string>
#include <memory>

class KicadSymFileHandler {
public:
    // Load an S-expression tree from a file
    static std::shared_ptr<SeExprNodeSymbol> loadFromFile(const std::string& filePath);

    // Save an S-expression tree to a file
    static void saveToFile(const std::shared_ptr<SeExprNodeSymbol>& root, const std::string& filePath);

    // Update a specific property in an S-expression tree
    static void updateProperty(std::shared_ptr<SeExprNodeSymbol>& root, const std::string& key, const std::string& newValue);
};

#endif // KICAD_SYM_FILEHANDLER_H
