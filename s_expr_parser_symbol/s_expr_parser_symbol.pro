TEMPLATE = lib
CONFIG += staticlib c++17

TARGET = s_expr_parser_symbol

INCLUDEPATH += include \
               ../s_expr_parser_mod/include  # Dependency on s_expr_parser_mod

SOURCES += src/s_expr_node_symbol.cpp \
           src/s_expr_parser_symbol.cpp

HEADERS += include/s_expr_node_symbol.h \
           include/s_expr_parser_symbol.h

# Link against s_expr_parser_mod
LIBS += -L$$OUT_PWD/../s_expr_parser_mod -ls_expr_parser_mod
