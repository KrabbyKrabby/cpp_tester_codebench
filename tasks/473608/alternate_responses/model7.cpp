#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    std::vector<std::pair<std::string, int>> contacts;
};

class Trie {
public:
    TrieNode* root;

    Trie() : root(new TrieNode()) {}

    void insert(const std::string& contact, int index) {
        TrieNode* node = root;
        for (char c : contact) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->contacts.push_back({contact, index});
    }

    std::vector<std::pair<std::string, int>> search(const std::string& query) {
        TrieNode* node = root;
        for (char c : query) {
            if (node->children.find(c) == node->children.end()) {
                return {};
            }
            node = node->children[c];
        }
        return node->contacts;
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    std::string trimmedQuery = query;
    trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" "));
    trimmedQuery.erase(trimmedQuery.find_last_not_of(" ") + 1);

    if (trimmedQuery.empty()) {
        return {};
    }

    std::transform(trimmedQuery.begin(), trimmedQuery.end(), trimmedQuery.begin(), ::tolower);

    Trie trie;
    for (const auto& contact : contacts) {
        std::string lowerCaseContact = contact.first;
        std::transform(lowerCaseContact.begin(), lowerCaseContact.end(), lowerCaseContact.begin(), ::tolower);
        trie.insert(lowerCaseContact, contact.second);
    }

    std::vector<std::pair<std::string, int>> matchingContacts = trie.search(trimmedQuery);
    std::sort(matchingContacts.begin(), matchingContacts.end());
    return matchingContacts;
}