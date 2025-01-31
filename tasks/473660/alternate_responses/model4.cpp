#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <sstream>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(XMLNode* child) {
        if (child) {
            children.push_back(child);
        }
    }

    ~XMLNode() {
        for (XMLNode* child : children) {
            delete child;
        }
        children.clear();
    }
};

class XMLToJSONConverter {
private:
    static std::string escapeJSON(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"': oss << "\\\""; break;
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

    static std::string convertNode(XMLNode* node, int indent, std::unordered_set<XMLNode*>& visited) {
        if (!node) {
            return "null";
        }

        // Check for circular references
        if (visited.find(node) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node);

        std::ostringstream json;
        std::string indentation(indent, ' ');
        
        json << "{\n" << indentation << "  \"" << escapeJSON(node->name) << "\": ";

        if (node->children.empty()) {
            // Leaf node with value
            if (node->value.empty()) {
                json << "\"\"";
            } else {
                json << "\"" << escapeJSON(node->value) << "\"";
            }
        } else {
            // Node with children
            if (node->children.size() == 1) {
                json << convertNode(node->children[0], indent + 2, visited);
            } else {
                json << "[\n";
                for (size_t i = 0; i < node->children.size(); ++i) {
                    json << indentation << "    ";
                    json << convertNode(node->children[i], indent + 4, visited);
                    if (i < node->children.size() - 1) {
                        json << ",";
                    }
                    json << "\n";
                }
                json << indentation << "  ]";
            }
        }

        json << "\n" << indentation << "}";
        visited.erase(node);
        return json.str();
    }

public:
    static std::string convert(XMLNode* node) {
        if (!node) {
            return "null";
        }
        
        try {
            std::unordered_set<XMLNode*> visited;
            return convertNode(node, 0, visited);
        } catch (const std::exception& e) {
            return std::string("{\"error\": \"") + escapeJSON(e.what()) + "\"}";
        }
    }
};