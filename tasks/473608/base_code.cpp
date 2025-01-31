#include <vector>
#include <string>
#include <algorithm>

std::vector<std::pair<std::string, int>> findMatchingContacts(const std::vector<std::pair<std::string, int>>& contacts, const std::string& query) {
    std::vector<std::pair<std::string, int>> matchingContacts;

    for (const auto& contact : contacts) {
        if (contact.first.substr(0, query.size()) == query) {
            matchingContacts.push_back(contact);
        }
    }

    std::sort(matchingContacts.begin(), matchingContacts.end());
    return matchingContacts;
}