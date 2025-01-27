#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

struct Config {
    std::string libraryLocation;
    std::string footprintLocation;
    std::string footprint;
    std::string model3D;
    std::string symbolLibraryName;
    std::string tmpFolderRoot;
};

extern Config globalConfig;

// Global variables for temporary folder paths
extern std::string ExtractedLibrary;
extern std::string ModifiedLibrary;

// List of file extensions to search for
extern const std::vector<std::string> FileExtensions;

#endif // CONFIG_H
