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
};

#endif // FILE_MANAGER_H
