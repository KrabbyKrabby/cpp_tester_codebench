#include <iostream>
#include <string>
#include <vector>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {
        // Validate node name
        if (!isValidName(name)) {
            throw std::invalid_argument("Invalid node name");
        }
    }

    void addChild(XMLNode* child) {
        // Check for duplicate child nodes
        for (XMLNode* existingChild : children) {
            if (existingChild->name == child->name && existingChild->value == child->value) {
                throw std::runtime_error("Duplicate child node");
            }
        }
        children.push_back(child);
    }

private:
    bool isValidName(const std::string& name) {
        if (name.empty()) {
            return false;
        }
        if (!isalpha(name[0]) && name[0] != '_') {
            return false;
        }
        for (char c : name) {
            if (!isalnum(c) && c != '_' && c != '.' && c != '-') {
                return false;
            }
        }
        return true;
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        // Validate node name
        if (!node->isValidName(node->name)) {
            return false;
        }

        // Recursively validate child nodes
        for (XMLNode* child : node->children) {
            if (!isValid(child)) {
                return false;
            }
        }

        return true;
    }
};
