#ifndef S_EXPR_MERGE_SYMBOL_H
#define S_EXPR_MERGE_SYMBOL_H

#include <string>
//#include <memory>
//#include "s_expr_node_symbol.h"

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
// ANSI color codes for console output
#define RESET       "\033[0m"
#define BOLD_PURPLE "\033[1;35m"

#define BLACK         "\033[30m"
#define RED           "\033[31m"
#define GREEN         "\033[32m"
#define YELLOW        "\033[33m"
#define BLUE          "\033[34m"
#define PURPLE        "\033[35m"
#define CYAN          "\033[36m"
#define WHITE         "\033[37m"

#define BRIGHT_BLACK  "\033[90m"
#define BRIGHT_RED    "\033[91m"
#define BRIGHT_GREEN  "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE   "\033[94m"
#define BRIGHT_PURPLE "\033[95m"
#define BRIGHT_CYAN   "\033[96m"
#define BRIGHT_WHITE  "\033[97m"

#define BOLD_BLACK         "\033[1;30m"
#define BOLD_RED           "\033[1;31m"
#define BOLD_GREEN         "\033[1;32m"
#define BOLD_YELLOW        "\033[1;33m"
#define BOLD_BLUE          "\033[1;34m"
#define BOLD_PURPLE        "\033[1;35m"
#define BOLD_CYAN          "\033[1;36m"
#define BOLD_WHITE         "\033[1;37m"

#define BOLD_BRIGHT_BLACK  "\033[1;90m"
#define BOLD_BRIGHT_RED    "\033[1;91m"
#define BOLD_BRIGHT_GREEN  "\033[1;92m"
#define BOLD_BRIGHT_YELLOW "\033[1;93m"
#define BOLD_BRIGHT_BLUE   "\033[1;94m"
#define BOLD_BRIGHT_PURPLE "\033[1;95m"
#define BOLD_BRIGHT_CYAN   "\033[1;96m"
#define BOLD_BRIGHT_WHITE  "\033[1;97m"
#define CLEAR_SCREEN       "\033[2J\033[H"

} // namespace SeExprMergeSymbol

#endif // S_EXPR_MERGE_SYMBOL_H
