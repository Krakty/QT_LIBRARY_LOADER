#ifndef S_EXPR_PARSER_SYMBOL_H
#define S_EXPR_PARSER_SYMBOL_H

#include <memory>
#include <string>
#include "s_expr_node_symbol.h"

/**
 * A parser for S-expression-based `.kicad_sym` files.
 */
class SeExprParserSymbol {
public:
    /**
     * Parses an entire S-expression string and returns the root node of the tree.
     *
     * @param input The string containing the S-expression data.
     * @return A shared pointer to the root of the parsed S-expression tree.
     * @throws std::runtime_error if parsing fails due to syntax errors.
     */
    static std::shared_ptr<SeExprNodeSymbol> parse(const std::string& input);

private:
    /**
     * Skips whitespace characters in the input string starting from the given position.
     *
     * @param input The string to parse.
     * @param pos   The current parsing position, which will be updated.
     */
    static void skipWhitespace(const std::string& input, size_t& pos);

    /**
     * Parses an atom node (either a quoted string or an unquoted token).
     *
     * @param input The string to parse.
     * @param pos   The current parsing position, which will be updated.
     * @return A shared pointer to the parsed atom node.
     * @throws std::runtime_error if the atom is invalid or malformed.
     */
    static std::shared_ptr<SeExprNodeSymbol> parseAtom(const std::string& input, size_t& pos);

    /**
     * Parses a list node starting at the current position, assuming '(' has been encountered.
     *
     * @param input The string to parse.
     * @param pos   The current parsing position, which will be updated.
     * @return A shared pointer to the parsed list node.
     * @throws std::runtime_error if the list is unmatched or malformed.
     */
    static std::shared_ptr<SeExprNodeSymbol> parseList(const std::string& input, size_t& pos);
};

#endif // S_EXPR_PARSER_SYMBOL_H
