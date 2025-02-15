#include <iostream>
#include <stack>
#include <string>

using namespace std;


bool Check(string& str) {
	stack<char> stack;

    for (char symb : str) {
        if (symb == '(' or symb == '[' or symb == '{') {
            stack.push(symb); 
        }
        else {
            if (stack.empty()) {
                return false; 
            }
            char top = stack.top();
            stack.pop();

            if ((symb == ')' and top != '(') or
                (symb == ']' and top != '[') or
                (symb == '}' and top != '{')) {
                return false;
            }
        }
    }
    return stack.empty();
}


int main() {
	string text = "{[()]}";
    if (Check(text)) {
        cout << "Right" << endl;
    }
    else {
        cout << "Wrong" << endl;
    }

    return 0;
	

}