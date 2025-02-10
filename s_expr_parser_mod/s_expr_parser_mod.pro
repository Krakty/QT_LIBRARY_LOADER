TEMPLATE = lib
CONFIG += staticlib c++17

TARGET = s_expr_parser_mod

INCLUDEPATH += include

SOURCES += src/s_expr_node_mod.cpp \
           src/s_expr_parser_mod.cpp

HEADERS += include/s_expr_node_mod.h \
           include/s_expr_parser_mod.h

