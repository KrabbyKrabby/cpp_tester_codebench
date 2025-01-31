#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<std::unique_ptr<XMLNode>> children;
    std::unordered_map<std::string, std::string> attributes;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(std::unique_ptr<XMLNode> child) {      
        children.push_back(std::move(child));
    }

    void addAttribute(const std::string& key, const std::string& value) {
        attributes[key] = value;
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        if (node == nullptr) {
            return "{}"; // Return empty JSON object for null pointer
        }

        std::string json = "{";

        // Process attributes
        for (const auto& attribute : node->attributes) {
            json += "\"" + attribute.first + "\":\"" + attribute.second + "\",";
        }

        // Process child nodes
        for (const auto& child : node->children) {
            json += "\"" + child->name + "\":" + convert(child.get()) + ",";
        }

        // Process node value
        if (!node->value.empty()) {
            json += "\"value\":\"" + node->value + "\",";
        }

        // Remove trailing comma if present
        if (json.back() == ',') {
            json.pop_back();
        }

        json += "}";

        return json;
    }
};