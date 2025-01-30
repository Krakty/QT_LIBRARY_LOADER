#ifndef S_EXPR_NODE_SYMBOL_H
#define S_EXPR_NODE_SYMBOL_H

#include <memory>
#include <string>
#include <vector>

class SeExprNodeSymbol : public std::enable_shared_from_this<SeExprNodeSymbol> {
public:
    // Enum to differentiate between Atom and List nodes
    enum class NodeType { Atom, List };

    // Constructors
    explicit SeExprNodeSymbol(const std::string& value); // For Atom nodes
    SeExprNodeSymbol();                                  // For List nodes

    // Add a child to the List node
    void addChild(std::shared_ptr<SeExprNodeSymbol> child);

    // Serialize the S-expression tree to a formatted string
    std::string toString(int indentLevel = 0) const;

    // Node type
    NodeType type;

    // For Atom nodes
    std::string value;

    // For List nodes
    std::vector<std::shared_ptr<SeExprNodeSymbol>> children;
    std::shared_ptr<SeExprNodeSymbol> findKey(const std::string& key);

    // Find a property node by name
    std::shared_ptr<SeExprNodeSymbol> findPropertyByName(const std::string& propertyName);
};

#endif // S_EXPR_NODE_SYMBOL_H
