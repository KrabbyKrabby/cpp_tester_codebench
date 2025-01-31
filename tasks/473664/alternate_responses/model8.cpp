#include <iostream>
#include <string>
#include <vector>

// Define a helper function to validate node names
bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    // Check if the name starts with a letter or an underscore
    if (!(isalpha(name[0]) || name[0] == '_')) {
        return false;
    }

    // Check each character in the name
    for (char c : name) {
        if (!(isalnum(c) || c == '_' || c == '.' || c == '-')) {
            return false;
        }
    }

    return true;
}

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {
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

    // Destructor to clean up child nodes
    ~XMLNode() {
        for (XMLNode* child : children) {
            delete child;
        }
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        if (node == nullptr) {
            return false;
        }

        // Validate child nodes recursively
        for (XMLNode* child : node->children) {
            if (!isValid(child)) {
                return false;
            }
        }

        return true;
    }
};

