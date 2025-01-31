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
    bool isEndOfWord = false;
};

class ContactTrie {
private:
    std::unique_ptr<TrieNode> root;

    void insertContact(const std::string& name, int number) {
        TrieNode* current = root.get();
        std::string lowercaseName = name;
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);

        for (char c : lowercaseName) {
            if (!current->children[c]) {
                current->children[c] = std::make_unique<TrieNode>();
            }
            current = current->children[c].get();
        }
        current->isEndOfWord = true;
        current->contacts.emplace_back(name, number);
    }

    std::vector<std::pair<std::string, int>> searchPrefix(const std::string& prefix) {
        TrieNode* current = root.get();
        std::vector<std::pair<std::string, int>> results;

        for (char c : prefix) {
            if (!current->children[c]) {
                return results;
            }
            current = current->children[c].get();
        }

        collectContacts(current, results);
        return results;
    }

    void collectContacts(TrieNode* node, std::vector<std::pair<std::string, int>>& results) {
        if (!node) return;
        
        if (node->isEndOfWord) {
            results.insert(results.end(), node->contacts.begin(), node->contacts.end());
        }

        for (const auto& child : node->children) {
            collectContacts(child.second.get(), results);
        }
    }

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

public:
    ContactTrie() : root(std::make_unique<TrieNode>()) {}

    std::vector<std::pair<std::string, int>> findMatchingContacts(
        const std::vector<std::pair<std::string, int>>& contacts,
        const std::string& query) {
            
        if (contacts.empty() || query.empty()) {
            return std::vector<std::pair<std::string, int>>();
        }

        // Build Trie
        for (const auto& contact : contacts) {
            insertContact(contact.first, contact.second);
        }

        // Process query
        std::string processedQuery = trim(query);
        if (processedQuery.empty()) {
            return std::vector<std::pair<std::string, int>>();
        }

        std::transform(processedQuery.begin(), processedQuery.end(), 
                      processedQuery.begin(), ::tolower);

        // Search and sort results
        auto results = searchPrefix(processedQuery);
        std::sort(results.begin(), results.end());
        
        return results;
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(
    const std::vector<std::pair<std::string, int>>& contacts,
    const std::string& query) {
    
    static ContactTrie trie;
    return trie.findMatchingContacts(contacts, query);
}
