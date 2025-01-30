#include "s_expr_node_symbol.h"
#include <sstream>

// Constructor for Atom nodes
SeExprNodeSymbol::SeExprNodeSymbol(const std::string& value) : type(NodeType::Atom), value(value) {}

// Constructor for List nodes
SeExprNodeSymbol::SeExprNodeSymbol() : type(NodeType::List) {}

// Add a child to the List node
void SeExprNodeSymbol::addChild(std::shared_ptr<SeExprNodeSymbol> child) {
    children.push_back(child);
}

// Serialize the S-expression tree to a single-line string
std::string SeExprNodeSymbol::toString(int indentLevel) const {
    if (type == NodeType::Atom) {
        return value; // Return the value for Atom nodes
    } else {
        std::ostringstream oss;
        oss << "(";
        for (size_t i = 0; i < children.size(); ++i) {
            oss << children[i]->toString(0); // Always serialize without indentation
            if (i < children.size() - 1) {
                oss << " "; // Add space between child nodes
            }
        }
        oss << ")";
        return oss.str();
    }
}

// Find the first occurrence of a key in the S-expression tree
std::shared_ptr<SeExprNodeSymbol> SeExprNodeSymbol::findKey(const std::string& key) {
    if (type == NodeType::List && !children.empty()) {
        // Check if the first child is an Atom with the matching key
        if (children[0]->type == NodeType::Atom && children[0]->value == key) {
            return shared_from_this(); // Return the entire List node
        }

        // Recursively search for the key in child nodes
        for (const auto& child : children) {
            auto result = child->findKey(key);
            if (result) {
                return result;
            }
        }
    }

    return nullptr; // Key not found
}

// Find a specific property node by name
std::shared_ptr<SeExprNodeSymbol> SeExprNodeSymbol::findPropertyByName(const std::string& propertyName) {
    if (type == NodeType::List && !children.empty()) {
        if (children[0]->type == NodeType::Atom && children[0]->value == "property") {
            if (children.size() > 1 &&
                children[1]->type == NodeType::Atom &&
                children[1]->value == "\"" + propertyName + "\"") {
                if (auto ptr = shared_from_this(); ptr) {
                    return ptr;
                }
            }
        }
        for (const auto& child : children) {
            auto result = child->findPropertyByName(propertyName);
            if (result) return result;
        }
    }
    return nullptr; // Property not found
}
