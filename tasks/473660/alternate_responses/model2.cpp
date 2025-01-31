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
    static std::string escapeJSON(const std::string& input) {
        std::ostringstream oss;
        for (char c : input) {
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

    static std::string convertNode(const std::shared_ptr<XMLNode>& node, 
                                 std::unordered_set<const XMLNode*>& visited,
                                 int indent = 0) {
        if (!node) {
            return "null";
        }

        // Check for circular reference
        if (visited.find(node.get()) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node.get());

        std::ostringstream json;
        json << "{\n";
        std::string indentStr(indent + 2, ' ');

        // Add name
        json << indentStr << "\"nodeName\": \"" << escapeJSON(node->name) << "\"";

        // Add attributes if present
        if (!node->attributes.empty()) {
            json << ",\n" << indentStr << "\"attributes\": {";
            bool firstAttr = true;
            for (const auto& attr : node->attributes) {
                if (!firstAttr) json << ",";
                json << "\n" << indentStr << "  \"" << escapeJSON(attr.first) 
                     << "\": \"" << escapeJSON(attr.second) << "\"";
                firstAttr = false;
            }
            json << "\n" << indentStr << "}";
        }

        // Add value if present
        if (!node->value.empty()) {
            json << ",\n" << indentStr << "\"value\": \"" << escapeJSON(node->value) << "\"";
        }

        // Add children if present
        if (!node->children.empty()) {
            json << ",\n" << indentStr << "\"children\": [\n";
            for (size_t i = 0; i < node->children.size(); ++i) {
                if (i > 0) json << ",\n";
                json << convertNode(node->children[i], visited, indent + 4);
            }
            json << "\n" << indentStr << "]";
        }

        json << "\n" << std::string(indent, ' ') << "}";

        visited.erase(node.get());
        return json.str();
    }

public:
    static std::string convert(XMLNode* node) {
        if (!node) {
            return "null";
        }

        std::shared_ptr<XMLNode> rootNode(node);
        std::unordered_set<const XMLNode*> visited;

        try {
            return convertNode(rootNode, visited);
        } catch (const std::exception& e) {
            return "{\"error\": \"" + std::string(e.what()) + "\"}";
        }
    }
};