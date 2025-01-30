#include "config.h"

// Initialize global variables
std::string ExtractedLibrary;
std::string ModifiedLibrary;

Config globalConfig;  // Define globalConfig

// List of file extensions to search for
const std::vector<std::string> FileExtensions = {
    ".epw", ".stp", ".wrl", ".step", ".wrz", ".x3d", ".idf", ".emn",
    ".kicad_sym", ".kicad_mod"
};

// List of 3d Files were interested in extracting

const std::vector<std::string> FileTypes3D = {
    ".stp", ".wrl", ".step"
};
