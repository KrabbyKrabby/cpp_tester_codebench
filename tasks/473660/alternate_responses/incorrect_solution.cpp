#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;
    std::unordered_map<std::string, std::string> attributes;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(XMLNode* child) {
        children.push_back(child);
    }

    void addAttribute(const std::string& key, const std::string& value) {
        attributes[key] = value;
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        std::string json = "{";
        if (node->value.empty()) {
            json += "\"" + node->name + "\": {";
        } else {
            json += "\"" + node->name + "\": \"" + node->value + "\", ";
        }

        for (auto& attribute : node->attributes) {
            json += "\"" + attribute.first + "\": \"" + attribute.second + "\", ";
        }

        for (auto child : node->children) {
            json += convert(child) + ", ";
        }

        json += "}";
        size_t lastComma = json.find_last_of(',');
        if (lastComma != std::string::npos) {
            json.erase(lastComma, 2);
        }

        return json;
    }
};