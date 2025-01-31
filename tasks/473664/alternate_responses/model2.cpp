#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

class XMLNode {
private:
    static bool isValidNodeName(const std::string& name) {
        if (name.empty()) {
            return false;
        }

        // Check first character
        if (!std::isalpha(name[0]) && name[0] != '_') {
            return false;
        }

        // Check remaining characters
        return std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalnum(c) || c == '_' || c == '.' || c == '-';
        });
    }

public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {
        if (!isValidNodeName(name)) {
            throw std::invalid_argument("Invalid XML node name");
        }
    }

    ~XMLNode() {
        for (XMLNode* child : children) {
            delete child;
        }
    }

    void addChild(XMLNode* child) {
        if (!child) {
            throw std::invalid_argument("Cannot add null child node");
        }

        // Check for duplicate child
        for (const XMLNode* existingChild : children) {
            if (existingChild->name == child->name && 
                existingChild->value == child->value) {
                throw std::runtime_error("Duplicate child node");
            }
        }

        children.push_back(child);
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        if (!node) {
            return false;
        }

        try {
            // Validate current node's name
            if (!XMLNode::isValidNodeName(node->name)) {
                return false;
            }

            // Validate all children recursively
            for (XMLNode* child : node->children) {
                if (!isValid(child)) {
                    return false;
                }
            }

            return true;
        }
        catch (...) {
            return false;
        }
    }
};