#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename Container>
void Print(Container container,string str) {
    bool indicator = true;
    for (auto element : container) {
        if (!indicator) {
            cout << str;
        }
        cout << element;
        indicator = false;
    }
    cout << '\n';
};

int main() {
    vector<int> data = { 1, 2, 3 };
    vector<string> colors = { "yellow", "green", "blue" };
    Print(data, ", ");
    Print(colors, "; ");
};
