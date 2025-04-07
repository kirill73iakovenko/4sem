#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include <random>
#include <string>
#include <vector>


using namespace std;

enum Color { RED, BLACK };

struct Book {
    int id;
    string author;
    string title;
    bool operator<(const Book& other) const { return id < other.id; }
};

struct RBNode {
    Book book;
    Color color;
    RBNode* left, * right, * parent;
    RBNode(const Book& b)
        : book(b), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};
class RBTree {
private:
    RBNode* root;
    RBNode* nil; //special object as nullptr

    //balance
    void rotateLeft(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    //balance
    void rotateRight(RBNode* x) {
        RBNode* y = x->left;
        x->left = y->right;
        if (y->right != nil) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }
    //restoring tree after insert
    void fixInsert(RBNode* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBNode* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            }
            else {
                RBNode* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }
    //root replacement.Used during deletion.
    void transplant(RBNode* u, RBNode* v) {
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }
    //Pointer to the node with the minimum key in the subtree, the root of which is the node. Used during deletion.
    RBNode* minimum(RBNode* node) {
        while (node->left != nil) node = node->left;
        return node;
    }

    //restoring  tree after delete
    void fixDelete(RBNode* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBNode* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else {
                RBNode* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

public:
    RBTree() {
        nil = new RBNode({ -1, "", "" });
        nil->color = BLACK;
        root = nil;
    }

    ~RBTree() {
        clear(root);
        delete nil;
    }

    void clear(RBNode* node) {
        if (node != nil) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void insert(const Book& book) {
        RBNode* z = new RBNode(book);
        RBNode* y = nil;
        RBNode* x = root;
        while (x != nil) {
            y = x;
            if (z->book.id < x->book.id) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == nil) root = z;
        else if (z->book.id < y->book.id) y->left = z;
        else y->right = z;
        z->left = nil;
        z->right = nil;
        fixInsert(z);
    }

    void remove(int id) {
        RBNode* z = root;
        while (z != nil) {
            if (z->book.id == id) break;
            else if (id < z->book.id) z = z->left;
            else z = z->right;
        }
        if (z == nil) return;

        RBNode* y = z;
        Color y_original_color = y->color;
        RBNode* x;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK) fixDelete(x);
    }

    RBNode* search(int id) {
        RBNode* node = root;
        while (node != nil) {
            if (id == node->book.id) return node;
            else if (id < node->book.id) node = node->left;
            else node = node->right;
        }
        return nullptr;
    }

    void inorder(RBNode* node, vector<Book>& result) {
        if (node == nil) return;
        inorder(node->left, result);
        result.push_back(node->book);
        inorder(node->right, result);
    }

    vector<Book> getAllBooks() {
        vector<Book> result;
        inorder(root, result);
        return result;
    }

    void saveToFile(const string& filename) {
        ofstream out(filename);
        vector<Book> books = getAllBooks();
        for (const auto& book : books) {
            out << "ID: " << book.id << "\nAuthor: " << book.author
                << "\nTitle: " << book.title << "\n\n";
        }
        out.close();
    }
};



vector<Book> generateBooks(int count) {
    vector<Book> books;
    random_device rd;
    mt19937 gen(rd());

    
    uniform_int_distribution<> idDist(1, count * 100);

    
    uniform_int_distribution<> lenDist(5, 15);

    
    uniform_int_distribution<> nameDist(1, 1000);

    for (int i = 0; i < count; ++i) {
        int id;
        do {
            id = idDist(gen);  // guarantee of uniqueness
        } while (any_of(books.begin(), books.end(), [id](const Book& b) { return b.id == id; }));

       
        string author = "Author_" + to_string(nameDist(gen));
        string title = "Title_" + to_string(nameDist(gen));

        books.push_back({ id, author, title });
    }
    return books;
}

template <typename F>
long long measureTime(F func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}


int main() {
    vector<Book> books = generateBooks(100000);


    RBTree rbTree;
    long long rbTreeInsertTime = measureTime([&]() {
        for (const auto& book : books) rbTree.insert(book);
        });
    cout << "RBTree Insert: " << rbTreeInsertTime << " ms\n";

    rbTree.saveToFile("books.txt");
   // vector<Book> allBooks = rbTree.getAllBooks();

    //cout << "Books in RBTree (sorted by ID):" << endl;
    //for (const auto& book : allBooks) {
       // cout << "ID: " << book.id << ", Author: " << book.author << ", Title: " << book.title << endl;
    //}


    long long rbTreeSearchTime = measureTime([&]() {
        for (int i = 0; i < 10000; ++i) rbTree.search(books[i].id);
        });
    cout << "RBTree Search: " << rbTreeSearchTime << " ms\n";


    long long rbTreeCountTime = measureTime([&]() {
        int foundCount = 0;
        for (int i = 0; i < 10000; ++i) {
            if (rbTree.search(books[i].id) != nullptr) {
                foundCount++;
            }
        }
        cout << "Found " << foundCount << " elements" << endl;
        });
    cout << "RBTree Count: " << rbTreeCountTime << " ms\n";



    map<int, Book> stdMap;
    long long stdMapInsertTime = measureTime([&]() {
        for (const auto& book : books) stdMap[book.id] = book;
        });
    cout << "std::map Insert: " << stdMapInsertTime << " ms\n";

    long long stdMapSearchTime = measureTime([&]() {
        for (int i = 0; i < 10000; ++i) stdMap.find(books[i].id);
        });
    cout << "std::map Search: " << stdMapSearchTime << " ms\n";

    long long stdMapCountTime = measureTime([&]() {
        int foundCount = 0;
        for (int i = 0; i < 10000; ++i) {
            if (stdMap.find(books[i].id) != stdMap.end()) {
                foundCount++;
            }
        }
        cout << "Found " << foundCount << " elements" << endl;
        });
    cout << "std::map Count: " << stdMapCountTime << " ms\n";

    //RBTree is more efficient than map

    return 0;
} 
