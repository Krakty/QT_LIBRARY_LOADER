# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Main Build Process
```bash
# Build the entire project (builds all sub-libraries and main app)
mkdir build
cd build
qmake ..
make -j$(nproc)
```

### Individual Component Builds
```bash
# Build specific components individually
qmake s_expr_parser_mod.pro && make
qmake s_expr_parser_symbol.pro && make
qmake s_expr_merge_symbol.pro && make
qmake app/QT_LIBRARY_LOADER_APP.pro && make
```

### Running the Application
```bash
# From build directory
./QT_LIBRARY_LOADER LIBRARY.ZIP

# Or with command line options
./QT_LIBRARY_LOADER -SuppressOutput LIBRARY.ZIP
./QT_LIBRARY_LOADER -Defaults  # Shows default configuration
```

## Architecture Overview

QT_LIBRARY_LOADER is a C++ application that processes KiCad library files (.kicad_sym and .kicad_mod) from compressed archives. The architecture consists of:

### Core Components
1. **Main Application** (`app/`) - Entry point and orchestration
2. **S-Expression Parsers** - Custom parsers for KiCad file formats:
   - `s_expr_parser_mod/` - Parses .kicad_mod files (footprints)
   - `s_expr_parser_symbol/` - Parses .kicad_sym files (symbols)
   - `s_expr_merge_symbol/` - Merges modified symbols back to master library

### Key Processing Flow
1. **File Extraction**: Unzips library archives using `unzipper.cpp`
2. **File Management**: Handles temporary directories and file operations via `FileManager`
3. **Configuration**: Loads settings from `QT_LIBRARY_LOADER.json`
4. **Processing Pipeline**:
   - **KicadModProcessor**: Updates 3D model paths in .kicad_mod files
   - **KicadSymProcessor**: Updates footprint references in .kicad_sym files
   - **Symbol Merge**: Integrates modified symbols into master library
5. **Library Recompression**: Packages processed files back into archives

### Data Flow
- Input: ZIP archive containing KiCad library files
- Extraction to temporary directory structure
- File renaming for consistency (basename matching)
- Path updates for 3D models and footprint references
- Symbol merging with master library
- Output: Updated library files in target locations

### Configuration System
The application uses `QT_LIBRARY_LOADER.json` for configuration:
- `Library_Location`: Target directory for KiCad libraries
- `Footprint_Location`: Footprint library subdirectory
- `Symbol_Library_Name`: Master symbol library file
- `Footprint`/`3D_Model`: KiCad path variables for references

### Dependencies
- Qt Core (C++17)
- minizip library for archive handling
- Custom S-expression parsing libraries
- nlohmann/json for configuration parsing

## Development Notes

### Build Dependencies
The project uses QMake with a hierarchical build system. The main `.pro` file defines build order:
1. `s_expr_parser_mod` (foundation)
2. `s_expr_parser_symbol` (depends on mod parser)
3. `s_expr_merge_symbol` (depends on symbol parser)
4. `QT_LIBRARY_LOADER_APP` (depends on merge functionality)

### Key Classes
- `FileManager`: File operations and temporary directory management
- `KicadModProcessor`: .kicad_mod file processing and 3D model path updates
- `KicadSymProcessor`: .kicad_sym file processing and footprint reference updates
- `Config`: Global configuration management
- `SeExprNode*`: S-expression tree nodes for parsing KiCad files

### Command Line Interface
The application supports several command-line options handled by `argument_handler.cpp`:
- `-SuppressOutput`: Reduces debug output
- `-Defaults`: Displays default configuration and exits
- Primary argument: Path to library ZIP file

### Error Handling
The codebase includes comprehensive error handling with a `ProgramExecutionState` enum for managing execution flow and early exits.