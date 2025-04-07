#include <iostream>
#include <vector>
#include <random>
#include <chrono>


using namespace std;



void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void quickSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    quickSort(arr, left, j);
    quickSort(arr, i, right);
}


void hybridSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    if (right - left < 10) {
        insertionSort(arr, left, right);
        return;
    }
    else {
        int pivot = arr[(left + right) / 2];
        int i = left, j = right;
        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        hybridSort(arr, left, j);
        hybridSort(arr, i, right);
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
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << name << ": " << duration.count() << " microseconds" << endl;
}

int main() {
    vector<int> sizes = { 10, 20, 100, 1000, 5000, 10000, 50000 };

    for (int size : sizes) {
        cout << "Array size: " << size << "\n";
        vector<int> arr = generateArray(size);


        vector<int> arr1(arr), arr2(arr);


        measureTime(arr1, [](vector<int>& v) {quickSort(v, 0, v.size() - 1); }, "quickSort");
        measureTime(arr2, [](vector<int>& v) {hybridSort(v, 0, v.size() - 1); }, "hybridSort");
        //hybridSort is more efficient than quickSort
    }

    return 0;
}
