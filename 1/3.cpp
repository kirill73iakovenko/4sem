#include <iostream>
#include <map>
#include <string>


using namespace std;

void lowersymbol(string* str) {
    for (char& c : *str) {
        c = tolower(c);
    }
}

string Find(string* text) {
    map<string, int> word_count;
    string temp;
    for (char c : *text) {
        if (isspace(c) or ispunct(c)) {
            if (!temp.empty()) {
                lowersymbol(&temp);
                word_count[temp]++;
                temp.clear();
            }
        }
        else {
            temp += c;
        }
    }

    if (!temp.empty()) {
        lowersymbol(&temp);
        word_count[temp]++;
    }
    string mostFrequentWord;
    int maxCount = 0;
    for (auto entry : word_count) {
        if (entry.second > maxCount or (entry.second == maxCount and entry.first < mostFrequentWord)) {
            mostFrequentWord = entry.first;
            maxCount = entry.second;
        }
    }
    return mostFrequentWord;
}


int main() {
    string text = "Hello world! Hello my friends";
    string result = Find(&text);
    cout << " The most  frequent word: " << result;

    return 0;
}
