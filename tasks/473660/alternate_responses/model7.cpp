#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>

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

    void addAttribute(const std::string& name, const std::string& value) {
        attributes[name] = value;
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        if (!node) {
            return "{}"; // Return an empty JSON object for null pointers
        }

        std::stringstream json;
        convertNode(node, json, 0);
        return json.str();
    }

private:
    static void convertNode(XMLNode* node, std::stringstream& json, int indentLevel) {
        if (!node) {
            return; // Handle null pointers
        }

        // Handle node attributes
        if (!node->attributes.empty()) {
            json << std::string(indentLevel * 2, ' ') << "\"" << node->name << "\": {" << std::endl;
            for (const auto& attribute : node->attributes) {
                json << std::string((indentLevel + 1) * 2, ' ') << "\"" << attribute.first << "\": \"" << attribute.second << "\"," << std::endl;
            }
            json << std::string(indentLevel * 2, ' ') << "}," << std::endl;
        } else {
            json << std::string(indentLevel * 2, ' ') << "\"" << node->name << "\": ";
        }

        // Handle node value
        if (!node->value.empty()) {
            json << "\"" << node->value << "\"";
        }

        // Handle child nodes
        if (!node->children.empty()) {
            json << "[" << std::endl;
            for (const auto& child : node->children) {
                convertNode(child.get(), json, indentLevel + 1);
                json << "," << std::endl;
            }
            json.seekp(-2, std::ios_base::end); // Remove the trailing comma and newline
            json << std::endl << std::string(indentLevel * 2, ' ') << "]";
        }

        json << "," << std::endl;
    }
};