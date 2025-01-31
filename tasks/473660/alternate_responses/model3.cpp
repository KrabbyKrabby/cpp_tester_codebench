#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
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

        // Check for circular references
        if (visited.find(node.get()) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node.get());

        std::ostringstream json;
        std::string indentation(indent, ' ');
        
        json << "{\n" << indentation << "  \"" << escapeJSONString(node->name) << "\": ";

        if (node->children.empty()) {
            // Leaf node with value
            json << "\"" << escapeJSONString(node->value) << "\"";
        } else {
            // Node with children
            json << "{\n";
            
            for (size_t i = 0; i < node->children.size(); ++i) {
                json << indentation << "    ";
                json << convertNode(node->children[i], visited, indent + 4);
                
                if (i < node->children.size() - 1) {
                    json << ",";
                }
                json << "\n";
            }
            
            json << indentation << "  }";
        }

        json << "\n" << indentation << "}";
        
        visited.erase(node.get());
        return json.str();
    }

public:
    static std::string convert(std::shared_ptr<XMLNode> root) {
        if (!root) {
            throw std::invalid_argument("Root node cannot be null");
        }

        try {
            std::unordered_set<const XMLNode*> visited;
            return convertNode(root, visited);
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("JSON conversion failed: ") + e.what());
        }
    }
};