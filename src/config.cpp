#include "config.h"

// Initialize global variables
std::string ExtractedLibrary;
std::string ModifiedLibrary;

// List of file extensions to search for
const std::vector<std::string> FileExtensions = {
    ".epw", ".stp", ".wrl", ".step", ".wrz", ".x3d", ".idf", ".emn",
    ".kicad_sym", ".kicad_mod"
};
