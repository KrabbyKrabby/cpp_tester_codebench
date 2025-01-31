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
        std::string json;

        // No null pointer check
        json += "{\"" + node->name + "\": ";

        // No handling for empty nodes
        if (node->value.empty() && node->children.empty()) {
            json += "\"\"";
        }
        else if (!node->value.empty()) {
            // No proper JSON formatting
            json += "\"" + node->value + "\"";
        }
        else {
            json += "[";
            for (auto child : node->children) {
                json += convert(child) + ",";
            }
            // No handling for trailing comma
            json += "]";
        }

        json += "}";

        return json;
    }
};