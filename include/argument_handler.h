#ifndef ARGUMENT_HANDLER_H
#define ARGUMENT_HANDLER_H

#include "config.h"
#include <string>

// Enum for program execution states
enum class ProgramExecutionState {
    EXIT_SUCCESS_STATE,
    EXIT_FAILURE_STATE,
    PROCESS_ZIP_STATE
};

// Function to handle command-line arguments
ProgramExecutionState handleCommandLineArguments(int argc, char* argv[], std::string& zipFilePath);

#endif // ARGUMENT_HANDLER_H
