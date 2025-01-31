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
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        
        // Trim trailing spaces
        size_t end = str.find_last_not_of(" \t\n\r");
        std::string trimmed = str.substr(start, end - start + 1);
        
        // Convert to lowercase
        result.reserve(trimmed.length());
        for (char c : trimmed) {
            result += std::tolower(c);
        }
        return result;
    }

    void insertIntoTrie(const std::string& name, int number, const std::string& processedName) {
        TrieNode* current = root.get();
        
        for (char c : processedName) {
            if (!current->children[c]) {
                current->children[c] = std::make_unique<TrieNode>();
            }
            current = current->children[c].get();
            current->contacts.push_back({name, number});
        }
    }

    std::vector<std::pair<std::string, int>> searchInTrie(const std::string& processedQuery) const {
        TrieNode* current = root.get();
        
        for (char c : processedQuery) {
            if (!current->children[c]) {
                return {};
            }
            current = current->children[c].get();
        }
        
        std::vector<std::pair<std::string, int>> result = current->contacts;
        std::sort(result.begin(), result.end());
        return result;
    }

public:
    ContactTrie() : root(std::make_unique<TrieNode>()) {}

    void buildTrie(const std::vector<std::pair<std::string, int>>& contacts) {
        for (const auto& contact : contacts) {
            std::string processedName = preprocessString(contact.first);
            if (!processedName.empty()) {
                insertIntoTrie(contact.first, contact.second, processedName);
            }
        }
    }

    std::vector<std::pair<std::string, int>> search(const std::string& query) const {
        std::string processedQuery = preprocessString(query);
        if (processedQuery.empty()) {
            return {};
        }
        return searchInTrie(processedQuery);
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(
    const std::vector<std::pair<std::string, int>>& contacts,
    const std::string& query) {
    
    ContactTrie trie;
    trie.buildTrie(contacts);
    return trie.search(query);
}
