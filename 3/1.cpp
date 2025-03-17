#include <iostream>
#include <list>
#include <string>
#include <stdexcept>

using namespace std;

template<typename KeyType>
class Hasher {
public:
    size_t operator()(const KeyType& key, size_t tableSize) const {
        return static_cast<size_t>(key) % tableSize;
    }
};


template<typename KeyType, typename ValueType>
class HashTable{
private:
    struct KeyValuePair {
        KeyType key;
        ValueType value;
        KeyValuePair(const KeyType& k, const ValueType& v) : key(k), value(v) {}
    };

    size_t TABLE_SIZE = 11;
    Hasher<KeyType> hasher;
    list<KeyValuePair>* table;
    size_t getIndex(const KeyType& key) const {
        return hasher(key, TABLE_SIZE);
    }

public:
    HashTable() : table(new list<KeyValuePair>[TABLE_SIZE]) {}
    void insert(const KeyType& key, const ValueType& value) {
        size_t index = getIndex(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                pair.value = value;
                return;
            }
        }
        table[index].push_back(KeyValuePair(key, value));
    }

    ValueType find(const KeyType& key) const {
        size_t index = getIndex(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw out_of_range("Key not found");  
    }

};

int main() {
    HashTable<int, string> table;
    table.insert(1, "a");
    table.insert(2, "b");
    table.insert(12, "c"); 
    try {
        cout << table.find(1) << endl;
        cout << table.find(12) << endl;
        cout << table.find(13) << endl;
    }
    catch (const out_of_range& e) {
        cout << e.what() << endl;
    }
    return 0;
}
