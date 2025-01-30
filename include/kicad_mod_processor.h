#ifndef KICAD_MOD_PROCESSOR_H
#define KICAD_MOD_PROCESSOR_H

#include <string>
#include <memory>
#include "config.h"
#include "s_expr_node_mod.h"

// Function to process `.kicad_mod` files in the specified directory
void processKicadModFiles(const std::string& directory, const Config& config, const std::string& baseName);

// Function to find the correct 3D file in the directory
std::string find3DFile(const std::string& directory, const std::string& baseName);

// Function to update the 3D model path in the 'model' node
void updateModelPath(std::shared_ptr<SExprNodeMod> modelNode, const Config& config, const std::string& baseName);

#endif // KICAD_MOD_PROCESSOR_H
