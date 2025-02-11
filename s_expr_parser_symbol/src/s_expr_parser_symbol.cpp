#include "s_expr_parser_symbol.h"
#include <stdexcept>
//#include <iostream>

/**
 * Parse an entire string into an S-expression tree, returning the root node.
 * If there's leftover text after the first parsed expression, that might or might
 * not be valid for your usage. Typically you'd parse the entire file as one top-level list.
 */
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parse(const std::string& input) {
    size_t pos = 0;
    skipWhitespace(input, pos);

    if (pos < input.size() && input[pos] == '(') {
        return parseList(input, pos);  // Parse a list if '(' is encountered
    } else if (pos < input.size()) {
        return parseAtom(input, pos);  // Otherwise, parse an atom
    }

    throw std::runtime_error("Unexpected end of input while parsing S-expression.");
}

/**
 * Skip whitespace in 'input' starting at 'pos'.
 * Whitespace includes space, tab, newline, etc.
 */
void SeExprParserSymbol::skipWhitespace(const std::string& input, size_t& pos) {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos]))) {
        ++pos;
    }
}

/**
 * Parse an Atom node. This can be:
 *  - A quoted string (possibly multiline).
 *  - An unquoted token (stops at whitespace, '(' or ')').
 */
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parseAtom(const std::string& input, size_t& pos)
{
    skipWhitespace(input, pos);

    if (pos < input.size() && input[pos] == '"') {
        size_t startPos = pos;
        ++pos;  // Skip the opening quote

        std::string buffer;
        bool foundClosingQuote = false;

        while (pos < input.size()) {
            char c = input[pos++];
            if (c == '"') {
                foundClosingQuote = true;
                break;
            } else if (c == '\n' || c == '\r') {
                buffer.push_back(' ');  // Convert newlines to spaces
            } else {
                buffer.push_back(c);
            }
        }

        if (!foundClosingQuote) {
            throw std::runtime_error("Unclosed quoted string starting at position " + std::to_string(startPos));
        }

        return std::make_shared<SeExprNodeSymbol>("\"" + buffer + "\"");
    }

    size_t start = pos;
    while (pos < input.size() &&
           !std::isspace(static_cast<unsigned char>(input[pos])) &&
           input[pos] != '(' && input[pos] != ')') {
        ++pos;
    }

    if (start == pos) {
        throw std::runtime_error("Empty atom encountered at position " + std::to_string(pos));
    }

    return std::make_shared<SeExprNodeSymbol>(input.substr(start, pos - start));
}

/**
 * Parse a List node: expects '(' at input[pos].
 * We'll read child expressions until the matching ')'.
 */
std::shared_ptr<SeExprNodeSymbol> SeExprParserSymbol::parseList(const std::string& input, size_t& pos) {
    if (pos >= input.size() || input[pos] != '(') {
        throw std::runtime_error("Expected '(' at position " + std::to_string(pos));
    }
    ++pos;  // Consume '('

    auto listNode = std::make_shared<SeExprNodeSymbol>();  // This is a List node

    while (pos < input.size()) {
        skipWhitespace(input, pos);

        if (pos >= input.size()) {
            throw std::runtime_error("Unmatched '(' - no closing ')' found.");
        }
        if (input[pos] == ')') {
            ++pos;  // Consume ')'
            return listNode;
        }

        if (input[pos] == '(') {
            listNode->addChild(parseList(input, pos));
        } else {
            listNode->addChild(parseAtom(input, pos));
        }
    }

    throw std::runtime_error("Unmatched '(' - reached end of input without ')' ");
}
