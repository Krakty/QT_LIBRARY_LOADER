// s_expr_parser_symbol.h
#ifndef S_EXPR_PARSER_SYMBOL_H
#define S_EXPR_PARSER_SYMBOL_H

#include "s_expr_node_symbol.h"
#include <memory>
#include <string>

class SeExprParserSymbol {
public:
    // Parse an S-expression string into a tree
    static std::shared_ptr<SeExprNodeSymbol> parse(const std::string& input);

private:
    // Helper function to skip whitespace
    static void skipWhitespace(const std::string& input, size_t& pos);

    // Helper function to parse an Atom
    static std::shared_ptr<SeExprNodeSymbol> parseAtom(const std::string& input, size_t& pos);

    // Helper function to parse a List
    static std::shared_ptr<SeExprNodeSymbol> parseList(const std::string& input, size_t& pos);
};

#endif // S_EXPR_PARSER_SYMBOL_H

