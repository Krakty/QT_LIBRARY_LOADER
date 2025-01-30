// Kicad_Mod_FileHandler.h
#ifndef KICAD_MOD_FILEHANDLER_H
#define KICAD_MOD_FILEHANDLER_H

#include "s_expr_node_mod.h"
#include <string>
#include <memory>

class KicadModFileHandler {
public:
    // Load an S-expression tree from a file
    static std::shared_ptr<SExprNodeMod> loadFromFile(const std::string& filePath);

    // Save an S-expression tree to a file
    static void saveToFile(const std::shared_ptr<SExprNodeMod>& root, const std::string& filePath);

    // Update a specific property in an S-expression tree
    static void updateProperty(std::shared_ptr<SExprNodeMod>& root, const std::string& key, const std::string& newValue);
};

#endif // KICAD_MOD_FILEHANDLER_H
