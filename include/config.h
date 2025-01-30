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

// ANSI color codes
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define PURPLE "\033[35m"
#define BOLD_PURPLE "\033[1;35m"
#define RED "\033[0;31m"

// Global variables for temporary folder paths
extern std::string ExtractedLibrary;
extern std::string ModifiedLibrary;

// List of file extensions to search for
extern const std::vector<std::string> FileExtensions;
extern const std::vector<std::string> FileTypes3D;

#endif // CONFIG_H
