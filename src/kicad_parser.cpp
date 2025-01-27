#include "kicad_parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

KicadParser::KicadParser(const std::string &filePath) : filePath(filePath) {}

void KicadParser::parse() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto it = content.begin();
    auto end = content.end();

    qi::rule<std::string::iterator, std::string(), qi::space_type> s_expression =
        '(' >> *(qi::char_ - ')') >> ')';

    std::vector<std::string> parsed_expressions;
    bool success = qi::phrase_parse(it, end, *s_expression, qi::space, parsed_expressions);

    if (success && it == end) {
        std::cout << "Parsed S-expressions:" << std::endl;
        for (const auto &expr : parsed_expressions) {
            std::cout << expr << std::endl;
        }
        s_expressions = parsed_expressions;
    } else {
        std::cerr << "Failed to parse S-expressions" << std::endl;
    }
}
