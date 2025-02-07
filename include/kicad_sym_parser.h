#ifndef KICAD_SYM_PARSER_H
#define KICAD_SYM_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include "s_expr_node_symbol.h"

/**
 * Parses `.kicad_sym` files within a specified directory and returns the parsed S-expression trees.
 */
std::vector<std::shared_ptr<SeExprNodeSymbol>> parseKicadSymFiles(const std::string& directory);

#endif // KICAD_SYM_PARSER_H
