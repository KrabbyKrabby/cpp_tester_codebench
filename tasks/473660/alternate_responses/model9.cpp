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
        if (!node) {
            return "{}"; // Return empty JSON object if node is null
        }

        std::string json = "{\"" + node->name + "\": ";
        if (!node->value.empty()) {
            json += "\"" + node->value + "\"";
        } else {
            json += "{}"; // Empty XML node becomes an empty JSON object
        }

        // Process attributes
        if (!node->attributes.empty()) {
            json += ", \"@attributes\": {";
            for (const auto& attribute : node->attributes) {
                json += "\"" + attribute.first + "\": \"" + attribute.second + "\", ";
            }
            json.pop_back(); // Remove trailing comma
            json.pop_back(); // Remove trailing space
            json += "}";
        }

        // Process child nodes
        if (!node->children.empty()) {
            json += ", \"children\": [";
            for (const auto& child : node->children) {
                json += convert(child.get()) + ", ";
            }
            json.pop_back(); // Remove trailing comma
            json.pop_back(); // Remove trailing space
            json += "]";
        }

        json += "}";
        return json;
    }
};
