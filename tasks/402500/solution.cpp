#include <iostream>
#include <algorithm>
#include <string>
#include <set>        // For std::set
#include <sstream>    // For std::stringstream
using namespace std;

string decipherMessage(string str) {
    stringstream ss(str);
    string word, result;
    bool isFirst = true;
    
    while (ss >> word) {
        // Use set to remove duplicates and sort characters
        set<char> chars(word.begin(), word.end());
        
        // Add space before word (except for first word)
        if (!isFirst) {
            result += " ";
        }
        isFirst = false;
        
        // Add sorted unique characters to result
        for (char c : chars) {
            result += c;
        }
    }
    
    return result;
}