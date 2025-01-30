#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

// Function to get files with a specific extension in a directory
std::vector<std::string> getFilesWithExtension(const std::string& directory, const std::string& extension);


#endif // FILE_UTILS_H
