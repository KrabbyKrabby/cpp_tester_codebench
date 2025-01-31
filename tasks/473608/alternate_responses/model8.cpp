#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cctype>

// TrieNode structure
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    std::vector<std::pair<std::string, int>> contacts;

    TrieNode() : isEndOfWord(false) {}
};

// Trie class
class Trie {
public:
    TrieNode* root;

    Trie() : root(new TrieNode()) {}

    // Insert contact into the Trie
    void insert(const std::string& name, int number) {
        TrieNode* node = root;
        for (char c : name) {
            if (node->children.find(std::tolower(c)) == node->children.end()) {
                node->children[std::tolower(c)] = new TrieNode();
            }
            node = node->children[std::tolower(c)];
        }
        node->isEndOfWord = true;
        node->contacts.push_back({name, number});
    }

    // Find matching contacts in the Trie
    std::vector<std::pair<std::string, int>> findMatchingContacts(const std::string& query) {
        TrieNode* node = root;
        for (char c : query) {
            if (node->children.find(std::tolower(c)) == node->children.end()) {
                return {};
            }
            node = node->children[std::tolower(c)];
        }

        // Collect all contacts from the current node and its children
        std::vector<std::pair<std::string, int>> matchingContacts;
        collectContacts(node, matchingContacts);
        return matchingContacts;
    }

private:
    // Recursively collect contacts from the current node and its children
    void collectContacts(TrieNode* node, std::vector<std::pair<std::string, int>>& matchingContacts) {
        if (node->isEndOfWord) {
            matchingContacts.insert(matchingContacts.end(), node->contacts.begin(), node->contacts.end());
        }
        for (auto& child : node->children) {
            collectContacts(child.second, matchingContacts);
        }
    }
};

// Function to find matching contacts using the Trie data structure
std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    if (query.empty()) {
        return {};
    }

    Trie trie;
    for (const auto& contact : contacts) {
        trie.insert(contact.first, contact.second);
    }

    // Remove leading and trailing spaces from the query
    std::string trimmedQuery = query;
    trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" "));
    trimmedQuery.erase(trimmedQuery.find_last_not_of(" ") + 1);

    // Perform case-insensitive search
    std::string lowerCaseQuery;
    for (char c : trimmedQuery) {
        lowerCaseQuery += std::tolower(c);
    }

    std::vector<std::pair<std::string, int>> matchingContacts = trie.findMatchingContacts(lowerCaseQuery);

    // Sort the matching contacts
    std::sort(matchingContacts.begin(), matchingContacts.end());
    return matchingContacts;
}