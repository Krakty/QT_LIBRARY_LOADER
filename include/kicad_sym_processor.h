#ifndef KICAD_SYM_PROCESSOR_H
#define KICAD_SYM_PROCESSOR_H

#include <string>
#include "config.h"

// Function to process `.kicad_sym` files in the specified directory
void processKicadSymFiles(const std::string& directory, const Config& config);

#endif // KICAD_SYM_PROCESSOR_H
