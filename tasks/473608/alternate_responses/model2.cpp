#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <cctype>

class TrieNode {
public:
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    std::vector<std::pair<std::string, int>> contacts;
};

class ContactTrie {
private:
    std::unique_ptr<TrieNode> root;

    static std::string preprocessString(const std::string& str) {
        std::string result;
        // Trim leading spaces
        size_t start = str.find_first_not_of(" \t");
        if (start == std::string::npos) return "";
        
        // Trim trailing spaces
        size_t end = str.find_last_not_of(" \t");
        
        // Convert to lowercase
        result = str.substr(start, end - start + 1);
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    void insertContact(const std::string& name, int number) {
        std::string processedName = preprocessString(name);
        TrieNode* current = root.get();
        
        for (char c : processedName) {
            if (!current->children[c]) {
                current->children[c] = std::make_unique<TrieNode>();
            }
            current = current->children[c].get();
            current->contacts.push_back({name, number});
        }
    }

    std::vector<std::pair<std::string, int>> searchPrefix(const std::string& prefix) {
        TrieNode* current = root.get();
        
        for (char c : prefix) {
            if (!current->children[c]) {
                return {};
            }
            current = current->children[c].get();
        }
        
        return current->contacts;
    }

public:
    ContactTrie() : root(std::make_unique<TrieNode>()) {}

    void buildTrie(const std::vector<std::pair<std::string, int>>& contacts) {
        for (const auto& contact : contacts) {
            insertContact(contact.first, contact.second);
        }
    }

    std::vector<std::pair<std::string, int>> findMatches(const std::string& query) {
        std::string processedQuery = preprocessString(query);
        if (processedQuery.empty()) {
            return {};
        }
        
        auto matches = searchPrefix(processedQuery);
        std::sort(matches.begin(), matches.end());
        return matches;
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(
    const std::vector<std::pair<std::string, int>>& contacts,
    const std::string& query) {
    
    ContactTrie trie;
    trie.buildTrie(contacts);
    return trie.findMatches(query);
}