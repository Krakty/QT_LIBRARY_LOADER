cmake_minimum_required(VERSION 3.16)

project(QT_LIBRARY_LOADER LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the nlohmann json library and minizip
include_directories(${CMAKE_HOME_DIRECTORY}/../include) # Including ~/include
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find Boost
find_package(Boost REQUIRED)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Add the executable
add_executable(QT_LIBRARY_LOADER
    src/main.cpp
    src/json_loader.cpp
    src/unzipper.cpp
    src/file_manager.cpp
    src/kicad_parser.cpp
    src/config.cpp
)

# Link libraries
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MINIZIP REQUIRED minizip)

target_link_libraries(QT_LIBRARY_LOADER PRIVATE Qt${QT_VERSION_MAJOR}::Core nlohmann_json::nlohmann_json ${MINIZIP_LIBRARIES} ${Boost_LIBRARIES})

# Install targets
include(GNUInstallDirs)
install(TARGETS QT_LIBRARY_LOADER
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
