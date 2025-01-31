#include <string>
#include <sstream>
#include <set>
#include <vector>

string decipherMessage(string str) {
    stringstream ss(str);
    string word, result;
    bool isFirst = true;
    
    while (ss >> word) {
        // Use set to remove duplicates and sort characters
        set<char> uniqueChars(word.begin(), word.end());
        
        // Add space before word (except for first word)
        if (!isFirst) {
            result += " ";
        }
        isFirst = false;
        
        // Add sorted unique characters to result
        for (char c : uniqueChars) {
            result += c;
        }
    }
    
    return result;
}