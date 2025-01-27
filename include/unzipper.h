#ifndef UNZIPPER_H
#define UNZIPPER_H

#include <string>

class Unzipper {
public:
    Unzipper(const std::string &zipFilePath, const std::string &destinationFolder);
    void unzip();
    static void extractToDir(const std::string &zipFilePath, const std::string &destinationFolder);

private:
    std::string zipFilePath;
    std::string destinationFolder;
};

#endif // UNZIPPER_H
