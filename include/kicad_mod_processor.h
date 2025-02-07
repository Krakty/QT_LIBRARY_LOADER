#ifndef KICAD_MOD_PROCESSOR_H
#define KICAD_MOD_PROCESSOR_H

#include <string>
#include <memory>
#include "config.h"
#include "s_expr_node_mod.h"

/**
 * Handles processing of `.kicad_mod` files, including inserting model nodes,
 * updating model paths, and locating 3D model files.
 */
class KicadModProcessor {
public:
    /**
     * Processes all `.kicad_mod` files in the specified directory.
     *
     * - Updates model paths based on the provided configuration.
     * - Inserts model nodes if missing.
     *
     * @param directory The directory containing `.kicad_mod` files.
     * @param config    Global configuration settings for the process.
     * @param baseName  The base name used for file identification and property updates.
     */
    void processKicadModFiles(const std::string& directory, const Config& config, const std::string& baseName);

private:
    /**
     * Inserts a 3D model node into the `.kicad_mod` file if it doesn't exist.
     *
     * @param root     The root node of the S-expression tree for the `.kicad_mod` file.
     * @param config   Global configuration settings.
     * @param baseName The base name for identifying the corresponding model file.
     */
    void insertModelNode(std::shared_ptr<SExprNodeMod> root, const Config& config, const std::string& baseName);

    /**
     * Updates the model path within an existing 3D model node.
     *
     * @param modelNode The node representing the 3D model within the `.kicad_mod` file.
     * @param config    Global configuration settings.
     * @param baseName  The base name for determining the correct model path.
     */
    void updateModelPath(std::shared_ptr<SExprNodeMod> modelNode, const Config& config, const std::string& baseName);

    /**
     * Searches for a 3D model file in the specified directory that matches the given base name.
     *
     * @param directory The directory to search for 3D model files.
     * @param baseName  The base name used to identify the correct 3D model file.
     * @return The path to the 3D model file if found; otherwise, an empty string.
     */
    std::string find3DFile(const std::string& directory, const std::string& baseName);
};

#endif // KICAD_MOD_PROCESSOR_H
