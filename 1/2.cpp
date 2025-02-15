#include <iostream>
#include <list>

using namespace std;

template <typename T>
void Sort(list<T>& List_o, list<T>& List_p, list<T>& List_n) {

    for (auto num : List_o) {
        if (num > 0) {
            List_p.push_back(num);
        }
        else if (num < 0) {
            List_n.push_back(num);
        }
    }

    List_p.sort();
    List_n.sort();

};

template <typename Container>
void Print(const Container container, const string str) {
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
    list<int> list1 = { 3, -2, 9, 8, 5, 12, -18, -100, 56, 83, -12 };
    list<int> list_plus;
    list<int> list_minus;
    Sort(list1, list_plus, list_minus);
    cout << "positive num: ";
    Print(list_plus, ", ");
    cout << "\n";
    cout << "negative num: ";
    Print(list_minus, ", ");

    return 0;
}