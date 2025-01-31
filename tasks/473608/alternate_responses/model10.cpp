#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

// Define a TrieNode structure to represent each node in the Trie
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    std::vector<std::pair<std::string, int>> contacts;
};

// Function to insert a contact into the Trie
void insertContact(TrieNode* root, const std::string& name, int number) {
    TrieNode* current = root;
    for (char c : name) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->contacts.push_back({name, number});
}

// Function to search for contacts in the Trie based on the query
std::vector<std::pair<std::string, int>> searchContacts(TrieNode* root, const std::string& query) {
    TrieNode* current = root;
    for (char c : query) {
        if (current->children.find(c) == current->children.end()) {
            return {}; // Return empty vector if query not found
        }
        current = current->children[c];
    }
    return current->contacts;
}

// Function to find matching contacts using the Trie data structure
std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    // Remove leading and trailing spaces from the query
    std::string trimmedQuery = query;
    trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" "));
    trimmedQuery.erase(trimmedQuery.find_last_not_of(" ") + 1);

    // If query is empty, return empty vector
    if (trimmedQuery.empty()) {
        return {};
    }

    // Convert query to lowercase for case-insensitive search
    std::transform(trimmedQuery.begin(), trimmedQuery.end(), trimmedQuery.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Create a Trie and insert all contacts into it
    TrieNode* root = new TrieNode();
    for (const auto& contact : contacts) {
        std::string name = contact.first;
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        insertContact(root, name, contact.second);
    }

    // Search for contacts in the Trie based on the query
    std::vector<std::pair<std::string, int>> matchingContacts = searchContacts(root, trimmedQuery);

    // Sort the matching contacts
    std::sort(matchingContacts.begin(), matchingContacts.end());

    // Clean up the Trie
    // ... (Trie cleanup code omitted for brevity)

    return matchingContacts;
}