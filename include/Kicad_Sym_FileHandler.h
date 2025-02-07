#ifndef KICAD_SYM_FILEHANDLER_H
#define KICAD_SYM_FILEHANDLER_H

#include <memory>
#include <string>
#include "s_expr_node_symbol.h"

/**
 * Handles reading, writing, and updating `.kicad_sym` files.
 */
class KicadSymFileHandler {
public:
    /**
     * Loads an S-expression tree from the specified `.kicad_sym` file.
     *
     * @param filePath The path to the `.kicad_sym` file.
     * @return A shared pointer to the root of the S-expression tree.
     * @throws std::runtime_error if the file cannot be opened or parsed.
     */
    static std::shared_ptr<SeExprNodeSymbol> loadFromFile(const std::string& filePath);

    /**
     * Saves the S-expression tree to the specified `.kicad_sym` file.
     *
     * @param root     The root node of the S-expression tree to save.
     * @param filePath The path where the file will be saved.
     * @throws std::runtime_error if the file cannot be opened for writing.
     */
    static void saveToFile(const std::shared_ptr<SeExprNodeSymbol>& root, const std::string& filePath);

    /**
     * Updates the value of a specified property key within the S-expression tree.
     *
     * @param root     The root node of the `.kicad_sym` file.
     * @param key      The property key to update.
     * @param newValue The new value to assign to the key.
     * @throws std::runtime_error if the key is not found or invalid.
     */
    static void updateProperty(std::shared_ptr<SeExprNodeSymbol>& root, const std::string& key, const std::string& newValue);
};

#endif // KICAD_SYM_FILEHANDLER_H
