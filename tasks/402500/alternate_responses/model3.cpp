#include <string>
#include <sstream>
#include <set>
#include <algorithm>

string decipherMessage(string str) {
    stringstream ss(str);
    string word, result;
    bool isFirst = true;
    
    while (ss >> word) {
        // Add space between words except for the first word
        if (!isFirst) {
            result += " ";
        }
        isFirst = false;
        
        // Remove duplicates using set
        set<char> uniqueChars(word.begin(), word.end());
        
        // Convert set back to string and sort
        string processedWord(uniqueChars.begin(), uniqueChars.end());
        sort(processedWord.begin(), processedWord.end());
        
        result += processedWord;
    }
    
    return result;
}
