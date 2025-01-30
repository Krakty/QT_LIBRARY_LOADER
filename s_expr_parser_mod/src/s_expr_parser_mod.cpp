// s_expr_parser_mod.cpp
#include "s_expr_parser_mod.h"
#include "s_expr_node_mod.h"
#include <stdexcept>

// Parse an S-expression from a string
std::shared_ptr<SExprNodeMod> SExprParserMod::parse(const std::string& input) {
    std::istringstream stream(input);
    return parseStream(stream);
}

std::shared_ptr<SExprNodeMod> SExprParserMod::parseStream(std::istringstream& stream) {
    skipWhitespace(stream);

    char c = stream.peek();
    if (c == '(') {
        // Parse a list node
        stream.get(); // Consume '('
        auto node = std::make_shared<SExprNodeMod>(); // Create a new list node
        while (stream.peek() != ')' && stream.good() && stream.peek() != EOF) {
            auto child = parseStream(stream); // Recursively parse each child
            node->addChild(child); // Add the child to the list node
            skipWhitespace(stream);
        }
        if (stream.get() != ')') {
            throw std::runtime_error("Unmatched parentheses");
        }
        return node;
    } else if (c == ')') {
        throw std::runtime_error("Unexpected ')'");
    } else {
        // Parse an atom node
        std::string atom;
        while (!std::isspace(c) && c != '(' && c != ')' && stream.good() && stream.peek() != EOF) {
            atom += stream.get();
            c = stream.peek();
        }
        return std::make_shared<SExprNodeMod>(atom);
    }
}

// Helper function to skip whitespace in the stream
void SExprParserMod::skipWhitespace(std::istringstream& stream) {
    while (stream.good() && stream.peek() != EOF && std::isspace(stream.peek())) stream.get();
}
