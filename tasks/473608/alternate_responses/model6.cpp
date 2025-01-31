#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

// Trie node structure
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    std::vector<std::pair<std::string, int>> contacts;
};

// Trie class
class Trie {
public:
    Trie() : root(new TrieNode()) {}

    // Insert contact into Trie
    void insert(const std::string& name, int number) {
        TrieNode* current = root;
        for (char c : name) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->contacts.push_back(std::make_pair(name, number));
    }

    // Search for contacts matching the query
    std::vector<std::pair<std::string, int>> search(const std::string& query) {
        TrieNode* current = root;
        for (char c : query) {
            if (current->children.find(c) == current->children.end()) {
                return {};
            }
            current = current->children[c];
        }
        return current->contacts;
    }

private:
    TrieNode* root;
};

// Function to find matching contacts
std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    if (query.empty()) {
        return {};
    }

    std::string trimmedQuery = query;
    trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" "));
    trimmedQuery.erase(trimmedQuery.find_last_not_of(" ") + 1);

    for (char& c : trimmedQuery) {
        c = std::tolower(c);
    }

    Trie trie;
    for (const auto& contact : contacts) {
        std::string name = contact.first;
        for (char& c : name) {
            c = std::tolower(c);
        }
        trie.insert(name, contact.second);
    }

    std::vector<std::pair<std::string, int>> matchingContacts = trie.search(trimmedQuery);
    std::sort(matchingContacts.begin(), matchingContacts.end());
    return matchingContacts;
}
