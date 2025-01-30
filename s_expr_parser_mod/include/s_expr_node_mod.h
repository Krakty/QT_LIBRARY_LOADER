#ifndef S_EXPR_NODE_MOD_H
#define S_EXPR_NODE_MOD_H

#include <string>
#include <vector>
#include <memory>

class SExprNodeMod : public std::enable_shared_from_this<SExprNodeMod> {
public:
    // Node type: Atom (leaf) or List (tree structure)
    enum class NodeType { Atom, List };

    // Constructor for Atom nodes (value is a string)
    explicit SExprNodeMod(const std::string& value);

    // Constructor for List nodes (empty list)
    SExprNodeMod();

    // Add a child to the list node
    void addChild(std::shared_ptr<SExprNodeMod> child);

    // Serialize the S-expression tree to a string
    std::string toString() const;

    // Find a key in the tree (recursive search)
    std::shared_ptr<SExprNodeMod> findKey(const std::string& key);

    // Find a property node by name
    std::shared_ptr<SExprNodeMod> findPropertyByName(const std::string& propertyName);

    // Public members
    NodeType type; // Type of the node (Atom or List)
    std::string value; // Value of the Atom node
    std::vector<std::shared_ptr<SExprNodeMod>> children; // Children of the List node
};

#endif // S_EXPR_NODE_MOD_H
