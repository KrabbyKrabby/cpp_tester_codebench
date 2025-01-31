#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(XMLNode* child) {      
        children.push_back(child);
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        std::string json = "{\"" + node->name + "\" : ";
        if (!node->value.empty()) {
            json += "\"" + node->value + "\"";
        } else {
            json += "{";
            for (XMLNode* child : node->children) {
                json += "\"" + child->name + "\" : " + convert(child) + ",";
            }
            // Remove trailing comma
            if (json.back() == ',') {
                json.pop_back();
            }
            json += "}";
        }
        return json;
    }
};

int main() {
    XMLNode* root = new XMLNode("root");
    XMLNode* child1 = new XMLNode("child1", "value1");
    XMLNode* child2 = new XMLNode("child2");
    XMLNode* grandchild1 = new XMLNode("grandchild1", "grandvalue1");
    XMLNode* grandchild2 = new XMLNode("grandchild2", "grandvalue2");

    root->addChild(child1);
    root->addChild(child2);
    child2->addChild(grandchild1);
    child2->addChild(grandchild2);

    std::cout << XMLToJSONConverter::convert(root) << std::endl;

    delete root;
    delete child1;
    delete child2;
    delete grandchild1;
    delete grandchild2;

    return 0;
}