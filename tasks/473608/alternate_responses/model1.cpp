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

    static std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), 
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    void insert(const std::string& str, int number) {
        TrieNode* current = root.get();
        std::string lowered = toLower(str);

        for (char c : lowered) {
            if (!current->children[c]) {
                current->children[c] = std::make_unique<TrieNode>();
            }
            current = current->children[c].get();
        }

        current->isEndOfWord = true;
        current->contacts.push_back({str, number});
    }

    std::vector<std::pair<std::string, int>> search(const std::string& query) const {
        TrieNode* current = root.get();
        std::string lowered = toLower(query);

        for (char c : lowered) {
            if (!current->children[c]) {
                return {};
            }
            current = current->children[c].get();
        }

        std::vector<std::pair<std::string, int>> results;
        collectContacts(current, results);
        std::sort(results.begin(), results.end());
        return results;
    }

    void collectContacts(TrieNode* node, std::vector<std::pair<std::string, int>>& results) const {
        if (!node) return;

        results.insert(results.end(), node->contacts.begin(), node->contacts.end());

        for (const auto& child : node->children) {
            collectContacts(child.second.get(), results);
        }
    }

public:
    ContactTrie() : root(std::make_unique<TrieNode>()) {}

    std::vector<std::pair<std::string, int>> findMatchingContacts(
        const std::vector<std::pair<std::string, int>>& contacts,
        const std::string& query) {
            
        // Reset and rebuild trie
        root = std::make_unique<TrieNode>();
        for (const auto& contact : contacts) {
            insert(contact.first, contact.second);
        }

        // Handle empty or whitespace-only query
        std::string trimmedQuery = trim(query);
        if (trimmedQuery.empty()) {
            return {};
        }

        return search(trimmedQuery);
    }
};

std::vector<std::pair<std::string, int>> findMatchingContacts(
    const std::vector<std::pair<std::string, int>>& contacts,
    const std::string& query) {
    
    static ContactTrie trie;
    return trie.findMatchingContacts(contacts, query);
}
