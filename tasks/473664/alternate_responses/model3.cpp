#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

class XMLNode {
private:
    static bool isValidFirstChar(char c) {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    static bool isValidNameChar(char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || 
               c == '_' || c == '.' || c == '-';
    }

    static bool isValidNodeName(const std::string& name) {
        if (name.empty() || !isValidFirstChar(name[0])) {
            return false;
        }

        return std::all_of(name.begin() + 1, name.end(), 
                          [](char c) { return isValidNameChar(c); });
    }

public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") 
        : name(name), value(value) {
        if (!isValidNodeName(name)) {
            throw std::invalid_argument("Invalid XML node name");
        }
    }

    void addChild(XMLNode* child) {
        if (!child) {
            throw std::invalid_argument("Cannot add null child node");
        }

        // Check for duplicate child (same name and value)
        auto it = std::find_if(children.begin(), children.end(),
            [child](const XMLNode* existing) {
                return existing->name == child->name && 
                       existing->value == child->value;
            });

        if (it != children.end()) {
            throw std::runtime_error("Duplicate child node");
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
};
