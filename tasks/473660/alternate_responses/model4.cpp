#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>
#include <sstream>

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
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '\"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    } else {
                        oss << c;
                    }
            }
        }
        return oss.str();
    }

    static bool hasCircularReference(const std::shared_ptr<XMLNode>& node, 
                                   std::unordered_set<const XMLNode*>& visited) {
        if (!node) return false;
        if (!visited.insert(node.get()).second) return true;
        
        for (const auto& child : node->children) {
            if (hasCircularReference(child, visited)) return true;
        }
        
        visited.erase(node.get());
        return false;
    }

    static std::string convertNode(const std::shared_ptr<XMLNode>& node, int indent = 0) {
        if (!node) return "null";

        std::ostringstream json;
        json << "{\n";
        std::string padding(indent + 2, ' ');

        // Handle node name
        json << padding << "\"nodeName\": \"" << escapeJSON(node->name) << "\"";

        // Handle attributes
        if (!node->attributes.empty()) {
            json << ",\n" << padding << "\"attributes\": {";
            bool firstAttr = true;
            for (const auto& attr : node->attributes) {
                if (!firstAttr) json << ",";
                json << "\n" << padding << "  \"" << escapeJSON(attr.first) 
                     << "\": \"" << escapeJSON(attr.second) << "\"";
                firstAttr = false;
            }
            json << "\n" << padding << "}";
        }

        // Handle value
        if (!node->value.empty()) {
            json << ",\n" << padding << "\"value\": \"" << escapeJSON(node->value) << "\"";
        }

        // Handle children
        if (!node->children.empty()) {
            json << ",\n" << padding << "\"children\": [\n";
            for (size_t i = 0; i < node->children.size(); ++i) {
                if (i > 0) json << ",\n";
                json << padding << "  " << convertNode(node->children[i], indent + 2);
            }
            json << "\n" << padding << "]";
        }

        json << "\n" << std::string(indent, ' ') << "}";
        return json.str();
    }

public:
    static std::string convert(XMLNode* rawNode) {
        if (!rawNode) return "null";

        // Convert raw pointer to shared_ptr for safety
        std::shared_ptr<XMLNode> node(rawNode, [](XMLNode* p) { /* Custom deleter to avoid double deletion */ });

        // Check for circular references
        std::unordered_set<const XMLNode*> visited;
        if (hasCircularReference(node, visited)) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }

        try {
            return convertNode(node);
        } catch (const std::exception& e) {
            return "{ \"error\": \"" + std::string(e.what()) + "\" }";
        }
    }
};