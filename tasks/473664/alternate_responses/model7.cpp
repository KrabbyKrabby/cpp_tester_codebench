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
        validateNodeName(name);
    }

    void addChild(XMLNode* child) {
        if (hasChild(child)) {
            throw std::runtime_error("Duplicate child node");
        }
        children.push_back(child);
    }

    bool hasChild(XMLNode* child) {
        for (XMLNode* c : children) {
            if (c->name == child->name && c->value == child->value) {
                return true;
            }
        }
        return false;
    }

    ~XMLNode() {
        for (XMLNode* child : children) {
            delete child;
        }
    }

private:
    static void validateNodeName(const std::string& name) {
        if (name.empty()) {
            throw std::invalid_argument("Node name cannot be empty");
        }

        if (!isValidNameChar(name[0])) {
            throw std::invalid_argument("Node name must start with a letter or underscore");
        }

        for (char c : name) {
            if (!isValidNameChar(c)) {
                throw std::invalid_argument("Node name contains invalid characters");
            }
        }
    }

    static bool isValidNameChar(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '-';
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

int main() {
    try {
        XMLNode* root = new XMLNode("root");
        XMLNode* child1 = new XMLNode("child1", "value1");
        XMLNode* child2 = new XMLNode("child2");

        root->addChild(child1);
        root->addChild(child2);

        if (XMLValidator::isValid(root)) {
            std::cout << "XML is valid" << std::endl;
        } else {
            std::cout << "XML is invalid" << std::endl;
        }

        delete root;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}