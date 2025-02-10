TEMPLATE = lib
CONFIG += staticlib c++17

TARGET = s_expr_parser_symbol

INCLUDEPATH += include

SOURCES += src/s_expr_node_symbol.cpp \
           src/s_expr_parser_symbol.cpp

HEADERS += include/s_expr_node_symbol.h \
           include/s_expr_parser_symbol.h

