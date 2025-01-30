cmake_minimum_required(VERSION 3.16)
project(s_expr_parser_symbol)

# Define the s_expr_parser_symbol library
add_library(s_expr_parser_symbol STATIC
    src/s_expr_node_symbol.cpp
    src/s_expr_parser_symbol.cpp
)

# Specify include directories for the library
target_include_directories(s_expr_parser_symbol
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
