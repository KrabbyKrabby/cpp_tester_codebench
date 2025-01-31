#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

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
        for (XMLNode* existingChild : children) {
            if (existingChild->name == child->name && existingChild->value == child->value) {
                throw std::runtime_error("Duplicate child node");
            }
        }
        children.push_back(child);
    }

    static bool isValidName(const std::string& name) {
        if (name.empty()) {
            return false;
        }

        char firstChar = name[0];
        if (!(firstChar >= 'a' && firstChar <= 'z') && !(firstChar >= 'A' && firstChar <= 'Z') && firstChar != '_') {
            return false;
        }

        for (char c : name) {
            if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && c != '_' && c != '.' && c != '-') {
                return false;
            }
        }

        return true;
    }
};

class XMLValidator {
public:
    static bool isValid(XMLNode* node) {
        if (!XMLNode::isValidName(node->name)) {
            return false;
        }

        for (XMLNode* child : node->children) {
            if (!isValid(child)) {
                return false;
            }
        }

        return true;
    }
};