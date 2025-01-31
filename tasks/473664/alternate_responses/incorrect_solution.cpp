#include <iostream>
#include <string>
#include <vector>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(XMLNode* child) {
        // Check for duplicate child nodes
        for (XMLNode* existingChild : children) {
            if (existingChild->name == child->name && existingChild->value == child->value) {
                throw std::runtime_error("Duplicate child node");
            }
        }
        children.push_back(child);
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        if (node->name.empty()) {
            return true;
        }

        for (XMLNode* child : node->children) {
            if (!isValid(child)) {
                return false;
            }
        }

        return true;
    }
};
