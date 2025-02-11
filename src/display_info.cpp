#include "display_info.h"
#include "config.h"
#include <iostream>

void DisplayInfo(const std::string& message) {
    // Check if we should suppress output
    if (suppress_output) return;

    const int totalWidth = 70;               // Total width of the banner
    const std::string border(totalWidth, '*'); // Creates a line of 70 asterisks
    const int contentWidth = totalWidth - 8;   // Subtract 8 for the "****" on both sides

    std::cout << BRIGHT_RED << border << "\n";

    // Center the message within the content width
    int totalPadding = contentWidth - message.length();
    int paddingLeft = totalPadding / 2;
    int paddingRight = totalPadding - paddingLeft;

    std::cout << "****"
              << std::string(paddingLeft, ' ')
              << BOLD_BRIGHT_PURPLE << message << BRIGHT_RED
              << std::string(paddingRight, ' ')
              << "****" << "\n";

    std::cout << border << "\n" << RESET;
}

void DisplayInfo_hdr(const std::string& message) {
    // Check if we should suppress output
    if (suppress_output) return;

    const int totalWidth = 70;               // Total width of the banner
    const std::string header(totalWidth, '*'); // Creates a line of 70 asterisks
    //const int contentWidth = totalWidth - 8;   // Subtract 8 for the "****" on both sides

    std::cout << BRIGHT_RED << header << "\n";
    std::cout << CYAN << message << "\n";
}

void DisplayMessage(const std::string& message) {
    // Check if we should suppress output
    if (suppress_output) return;
    std::cout << message ;
}

