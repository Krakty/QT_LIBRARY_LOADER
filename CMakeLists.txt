cmake_minimum_required(VERSION 3.16)

project(QT_LIBRARY_LOADER LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the nlohmann json library and minizip
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/nlohmann)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


# Add the executable
add_executable(QT_LIBRARY_LOADER
    src/main.cpp
    src/json_loader.cpp
    src/unzipper.cpp
    src/file_manager.cpp
    src/config.cpp
    src/Kicad_Mod_FileHandler.cpp
    src/Kicad_Sym_FileHandler.cpp
    src/kicad_sym_processor.cpp
    include/kicad_sym_processor.h
    src/kicad_sym_parser.cpp
    include/kicad_sym_parser.h
    include/file_utils.h
    src/file_utils.cpp
    src/kicad_mod_processor.cpp
    src/kicad_mod_parser.cpp
    include/kicad_mod_parser.h
    include/kicad_mod_processor.h
    src/recompress_library.cpp
    include/recompress_library.h
)


# Include the s_expr library as a subdirectory
add_subdirectory(s_expr_parser_mod)
add_subdirectory(s_expr_parser_symbol)


# Link libraries
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MINIZIP REQUIRED minizip)

target_link_libraries(QT_LIBRARY_LOADER
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE ${MINIZIP_LIBRARIES}
    s_expr_parser_symbol
    s_expr_parser_mod
)

# Install targets
include(GNUInstallDirs)
install(TARGETS QT_LIBRARY_LOADER
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
