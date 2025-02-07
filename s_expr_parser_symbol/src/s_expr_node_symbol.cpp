#include "s_expr_node_symbol.h"
#include <sstream>
#include <iostream>

/**
 * Constructor for Atom nodes.
 */
SeExprNodeSymbol::SeExprNodeSymbol(const std::string& value)
    : type(NodeType::Atom), value(value) {}

/**
 * Constructor for List nodes.
 */
SeExprNodeSymbol::SeExprNodeSymbol()
    : type(NodeType::List) {}

/**
 * Add a child to the List node.
 */
void SeExprNodeSymbol::addChild(std::shared_ptr<SeExprNodeSymbol> child) {
    children.push_back(child);
}

/**
 * Returns true if this node is a List and *all* children are Atom nodes.
 */
bool SeExprNodeSymbol::allChildrenAreAtoms() const {
    if (type != NodeType::List) return true; // Treat non-lists as valid
    for (const auto& child : children) {
        if (child->type == NodeType::List) {
            return false; // Nested lists found
        }
    }
    return true; // All children are atoms
}

/**
 * Convert the node to a string representation (S-expression format).
 */
std::string SeExprNodeSymbol::toString(int indentLevel) const {
    if (type == NodeType::Atom) {
        return value;
    }

    bool onlyAtoms = allChildrenAreAtoms();
    std::ostringstream oss;
    oss << "(";

    if (onlyAtoms) {
        for (size_t i = 0; i < children.size(); ++i) {
            if (i > 0) {
                oss << " ";
            }
            oss << children[i]->toString(0);
        }
        oss << ")";
    } else {
        for (size_t i = 0; i < children.size(); ++i) {
            if (i == 0) {
                oss << children[i]->toString(indentLevel + 2);
            } else {
                oss << "\n" << std::string(indentLevel + 2, ' ');
                oss << children[i]->toString(indentLevel + 2);
            }
        }
        oss << ")";
    }

    return oss.str();
}

/**
 * Find the first occurrence of a key in the S-expression tree.
 */
std::shared_ptr<SeExprNodeSymbol> SeExprNodeSymbol::findKey(const std::string& key) {
    if (type == NodeType::List && !children.empty()) {
        if (children[0]->type == NodeType::Atom && children[0]->value == key) {
            return shared_from_this();
        }
        for (const auto& child : children) {
            auto result = child->findKey(key);
            if (result) {
                return result;
            }
        }
    }
    return nullptr;
}

/**
 * Find a property node by its name (e.g., "Footprint").
 */
std::shared_ptr<SeExprNodeSymbol> SeExprNodeSymbol::findPropertyByName(const std::string& propertyName) {
    if (type == NodeType::List && !children.empty()) {
        if (children[0]->type == NodeType::Atom && children[0]->value == "property") {
            if (children.size() > 1 && children[1]->type == NodeType::Atom &&
                children[1]->value == "\"" + propertyName + "\"") {
                return shared_from_this();
            }
        }
        for (const auto& child : children) {
            auto result = child->findPropertyByName(propertyName);
            if (result) return result;
        }
    }
    return nullptr;
}

/**
 * Recursively print all properties within the S-expression tree,
 * along with the parent symbol name for better context.
 */
void SeExprNodeSymbol::printAllProperties(const std::string& parentSymbol) {
    if (type == NodeType::List && !children.empty()) {
        // Identify and print the symbol name
        if (children[0]->type == NodeType::Atom && children[0]->value == "symbol" && children.size() > 1) {
            std::cout << "Symbol: " << children[1]->value << std::endl;
        }

        // Identify and print property information
        if (children[0]->type == NodeType::Atom && children[0]->value == "property") {
            std::cout << "  Found property: " << children[1]->value << " -> ";
            for (size_t i = 2; i < children.size(); ++i) {
                std::cout << children[i]->value << " ";
            }
            std::cout << std::endl;
        }

        // Recursively process child nodes
        for (const auto& child : children) {
            child->printAllProperties(parentSymbol);
        }
    }
}


/**
 * Clear all values from a specified property.
 */
void SeExprNodeSymbol::clearPropertyValues(const std::string& propertyName) {
    auto propertyNode = findPropertyByName(propertyName);
    if (propertyNode && propertyNode->children.size() > 2) {
        std::cout << "[DEBUG] Clearing all values from property: " << propertyName << "\n";
        propertyNode->children.erase(propertyNode->children.begin() + 2, propertyNode->children.end());
    } else {
        std::cerr << "[WARNING] Unable to clear values for property: " << propertyName << "\n";
    }
}

/**
 * Add new values to a property. Creates the property if it doesn't exist.
 */
void SeExprNodeSymbol::addPropertyValues(const std::string& propertyName, const std::vector<std::string>& values) {
    auto propertyNode = findPropertyByName(propertyName);

    if (!propertyNode) {
        std::cout << "[DEBUG] Property '" << propertyName << "' not found. Creating it.\n";
        auto newProperty = std::make_shared<SeExprNodeSymbol>();
        newProperty->addChild(std::make_shared<SeExprNodeSymbol>("property"));
        newProperty->addChild(std::make_shared<SeExprNodeSymbol>("\"" + propertyName + "\""));
        for (const auto& val : values) {
            newProperty->addChild(std::make_shared<SeExprNodeSymbol>("\"" + val + "\""));
        }
        addChild(newProperty);
    } else {
        clearPropertyValues(propertyName);  // Clear existing values first
        std::cout << "[DEBUG] Adding new values to property: " << propertyName << "\n";
        for (const auto& val : values) {
            propertyNode->addChild(std::make_shared<SeExprNodeSymbol>("\"" + val + "\""));
        }
    }
}
