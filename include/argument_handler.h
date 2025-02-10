#ifndef ARGUMENT_HANDLER_H
#define ARGUMENT_HANDLER_H

#include "config.h"
#include <string>

enum class ProgramExecutionState {  // Use enum class for strong typing
    EXIT_SUCCESS_STATE,
    EXIT_FAILURE_STATE,
    PROCESS_ZIP_STATE
};

ProgramExecutionState handleCommandLineArguments(int argc, char* argv[], std::string& zipFilePath);

#endif // ARGUMENT_HANDLER_H
