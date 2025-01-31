#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <json/json.h> // Requires jsoncpp library

class XMLNode {
public:
    std::string name;
    std::string value;
    std::unordered_map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<XMLNode>> children;

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
            return "null"; // or throw an exception
        }

        Json::Value jsonNode;

        // Process attributes
        for (const auto& attribute : node->attributes) {
            jsonNode[attribute.first] = attribute.second;
        }

        // Process children
        for (const auto& child : node->children) {
            if (!child) {
                continue; // Skip null children
            }

            Json::Value childJson = Json::Value(Json::objectValue);
            childJson = Json::Value(convert(child.get()));

            if (jsonNode.isMember(child->name)) {
                // Handle duplicate child nodes by converting the existing value to an array
                if (!jsonNode[child->name].isArray()) {
                    Json::Value array = Json::Value(Json::arrayValue);
                    array.append(jsonNode[child->name]);
                    jsonNode[child->name] = array;
                }
                jsonNode[child->name].append(childJson);
            } else {
                jsonNode[child->name] = childJson;
            }
        }

        // Process node value
        if (!node->value.empty()) {
            if (jsonNode.isObject() && !jsonNode.empty()) {
                // Handle mixed content (text and children) by adding a "_text" property
                jsonNode["_text"] = node->value;
            } else {
                jsonNode = node->value;
            }
        }

        Json::FastWriter writer;
        return writer.write(jsonNode);
    }
};