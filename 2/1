#include <iostream>
#include <deque>
#include <string>

using namespace std;

class Task {
private:
	string level;
	string info;
public:
	Task(string p_level, string p_info) {
		level = p_level;
		info = p_info;
	}
	void print_info() {
		cout << "Level of task: " << level << endl;
		cout << "Information: " << info << endl;
	}
	string get_level() {
		return level;
	}
};

void Push_Task(Task task, deque<Task>& task_list) {
	if (task.get_level() == "high") {
		task_list.push_front(task);
	}
	else if (task.get_level() == "low") {
		task_list.push_back(task);
	}
	else {
		cout << "The level is incorrect" << endl;
	}
}

void Use_Task(deque<Task>& task_list) {
	if (task_list.empty()) {
		cout << "List of task is empty!" << endl;
	}
	else {
		Task task = task_list.front();
		task.print_info();
		task_list.pop_front();
	}
}

int main() {
	Task task1("high", "eat");
	Task task2("low", "make homework in C++");
	Task task3("high", "sleep");
	Task task4("good", "drink");
	deque<Task> task_list1;
	Push_Task(task1, task_list1);
	Push_Task(task2, task_list1);
	Push_Task(task3, task_list1);
	Push_Task(task4, task_list1);
	Use_Task(task_list1);
	Use_Task(task_list1);
	Use_Task(task_list1);
	Use_Task(task_list1);
	return 0;
}
	
