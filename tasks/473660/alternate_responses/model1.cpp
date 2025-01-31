#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <stdexcept>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<std::shared_ptr<XMLNode>> children;
    std::unordered_map<std::string, std::string> attributes;

    XMLNode(const std::string& name, const std::string& value = "") 
        : name(name), value(value) {}

    void addChild(std::shared_ptr<XMLNode> child) {
        if (child) {
            children.push_back(child);
        }
    }

    void addAttribute(const std::string& key, const std::string& value) {
        attributes[key] = value;
    }
};

class XMLToJSONConverter {
private:
    static std::string escapeJSON(const std::string& str) {
        std::ostringstream result;
        for (char c : str) {
            switch (c) {
                case '\"': result << "\\\""; break;
                case '\\': result << "\\\\"; break;
                case '\b': result << "\\b"; break;
                case '\f': result << "\\f"; break;
                case '\n': result << "\\n"; break;
                case '\r': result << "\\r"; break;
                case '\t': result << "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        result << "\\u" << std::hex << std::setw(4) 
                               << std::setfill('0') << static_cast<int>(c);
                    } else {
                        result << c;
                    }
            }
        }
        return result.str();
    }

    static std::string convertNode(std::shared_ptr<XMLNode> node, 
                                 std::unordered_set<std::shared_ptr<XMLNode>>& visited,
                                 int indent = 0) {
        if (!node) {
            return "null";
        }

        // Check for circular references
        if (visited.find(node) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node);

        std::ostringstream json;
        json << "{";

        // Handle node name
        json << "\n" << std::string(indent + 2, ' ') 
             << "\"nodeName\": \"" << escapeJSON(node->name) << "\"";

        // Handle attributes
        if (!node->attributes.empty()) {
            json << ",\n" << std::string(indent + 2, ' ') << "\"attributes\": {";
            bool firstAttr = true;
            for (const auto& attr : node->attributes) {
                if (!firstAttr) json << ",";
                json << "\n" << std::string(indent + 4, ' ') 
                     << "\"" << escapeJSON(attr.first) << "\": \"" 
                     << escapeJSON(attr.second) << "\"";
                firstAttr = false;
            }
            json << "\n" << std::string(indent + 2, ' ') << "}";
        }

        // Handle value
        if (!node->value.empty()) {
            json << ",\n" << std::string(indent + 2, ' ') 
                 << "\"value\": \"" << escapeJSON(node->value) << "\"";
        }

        // Handle children
        if (!node->children.empty()) {
            json << ",\n" << std::string(indent + 2, ' ') << "\"children\": [";
            bool firstChild = true;
            for (const auto& child : node->children) {
                if (!firstChild) json << ",";
                json << "\n" << std::string(indent + 4, ' ') 
                     << convertNode(child, visited, indent + 4);
                firstChild = false;
            }
            json << "\n" << std::string(indent + 2, ' ') << "]";
        }

        json << "\n" << std::string(indent, ' ') << "}";
        visited.erase(node);
        return json.str();
    }

public:
    static std::string convert(XMLNode* node) {
        if (!node) return "null";
        
        try {
            std::unordered_set<std::shared_ptr<XMLNode>> visited;
            auto sharedNode = std::shared_ptr<XMLNode>(node);
            return convertNode(sharedNode, visited);
        } catch (const std::exception& e) {
            return "{\"error\": \"" + std::string(e.what()) + "\"}";
        }
    }
};