#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "config.h"
#include <string>
#include <nlohmann/json.hpp>

class JSONLoader {
public:
    JSONLoader(const std::string &filePath);
    void loadConfig();

private:
    std::string filePath;
};

#endif // JSON_LOADER_H
