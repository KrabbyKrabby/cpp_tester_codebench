#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Trie Node Structure
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    std::vector<std::pair<std::string, int>> contacts;
};

// Trie Class
class Trie {
public:
    TrieNode* root;

    Trie() : root(new TrieNode()) {}

    // Insert a contact into the Trie
    void insert(const std::string& name, int number) {
        std::string lowerName = name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        TrieNode* node = root;
        for (char c : lowerName) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
        node->contacts.push_back({name, number});
    }

    // Search for contacts in the Trie based on the query
    std::vector<std::pair<std::string, int>> search(const std::string& query) {
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
        TrieNode* node = root;
        for (char c : lowerQuery) {
            if (node->children.find(c) == node->children.end()) {
                return {};
            }
            node = node->children[c];
        }
        return node->contacts;
    }
};

// Function to find matching contacts
std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    Trie trie;

    // Insert all contacts into the Trie
    for (const auto& contact : contacts) {
        trie.insert(contact.first, contact.second);
    }

    // Remove leading and trailing spaces from the query
    std::string trimmedQuery = query;
    trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" "));
    trimmedQuery.erase(trimmedQuery.find_last_not_of(" ") + 1);

    // If the query is empty, return no contacts
    if (trimmedQuery.empty()) {
        return {};
    }

    // Search for contacts in the Trie
    std::vector<std::pair<std::string, int>> matchingContacts = trie.search(trimmedQuery);

    // Sort the matching contacts
    std::sort(matchingContacts.begin(), matchingContacts.end());
    return matchingContacts;
}
