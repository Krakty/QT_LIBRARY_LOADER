cmake_minimum_required(VERSION 3.16)
project(s_expr_merge_symbol)

# Define the s_expr_merge_symbol library from your .cpp files
add_library(s_expr_merge_symbol STATIC
    src/s_expr_merge_symbol.cpp
)

# Provide the path(s) where s_expr_merge_symbol can find its own headers
# as well as the headers from s_expr_parser_symbol.
target_include_directories(s_expr_merge_symbol
    PUBLIC
    # The local include directory for s_expr_merge_symbol (if you have headers here)
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    # If the parser's headers are not automatically exposed by the s_expr_parser_symbol library,
    # explicitly add the directory containing s_expr_parser_symbol.h and s_expr_node_symbol.h:
    ${CMAKE_SOURCE_DIR}/s_expr_parser_symbol/include
)

# Link the parser library. Because we specify PUBLIC, anything linking to
# s_expr_merge_symbol also gets access to s_expr_parser_symbol automatically.
target_link_libraries(s_expr_merge_symbol
    PUBLIC
    s_expr_parser_symbol
)
