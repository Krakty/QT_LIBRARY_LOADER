#ifndef KICAD_SYM_PROCESSOR_H
#define KICAD_SYM_PROCESSOR_H

#include <string>
#include <memory>
#include "s_expr_node_symbol.h"
#include "config.h"

/**
 * Updates the "Footprint" property of the given S-expression tree root.
 *
 * @param root     The root node of the `.kicad_sym` file.
 * @param basename The new basename to set in the "Footprint" property.
 */
void updateFootprintProperty(std::shared_ptr<SeExprNodeSymbol> root, const std::string& basename);

/**
 * Updates the "ki_fp_filters" property of the given S-expression tree root.
 * Only applies if there are multiple `.kicad_mod` files.
 *
 * @param root      The root node of the `.kicad_sym` file.
 * @param basenames A list of basenames derived from `.kicad_mod` files.
 */
void updateKiFpFiltersProperty(std::shared_ptr<SeExprNodeSymbol> root, const std::vector<std::string>& basenames);

/**
 * Processes a `.kicad_sym` file:
 *  - Updates the "Footprint" property based on `.kicad_mod` files found.
 *  - Updates the "ki_fp_filters" property only if multiple `.kicad_mod` files are detected.
 *
 * @param directory The directory containing the `.kicad_sym` file.
 * @param config    Global configuration settings for the process.
 */
//void processKicadSym(const std::string& directory, const Config& config);
void processKicadSym(const std::string& symFilePath, const Config& config);
#endif // KICAD_SYM_PROCESSOR_H
