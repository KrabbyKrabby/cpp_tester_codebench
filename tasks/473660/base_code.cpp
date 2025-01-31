#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class XMLNode {
public:
    std::string name;
    std::string value;
    std::vector<XMLNode*> children;

    XMLNode(const std::string& name, const std::string& value = "") : name(name), value(value) {}

    void addChild(XMLNode* child) {      
        children.push_back(child);
    }
};

class XMLToJSONConverter {
public:
    static std::string convert(XMLNode* node) {
        // BUG: Doesn't handle invalid XML properly or check for null pointers
      
};