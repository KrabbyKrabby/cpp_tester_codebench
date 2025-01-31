#include <iostream>
#include <string>
#include <vector>

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
        if (hasChild(child)) {
            throw std::runtime_error("Duplicate child node");
        }
        children.push_back(child);
    }

    bool hasChild(XMLNode* child) {
        for (XMLNode* existingChild : children) {
            if (existingChild->name == child->name && existingChild->value == child->value) {
                return true;
            }
        }
        return false;
    }

    static bool isValidName(const std::string& name) {
        if (name.empty()) {
            return false;
        }
        if (!isalnum(name[0]) && name[0] != '_') {
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
        if (node == nullptr) {
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