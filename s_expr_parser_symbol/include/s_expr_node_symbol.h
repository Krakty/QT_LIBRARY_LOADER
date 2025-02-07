#ifndef S_EXPR_NODE_SYMBOL_H
#define S_EXPR_NODE_SYMBOL_H

#include <memory>
#include <string>
#include <vector>

/**
 * Represents a node in an S-expression tree for `.kicad_sym` files.
 * A node can be either an Atom (leaf node) or a List (containing child nodes).
 */
class SeExprNodeSymbol : public std::enable_shared_from_this<SeExprNodeSymbol> {
public:
    enum class NodeType { Atom, List };  // Defines node types

    NodeType type;                        // Type of the node (Atom or List)
    std::string value;                    // Value of the Atom (empty for List nodes)
    std::vector<std::shared_ptr<SeExprNodeSymbol>> children;  // Child nodes for List type

    // Constructors
    SeExprNodeSymbol();                            // Constructor for List nodes
    explicit SeExprNodeSymbol(const std::string& value); // Constructor for Atom nodes

    // Core Functions
    void addChild(std::shared_ptr<SeExprNodeSymbol> child); // Adds a child node
    std::string toString(int indentLevel = 0) const;        // Converts the node to a string (S-expression format)

    // Property Management
    std::shared_ptr<SeExprNodeSymbol> findKey(const std::string& key);                      // Finds the first occurrence of a key
    std::shared_ptr<SeExprNodeSymbol> findPropertyByName(const std::string& propertyName);  // Finds a property node by name
    void clearPropertyValues(const std::string& propertyName);                              // Clears all values from a property
    void addPropertyValues(const std::string& propertyName, const std::vector<std::string>& values); // Adds values to a property

    // Debugging and Analysis
    void printAllProperties(const std::string& parentSymbol = "");
    //void printAllProperties();               // Recursively prints all properties within the tree
    bool allChildrenAreAtoms() const;        // Returns true if all children are Atom nodes
};

#endif // S_EXPR_NODE_SYMBOL_H
