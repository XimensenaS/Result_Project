#include "utilities.h"

vector<string> Split_Into_Words(const string &text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
            }
            word.clear();
        } else {
            word.push_back(c);
        }
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

string Skip_Spec_Symbols(const string &inputString) {
    string resultString;
    for (const auto &character:inputString) {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')) {
            resultString += tolower(character);
        }
    }
    return resultString;
}