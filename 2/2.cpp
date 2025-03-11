#include <iostream>
#include<map>
#include<string>

using namespace std;

class Phone_book {
private:
	map<string, string> book;
public:
	void Add(string p_name, string p_number) {
		if (book.count(p_name) == 1) {
			cout << "Contact is already in book" << endl;
		}
		else {
			book[p_name] = p_number;
		}
	}
	void Delete(string p_name) {
		book.erase(p_name);
	}
	string Find(string p_name) {
		if (book.count(p_name) == 0) {
			cout << p_name << " - contact is not in book" << endl;
			return "No contact";
		}
		else {
			cout << p_name << ": " << book[p_name] << endl;
			return book[p_name];
		}
	}

	void All_Contacts() {
		cout << "Contacts: " << endl;
		for (const auto& pair : book) {
			cout << pair.first << ": " << pair.second << endl;
		}
	}
};

int main() {
	map<string, string> book1;
	Phone_book Book1;
	Book1.Add("Idrisov", "111");
	Book1.Add("Sakbaev", "222");
	Book1.Add("Idrisov", "112");
	Book1.Add("Umnov", "333");
	Book1.Find("Idrisov");
	Book1.Delete("Idrisov");
	Book1.All_Contacts();
	Book1.Find("Idrisov");
	return 0;
}
