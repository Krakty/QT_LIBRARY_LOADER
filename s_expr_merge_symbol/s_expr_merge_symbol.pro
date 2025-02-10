TEMPLATE = lib
CONFIG += staticlib c++17

TARGET = s_expr_merge_symbol

# Include paths for both local headers and s_expr_parser_symbol headers
INCLUDEPATH += include \
               ../s_expr_parser_symbol/include

SOURCES += src/s_expr_merge_symbol.cpp

HEADERS += include/s_expr_merge_symbol.h

# Link against s_expr_parser_symbol
LIBS += -L../s_expr_parser_symbol -ls_expr_parser_symbol

