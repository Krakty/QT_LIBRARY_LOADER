#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    static void searchFiles(const std::string &directory, const std::vector<std::string> &extensions, std::vector<std::string> &foundFiles);
    static void copyFiles(const std::vector<std::string> &files, const std::string &destination);
    static void renameFiles(const std::string &directory, const std::string &newBaseName);
    static std::string createTempDir(const std::string& root, const std::string& baseName, const std::string& suffix);
    static void clearTempDir(const std::string& root);
    static void copyKicadModFiles();      // Copies .kicad_mod files to the footprint library
    static void copy3DModelFiles();       // Copies the best 3D model file to the 3D model library
    static void copyMergedSymbolFile();   // Copies the merged .kicad_sym back to the library
    static void copyMasterSymbolFile();   // Copies the master .kicad_sym to the working directory
    static void BackupMasterKicadSym();   // Backs up the master .kicad_sym file
};

#endif // FILE_MANAGER_H
