#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

void base_bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


void optimize_bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; 
    }
}

vector<int> generateArray(int size) {
    random_device rd; //source of randomness
    mt19937 gen(rd()); //generator object
    uniform_int_distribution<int> dist(0, 50000); //distribution of numbers

    vector<int> arr(size);
    for (int& num : arr) {
        num = dist(gen);
    }
    return arr;
}


void measureTime(vector<int> arr, void (*sortFunc)(vector<int>&), const string& name) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << name << ": " << duration.count() << " milliseconds" << endl;
}

int main() {
    vector<int> sizes = {500, 1000, 5000, 10000, 20000, 50000};
    for (int size : sizes) {
        cout << "Array size: " << size << "\n";
        vector<int> arr = generateArray(size);


        vector<int> arr1(arr), arr2(arr);

        measureTime(arr1, [](vector<int>& v) {base_bubbleSort(v);  }, "base_bubbleSort");
        measureTime(arr2, [](vector<int>& v) {optimize_bubbleSort(v);  }, "optimize_bubbleSort");
    }
   //The optimized algorithm is no more efficient than the basic 

    return 0;
}
