// s_expr_parser_symbol.cpp
#include "s_expr_parser_symbol.h"
#include <stdexcept>

// Parse an S-expression string into a tree
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parse(const std::string& input) {
    size_t pos = 0;
    skipWhitespace(input, pos);

    if (pos < input.size() && input[pos] == '(') {
        return parseList(input, pos);
    } else if (pos < input.size()) {
        return parseAtom(input, pos);
    }

    throw std::runtime_error("Unexpected end of input while parsing S-expression.");
}

// Helper function to skip whitespace
void SeExprParserSymbol::skipWhitespace(const std::string& input, size_t& pos) {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos]))) {
        ++pos;
    }
}

// Helper function to parse an Atom
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parseAtom(const std::string& input, size_t& pos) {
    size_t start = pos;
    while (pos < input.size() && !std::isspace(static_cast<unsigned char>(input[pos])) && input[pos] != '(' && input[pos] != ')') {
        ++pos;
    }
    return std::make_shared<SeExprNodeSymbol>(input.substr(start, pos - start));
}

// Helper function to parse a List
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parseList(const std::string& input, size_t& pos) {
    if (pos >= input.size() || input[pos] != '(') {
        throw std::runtime_error("Expected '(' at position " + std::to_string(pos));
    }
    ++pos; // Skip '('

    auto listNode = std::make_shared<SeExprNodeSymbol>();
    while (pos < input.size() && input[pos] != ')') {
        skipWhitespace(input, pos);
        if (pos < input.size() && input[pos] == '(') {
            listNode->addChild(parseList(input, pos));
        } else if (pos < input.size()) {
            listNode->addChild(parseAtom(input, pos));
        }
        skipWhitespace(input, pos);
    }

    if (pos >= input.size() || input[pos] != ')') {
        throw std::runtime_error("Expected ')' at position " + std::to_string(pos));
    }
    ++pos; // Skip ')'

    return listNode;
}
