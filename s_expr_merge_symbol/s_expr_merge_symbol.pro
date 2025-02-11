TEMPLATE = lib
CONFIG += staticlib c++17

TARGET = s_expr_merge_symbol

INCLUDEPATH += include \
               ../s_expr_parser_symbol/include  # Dependency on s_expr_parser_symbol

SOURCES += src/s_expr_merge_symbol.cpp

HEADERS += include/s_expr_merge_symbol.h

# Link against s_expr_parser_symbol
LIBS += -L$$OUT_PWD/../s_expr_parser_symbol -ls_expr_parser_symbol
