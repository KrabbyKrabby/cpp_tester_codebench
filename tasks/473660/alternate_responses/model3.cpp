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
    std::unordered_map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<XMLNode>> children;
    
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
    static std::string indent(int level) {
        return std::string(level * 2, ' ');
    }
    
    static bool hasCircularReference(const std::shared_ptr<XMLNode>& node, 
                                   std::unordered_set<const XMLNode*>& visited) {
        if (!node) return false;
        if (visited.find(node.get()) != visited.end()) return true;
        visited.insert(node.get());
        
        for (const auto& child : node->children) {
            if (hasCircularReference(child, visited)) return true;
        }
        
        visited.erase(node.get());
        return false;
    }
    
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
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    } else {
                        oss << c;
                    }
            }
        }
        return oss.str();
    }
    
    static std::string convertNode(const std::shared_ptr<XMLNode>& node, 
                                 int level, 
                                 std::unordered_set<const XMLNode*>& visited) {
        if (!node) return "null";
        
        std::ostringstream json;
        json << "{\n";
        
        // Add node name
        json << indent(level + 1) << "\"_name\": \"" << escapeJSON(node->name) << "\"";
        
        // Add attributes if present
        if (!node->attributes.empty()) {
            json << ",\n" << indent(level + 1) << "\"_attributes\": {";
            bool firstAttr = true;
            for (const auto& attr : node->attributes) {
                if (!firstAttr) json << ",";
                json << "\n" << indent(level + 2) 
                     << "\"" << escapeJSON(attr.first) << "\": \"" 
                     << escapeJSON(attr.second) << "\"";
                firstAttr = false;
            }
            json << "\n" << indent(level + 1) << "}";
        }
        
        // Add value if present
        if (!node->value.empty()) {
            json << ",\n" << indent(level + 1) 
                 << "\"_value\": \"" << escapeJSON(node->value) << "\"";
        }
        
        // Process children
        if (!node->children.empty()) {
            std::unordered_map<std::string, std::vector<std::string>> childrenJson;
            
            for (const auto& child : node->children) {
                if (!child) continue;
                if (visited.find(child.get()) != visited.end()) {
                    throw std::runtime_error("Circular reference detected");
                }
                visited.insert(child.get());
                childrenJson[child->name].push_back(
                    convertNode(child, level + 2, visited));
                visited.erase(child.get());
            }
            
            if (!childrenJson.empty()) {
                json << ",\n" << indent(level + 1) << "\"_children\": {";
                bool firstChild = true;
                
                for (const auto& pair : childrenJson) {
                    if (!firstChild) json << ",";
                    json << "\n" << indent(level + 2) << "\"" << escapeJSON(pair.first) << "\": ";
                    
                    if (pair.second.size() == 1) {
                        json << pair.second[0];
                    } else {
                        json << "[\n";
                        for (size_t i = 0; i < pair.second.size(); ++i) {
                            json << indent(level + 3) << pair.second[i];
                            if (i < pair.second.size() - 1) json << ",";
                            json << "\n";
                        }
                        json << indent(level + 2) << "]";
                    }
                    firstChild = false;
                }
                json << "\n" << indent(level + 1) << "}";
            }
        }
        
        json << "\n" << indent(level) << "}";
        return json.str();
    }
    
public:
    static std::string convert(XMLNode* node) {
        if (!node) return "null";
        
        auto sharedNode = std::shared_ptr<XMLNode>(node);
        std::unordered_set<const XMLNode*> visited;
        
        if (hasCircularReference(sharedNode, visited)) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        
        visited.clear();
        try {
            return convertNode(sharedNode, 0, visited);
        } catch (const std::exception& e) {
            return "{ \"error\": \"" + std::string(e.what()) + "\" }";
        }
    }
};
