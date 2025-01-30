#ifndef S_EXPR_PARSER_MOD_H
#define S_EXPR_PARSER_MOD_H

#include "s_expr_node_mod.h"
#include <string>
#include <memory>
#include <sstream>

class SExprParserMod {
public:
    // Parse an S-expression from a string
    static std::shared_ptr<SExprNodeMod> parse(const std::string& input);

private:
    // Helper function to parse from a stream
    static std::shared_ptr<SExprNodeMod> parseStream(std::istringstream& stream);

    // Skip whitespace in the input stream
    static void skipWhitespace(std::istringstream& stream);
};

#endif // S_EXPR_PARSER_MOD_H
