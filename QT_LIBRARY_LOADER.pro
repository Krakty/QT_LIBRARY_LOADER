TEMPLATE = subdirs

# Add subprojects (libraries and the main application)
SUBDIRS += s_expr_parser_mod \
           s_expr_parser_symbol \
           s_expr_merge_symbol \
           QT_LIBRARY_LOADER_APP

# Specify the path to the app's .pro file
QT_LIBRARY_LOADER_APP.file = app/QT_LIBRARY_LOADER_APP.pro
QT_LIBRARY_LOADER_APP.target = QT_LIBRARY_LOADER_APP

# Define dependencies (build order)
s_expr_parser_symbol.depends = s_expr_parser_mod
s_expr_merge_symbol.depends = s_expr_parser_symbol
QT_LIBRARY_LOADER_APP.depends = s_expr_merge_symbol
