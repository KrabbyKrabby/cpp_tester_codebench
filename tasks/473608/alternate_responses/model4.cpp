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
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class ContactTrie {
private:
    std::unique_ptr<TrieNode> root;

    std::string trimAndLower(const std::string& str) {
        std::string result = str;
        // Trim leading spaces
        result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
        // Trim trailing spaces
        result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1);
        // Convert to lowercase
        std::transform(result.begin(), result.end(), result.begin(), 
            [](unsigned char c){ return std::tolower(c); });
        return result;
    }

    void insertContact(TrieNode* node, const std::string& key, size_t index,
                      const std::pair<std::string, int>& contact) {
        if (index == key.length()) {
            node->isEndOfWord = true;
            node->contacts.push_back(contact);
            return;
        }

        char currentChar = std::tolower(key[index]);
        if (!node->children[currentChar]) {
            node->children[currentChar] = std::make_unique<TrieNode>();
        }
        insertContact(node->children[currentChar].get(), key, index + 1, contact);
    }

    void findMatchingContactsHelper(TrieNode* node, 
                                  std::vector<std::pair<std::string, int>>& result) {
        if (node->isEndOfWord) {
            result.insert(result.end(), node->contacts.begin(), node->contacts.end());
        }

        for (const auto& child : node->children) {
            findMatchingContactsHelper(child.second.get(), result);
        }
    }

    TrieNode* searchPrefix(TrieNode* node, const std::string& key, size_t index) {
        if (index == key.length()) {
            return node;
        }

        char currentChar = std::tolower(key[index]);
        if (!node->children[currentChar]) {
            return nullptr;
        }

        return searchPrefix(node->children[currentChar].get(), key, index + 1);
    }

public:
    ContactTrie() : root(std::make_unique<TrieNode>()) {}

    void insert(const std::pair<std::string, int>& contact) {
        insertContact(root.get(), contact.first, 0, contact);
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(
    const std::vector<std::pair<std::string, int>>& contacts,
    const std::string& query) {
    
    // Handle empty query
    if (query.empty()) {
        return std::vector<std::pair<std::string, int>>();
    }

    // Build Trie
    ContactTrie trie;
    for (const auto& contact : contacts) {
        trie.insert(contact);
    }

    // Process query
    std::string processedQuery = trie.trimAndLower(query);
    if (processedQuery.empty()) {
        return std::vector<std::pair<std::string, int>>();
    }

    // Search Trie
    std::vector<std::pair<std::string, int>> result;
    TrieNode* prefixNode = trie.searchPrefix(trie.root.get(), processedQuery, 0);
    
    if (prefixNode) {
        trie.findMatchingContactsHelper(prefixNode, result);
    }

    // Sort results
    std::sort(result.begin(), result.end());
    return result;
}