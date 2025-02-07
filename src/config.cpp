#include "config.h"
#include <iostream>
#include <string>
#include <vector>

// Define global variables (exactly once)
std::string ExtractedLibrary;
std::string ModifiedLibrary;
Config globalConfig;

// Define file extension lists
const std::vector<std::string> FileExtensions = {
    ".epw", ".stp", ".wrl", ".step", ".wrz", ".x3d", ".idf", ".emn",
    ".kicad_sym", ".kicad_mod"
};

const std::vector<std::string> FileTypes3D = {
    ".stp", ".wrl", ".step"
};

// Pause function
void pauseExecution(const std::string& message) {
    std::cout << BOLD_BRIGHT_WHITE << message << RESET << std::endl;
    char ch;
    do {
        ch = std::cin.get();  // Wait for user input
    } while (ch != ' ' && ch != '\n');  // Continue only on Space or Enter
}
