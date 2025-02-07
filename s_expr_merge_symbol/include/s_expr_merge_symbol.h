#ifndef S_EXPR_MERGE_SYMBOL_H
#define S_EXPR_MERGE_SYMBOL_H

#include <string>
#include <memory>
#include "s_expr_node_symbol.h"

/**
 * Namespace for handling the merging of `.kicad_sym` symbol libraries.
 */
namespace SeExprMergeSymbol {

/**
     * Merges symbols from a target `.kicad_sym` file into a master `.kicad_sym` file.
     *
     * - For each symbol in the target file:
     *   - Any symbol with the same name in the master file is removed (always overwrites).
     *   - The new symbol from the target file is added to the master file.
     *
     * @param masterPath The path to the master `.kicad_sym` file.
     * @param targetPath The path to the target `.kicad_sym` file containing new/updated symbols.
     * @param outputPath The path where the merged output will be saved (can overwrite the master).
     *
     * @return `true` if the merge was successful, `false` otherwise.
     */
bool mergeSymbolLibraryFiles(const std::string& masterPath,
                             const std::string& targetPath,
                             const std::string& outputPath);

} // namespace SeExprMergeSymbol

#endif // S_EXPR_MERGE_SYMBOL_H
