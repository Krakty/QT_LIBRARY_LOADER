#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

void pauseExecution(const std::string& message = "Press [Space] or [Enter] to continue...");

/**
 * Configuration settings used throughout the application.
 * Holds paths and identifiers for KiCad libraries, models, and other resources.
 */
struct Config {
    std::string libraryLocation;       // Path to the KiCad symbol library
    std::string footprintLocation;     // Path to the KiCad footprint library
    std::string footprint;             // Default footprint name
    std::string model3D;               // Default 3D model name
    std::string symbolLibraryName;     // Name of the master `.kicad_sym` library
    std::string tmpFolderRoot;         // Root temporary directory
    std::string model3DLocation;       // Location of 3D models
};

// Global configuration instance
extern Config globalConfig;

// Global variables for temporary folder paths
extern std::string ExtractedLibrary;   // Path to the extracted library
extern std::string ModifiedLibrary;    // Path to the modified library

// List of file extensions to search for
extern const std::vector<std::string> FileExtensions; // Extensions for KiCad-related files
extern const std::vector<std::string> FileTypes3D;    // Supported 3D model file types

// Global variable to suppress console output
extern bool suppress_output;

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



#endif // CONFIG_H
