#ifndef KICAD_MOD_PARSER_H
#define KICAD_MOD_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include "s_expr_node_mod.h"

// Function to parse all .kicad_mod files in the specified directory
std::vector<std::shared_ptr<SExprNodeMod>> parseKicadModFiles(const std::string& directory);

#endif // KICAD_MOD_PARSER_H
