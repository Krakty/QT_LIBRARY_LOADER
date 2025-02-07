//s_expr_node_mod.cpp
#include "s_expr_node_mod.h"
#include <sstream>

// Constructor for Atom nodes
SExprNodeMod::SExprNodeMod(const std::string& value) : type(NodeType::Atom), value(value) {}

// Constructor for List nodes
SExprNodeMod::SExprNodeMod() : type(NodeType::List) {}

// Add a child to the list node
void SExprNodeMod::addChild(std::shared_ptr<SExprNodeMod> child) {
    children.push_back(child);
}

// Serialize the S-expression tree to a string
std::string SExprNodeMod::toString() const {
    if (type == NodeType::Atom) {
        return value; // Return the value of the Atom node
    } else {
        std::ostringstream oss;
        oss << "(";
        for (size_t i = 0; i < children.size(); ++i) {
            oss << children[i]->toString(); // Serialize each child
            if (i < children.size() - 1) oss << " "; // Add space between children
        }
        oss << ")";
        return oss.str();
    }
}

// Find the first occurrence of a key in the S-expression tree
std::shared_ptr<SExprNodeMod> SExprNodeMod::findKey(const std::string& key) {
    if (type == NodeType::List && !children.empty()) {
        if (children[0]->type == NodeType::Atom && children[0]->value == key) {
            if (auto ptr = shared_from_this(); ptr) {
                return ptr;
            }
        }
        for (const auto& child : children) {
            auto result = child->findKey(key);
            if (result) return result;
        }
    }
    return nullptr; // Key not found
}

// Find a specific property node by name
std::shared_ptr<SExprNodeMod> SExprNodeMod::findPropertyByName(const std::string& propertyName) {
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
