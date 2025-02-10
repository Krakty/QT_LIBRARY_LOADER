QT += core

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = QT_LIBRARY_LOADER

# Correct Include Paths
INCLUDEPATH += include \
               include/nlohmann \
               s_expr_parser_mod/include \
               s_expr_parser_symbol/include \
               s_expr_merge_symbol/include

# Source and Header Files (unchanged)
SOURCES += src/main.cpp \
           src/config.cpp \
           src/file_manager.cpp \
           src/file_utils.cpp \
           src/json_loader.cpp \
           src/unzipper.cpp \
           src/Kicad_Mod_FileHandler.cpp \
           src/kicad_mod_parser.cpp \
           src/kicad_mod_processor.cpp \
           src/Kicad_Sym_FileHandler.cpp \
           src/kicad_sym_parser.cpp \
           src/kicad_sym_processor.cpp \
           src/recompress_library.cpp \
           src/display_info.cpp \
           src/argument_handler.cpp

HEADERS += include/config.h \
           include/file_manager.h \
           include/file_utils.h \
           include/json_loader.h \
           include/unzipper.h \
           include/Kicad_Mod_FileHandler.h \
           include/kicad_mod_parser.h \
           include/kicad_mod_processor.h \
           include/Kicad_Sym_FileHandler.h \
           include/kicad_sym_parser.h \
           include/kicad_sym_processor.h \
           include/recompress_library.h \
           include/display_info.h \
           include/argument_handler.h

# Removed -lnlohmann_json
LIBS += -lminizip

SUBDIRS += s_expr_parser_mod \
           s_expr_parser_symbol \
           s_expr_merge_symbol

LIBS += -L$$OUT_PWD/s_expr_parser_mod -ls_expr_parser_mod
LIBS += -L$$OUT_PWD/s_expr_parser_symbol -ls_expr_parser_symbol
LIBS += -L$$OUT_PWD/s_expr_merge_symbol -ls_expr_merge_symbol

target.path = /usr/local/bin
INSTALLS += target
