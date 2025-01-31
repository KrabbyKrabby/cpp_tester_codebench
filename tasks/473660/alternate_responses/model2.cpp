#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <stdexcept>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") 
        : name(name), value(value) {}

    void addChild(XMLNode* child) {
        if (child) {
            children.push_back(child);
        }
    }

    ~XMLNode() {
        for (XMLNode* child : children) {
            delete child;
        }
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        if (!node) {
            return "null";
        }
        
        std::unordered_set<XMLNode*> visited;
        try {
            return convertNode(node, visited, 0);
        } catch (const std::runtime_error& e) {
            return "\"error\": \"" + std::string(e.what()) + "\"";
        }
    }

private:
    static std::string convertNode(XMLNode* node, std::unordered_set<XMLNode*>& visited, int depth) {
        if (!node) {
            return "null";
        }

        // Check for circular references
        if (visited.find(node) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node);

        std::string indent(depth * 2, ' ');
        std::string result = "{\n";
        result += indent + "  \"" + escapeJSON(node->name) + "\": ";

        if (node->children.empty()) {
            // Leaf node with value
            result += "\"" + escapeJSON(node->value) + "\"";
        } else {
            // Node with children
            result += "{\n";
            
            bool firstChild = true;
            for (XMLNode* child : node->children) {
                if (!firstChild) {
                    result += ",\n";
                }
                firstChild = false;
                
                if (child) {
                    result += indent + "    \"" + escapeJSON(child->name) + "\": ";
                    if (child->children.empty()) {
                        result += "\"" + escapeJSON(child->value) + "\"";
                    } else {
                        result += convertNode(child, visited, depth + 2);
                    }
                }
            }
            
            result += "\n" + indent + "  }";
        }

        result += "\n" + indent + "}";
        visited.erase(node);
        return result;
    }

    static std::string escapeJSON(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '\"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        char buf[8];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        output += buf;
                    } else {
                        output += c;
                    }
            }
        }
        return output;
    }
};