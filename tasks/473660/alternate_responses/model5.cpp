#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <algorithm>

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
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    } else {
                        oss << c;
                    }
            }
        }
        return oss.str();
    }

    static std::string convertNode(std::shared_ptr<XMLNode> node, 
                                 std::unordered_set<std::shared_ptr<XMLNode>>& visited) {
        if (!node) {
            return "null";
        }

        // Check for circular reference
        if (visited.find(node) != visited.end()) {
            throw std::runtime_error("Circular reference detected in XML structure");
        }
        visited.insert(node);

        std::ostringstream json;
        json << "{";

        // Handle attributes
        if (!node->attributes.empty()) {
            json << "\"@attributes\":{";
            bool firstAttr = true;
            for (const auto& attr : node->attributes) {
                if (!firstAttr) json << ",";
                json << "\"" << escapeJSON(attr.first) << "\":\"" 
                     << escapeJSON(attr.second) << "\"";
                firstAttr = false;
            }
            json << "}";
            if (!node->value.empty() || !node->children.empty()) {
                json << ",";
            }
        }

        // Handle value
        if (!node->value.empty()) {
            if (!node->children.empty()) {
                json << "\"#text\":\"" << escapeJSON(node->value) << "\",";
            } else {
                json << "\"#text\":\"" << escapeJSON(node->value) << "\"";
            }
        }

        // Handle children
        if (!node->children.empty()) {
            std::unordered_map<std::string, std::vector<std::string>> childrenJson;
            
            for (const auto& child : node->children) {
                if (child) {
                    childrenJson[child->name].push_back(
                        convertNode(child, visited)
                    );
                }
            }

            bool firstChild = true;
            for (const auto& entry : childrenJson) {
                if (!firstChild) json << ",";
                json << "\"" << escapeJSON(entry.first) << "\":";
                
                if (entry.second.size() == 1) {
                    json << entry.second[0];
                } else {
                    json << "[";
                    for (size_t i = 0; i < entry.second.size(); ++i) {
                        if (i > 0) json << ",";
                        json << entry.second[i];
                    }
                    json << "]";
                }
                firstChild = false;
            }
        }

        json << "}";
        visited.erase(node);
        return json.str();
    }

public:
    static std::string convert(XMLNode* node) {
        if (!node) return "null";
        
        std::unordered_set<std::shared_ptr<XMLNode>> visited;
        auto sharedNode = std::shared_ptr<XMLNode>(new XMLNode(node->name, node->value));
        
        // Copy attributes and children to shared_ptr version
        sharedNode->attributes = node->attributes;
        for (auto* child : node->children) {
            if (child) {
                sharedNode->addChild(std::shared_ptr<XMLNode>(
                    new XMLNode(child->name, child->value)
                ));
            }
        }
        
        try {
            return convertNode(sharedNode, visited);
        } catch (const std::exception& e) {
            return "{\"error\":\"" + std::string(e.what()) + "\"}";
        }
    }
};