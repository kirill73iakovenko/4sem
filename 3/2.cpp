#include <iostream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <vector>


using namespace std;


void bfs(string& root, map<string, vector<string>>& family_tree, map<string, int>& heights) {
    queue<string> q;
    q.push(root);
    heights[root] = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (family_tree.find(current) != family_tree.end()) {
            for (string& child : family_tree.at(current)) {
                heights[child] = heights[current] + 1;
                q.push(child);
            }
        }
    }
}

int main() {
    int N;
    cin >> N;

    set<string> all_people;
    set<string> children;
    map<string, vector<string>> family_tree;

    for (int i = 0; i < N - 1; ++i) {
        string child, parent;
        cin >> child >> parent;
        all_people.insert(child);
        all_people.insert(parent);
        children.insert(child);
        family_tree[parent].push_back(child);
    }

    
    string root;
    for (const auto& person : all_people) {
        if (!children.count(person)) {
            root = person;
            break;
        }
    }

    if (root.empty()) {
        cout << "Error: no the oldest parent." << endl;
        return 1;
    }

    map<string, int> heights;
    bfs(root, family_tree, heights);
    for (const string& person : all_people) {
        cout << person << " " << heights[person] << endl;
    }
    return 0;
}
