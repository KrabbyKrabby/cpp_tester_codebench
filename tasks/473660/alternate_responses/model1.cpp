#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <memory>
#include <stdexcept>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<std::shared_ptr<XMLNode>> children;

    XMLNode(const std::string& name, const std::string& value = "") 
        : name(name), value(value) {}

    void addChild(std::shared_ptr<XMLNode> child) {
        if (!child) {
            throw std::invalid_argument("Cannot add null child node");
        }
        children.push_back(child);
    }
};

class XMLToJSONConverter {
private:
    static std::string escapeJSONString(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        output += buf;
                    } else {
                        output += c;
                    }
            }
        }
        return output;
    }

    static bool detectCircularReference(const XMLNode* node, 
                                     std::unordered_set<const XMLNode*>& visited) {
        if (!node) return false;
        if (visited.find(node) != visited.end()) return true;
        
        visited.insert(node);
        for (const auto& child : node->children) {
            if (detectCircularReference(child.get(), visited)) {
                return true;
            }
        }
        visited.erase(node);
        return false;
    }

    static std::string convertNode(const XMLNode* node, int indent = 0) {
        if (!node) {
            throw std::invalid_argument("Null node encountered");
        }

        std::string result;
        std::string indentation(indent, ' ');
        
        result += indentation + "{\n";
        result += indentation + "  \"" + escapeJSONString(node->name) + "\": ";

        if (node->children.empty()) {
            // Leaf node with value
            result += "\"" + escapeJSONString(node->value) + "\"";
        } else {
            // Node with children
            result += "[\n";
            
            for (size_t i = 0; i < node->children.size(); ++i) {
                result += convertNode(node->children[i].get(), indent + 4);
                if (i < node->children.size() - 1) {
                    result += ",";
                }
                result += "\n";
            }
            
            result += indentation + "  ]";
        }
        
        result += "\n" + indentation + "}";
        return result;
    }

public:
    static std::string convert(XMLNode* root) {
        if (!root) {
            throw std::invalid_argument("Root node cannot be null");
        }

        // Check for circular references
        std::unordered_set<const XMLNode*> visited;
        if (detectCircularReference(root, visited)) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }

        try {
            return convertNode(root);
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("JSON conversion failed: ") + e.what());
        }
    }
};