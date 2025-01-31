#include <string>
#include <sstream>
#include <set>
#include <vector>

string decipherMessage(string str) {
    stringstream ss(str);
    string word;
    vector<string> result;
    
    // Process each word
    while (ss >> word) {
        // Remove duplicates using set
        set<char> uniqueChars(word.begin(), word.end());
        
        // Create sorted word from unique characters
        string processedWord(uniqueChars.begin(), uniqueChars.end());
        
        result.push_back(processedWord);
    }
    
    // Combine processed words
    string deciphered;
    for (int i = 0; i < result.size(); i++) {
        deciphered += result[i];
        if (i < result.size() - 1) {
            deciphered += " ";
        }
    }
    
    return deciphered;
}