#ifndef KICAD_PARSER_H
#define KICAD_PARSER_H

#include <string>
#include <vector>

class KicadParser {
public:
    KicadParser(const std::string &filePath);
    void parse();

private:
    std::string filePath;
    std::vector<std::string> s_expressions;
};

#endif // KICAD_PARSER_H
