cmake_minimum_required(VERSION 3.16)
project(s_expr_parser_mod)

# Define the s_expr_parser_mod static library
add_library(s_expr_parser_mod STATIC
    src/s_expr_node_mod.cpp
    src/s_expr_parser_mod.cpp
)

# Specify include directories for the library
target_include_directories(s_expr_parser_mod
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
