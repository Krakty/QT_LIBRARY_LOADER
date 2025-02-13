# QT_LIBRARY_LOADER

QT_LIBRARY_LOADER is a C++ project designed to parse and manipulate KiCad library files (`.kicad_sym` and `.kicad_mod`) using custom s_expr parsing libraries. This project aims to enable the modification of key attributes within these files, such as symbol names, footprint references, and model file paths.

## Features

- **Parsing `.kicad_sym` Files**: Extracts and modifies symbol properties.
- **Parsing `.kicad_mod` Files**: Extracts and modifies footprint attributes.
- **C++ Implementation**: Provides a structured and extendable approach for handling KiCad files.
- **Operation**: This program will extract the zip file containing the library and make a few modifications  
   1.)  the files are renamed to the basename of the zip file for consistancy by default libraries from mouser don't necessarly have matching footprints and symbol names  
   2.)  the 3D Model path is changed inside the .kicad_mod file to point to the correct path of the file.  
   3.)  inside the .kicad_sym file the footprint is modified to point to the correct footprint and it's location  

## Project Structure
```
/kicad-library-parser
│── /include               # Header files
│── /src                   # Source files
├── s_expr_merge_symbol    # Merge code to merge our modified symbol into the master symbol library
    │── /include           # Header files
    │── /src               # Source files
    │── CMakeLists.txt     # Build system configuration
│── /s_expr_parser_sym     # Custom s_expr symbol parser
    │── /include           # Header files
    │── /src               # Source files
    │── CMakeLists.txt     # Build system configuration
│── /s_expr_parser_mod     # Custom s_expr module parser
    │── /include           # Header files
    │── /src               # Source files
    │── CMakeLists.txt     # Build system configuration
│── /tests                 # Unit tests
│── CMakeLists.txt         # Build system configuration
│── README.md              # Project documentation
```

## TODO

## Dependencies

- **QMake**: Used for building the project.
- **Qt Creator**: Recommended development environment.

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/your-repository/QT_LIBRARY_LOADER.git
cd QT_LIBRARY_LOADER

### Build the project

mkdir build
cd build
qmake ..
make -j$(nproc)


### Usage

./QT_LIBRARY_LOADER LIBRARY.ZIP

modify the QT_LIBRARY_LOADER.json file for your use case.
these should be rather obvious as to what there usage is Footprint 
and 3D_Model are KICAD Path Variables used to indicate the directory path
to the location where the 3D_Models are located.


{
    "Library_Location": "/home/<username>/CVS/Kicad_Libraries/",
    "Footprint_Location": "SamacSys_Parts.pretty/",
    "Footprint": "${SAMACSYS_FP}",
    "3D_Model": "${SAMACSYS_3D_SHAPES}",
    "Symbol_Library_Name": "SamacSys_Parts.kicad_sym",
    "TMP_FOLDER_ROOT":"./tmp"
}



EXAMPLE

### Contributing

Contributions are welcome!  This is the first project i've developed and would love the feedback


