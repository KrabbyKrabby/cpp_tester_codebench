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
        // BUG: Allows adding the same child multiple times
        children.push_back(child);
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        // BUG: Doesn't validate node name properly (e.g., empty name is allowed)
        if (node->name.empty()) {
            return true; // Incorrect logic
        }

        for (XMLNode* child : node->children) {
            if (!isValid(child)) {
                return false;
            }
        }

        // BUG: Doesn't check for other XML rules like invalid characters in names
        return true;
    }
};