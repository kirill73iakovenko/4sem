#include <iostream>
#include <vector>
#include <random>
#include <chrono>


using namespace std;


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


void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int p = 0; p < k; ++p) {
        arr[left + p] = temp[p];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        swap(arr[i], arr[min_idx]);
    }
}

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
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
    cout << name << ": " << duration.count() << " microseconds" <<endl;
}

int main() {
    vector<int> sizes = { 1000, 5000, 10000, 50000 };

    for (int size : sizes) {
        cout << "Array size: " << size << "\n";
        vector<int> arr = generateArray(size);

        
        vector<int> arr1(arr), arr2(arr), arr3(arr), arr4(arr);

        
        measureTime(arr1, [](vector<int>& v) {quickSort(v, 0, v.size() - 1); }, "quickSort");
        measureTime(arr2, [](vector<int>& v) {mergeSort(v, 0, v.size() - 1); }, "mergeSort");
        measureTime(arr3, [](vector<int>& v) {selectionSort(v);  }, "selectionSort");
        measureTime(arr4, [](vector<int>& v) {bubbleSort(v);  }, "bubbleSort");
    }

    return 0;
}
