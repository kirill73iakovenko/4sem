#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <future>
#include <thread>
#include <cmath>


using namespace std;

const int MIN_SIZE = 1000;
void base_quickSort(vector<int>& arr, int left, int right) {
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
    base_quickSort(arr, left, j);
    base_quickSort(arr, i, right);
}

void quickSortMultiThreaded(vector<int>& arr, int left, int right, int depth = 0) {
    if (left >= right) return;

    if (right - left < MIN_SIZE) {
        base_quickSort(arr, left, right);
        return;
    }

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

    static const int max_depth = static_cast<int>(
        log2(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 2)
        );

    if (depth < max_depth) {
        auto future = async(launch::async, [&arr, left, j, depth]() {
            quickSortMultiThreaded(arr, left, j, depth + 1);
            });
        quickSortMultiThreaded(arr, i, right, depth + 1);
        future.get();
    }
    else {
        quickSortMultiThreaded(arr, left, j, depth + 1);
        quickSortMultiThreaded(arr, i, right, depth + 1);
    }
}

void parallelQuickSort(vector<int>& arr) {
    quickSortMultiThreaded(arr, 0, arr.size() - 1);
}

int binarySearch(const vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1; 
}



int binarySearchParallel(const vector<int>& arr, int left, int right, int target, int depth = 0) {
    if (left > right)
        return -1;

    if (right - left <= MIN_SIZE) {
        return binarySearch(arr, left, right, target);
    }

    int mid = left + (right - left) / 2;
    if (arr[mid] == target)
        return mid;

    static const int max_depth = static_cast<int>(
        log2(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 2)
        );

    if (depth < max_depth) {
        
        auto left_future = async(launch::async, binarySearchParallel, ref(arr), left, mid - 1, target, depth + 1);
        
        int right_result = binarySearchParallel(arr, mid + 1, right, target, depth + 1);

        int left_result = left_future.get();

        if (left_result != -1)
            return left_result;
        return right_result;
    }
    else {
        
        int left_result = binarySearch(arr, left, mid - 1, target);
        if (left_result != -1)
            return left_result;
        return binarySearch(arr, mid + 1, right, target);
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
    vector<int> sizes = { 1000, 5000, 10000, 50000, 100000 };

    for (int size : sizes) {
        cout << "Array size: " << size << "\n";
        vector<int> arr = generateArray(size);


        vector<int> arr1(arr), arr2(arr);


        measureTime(arr1, [](vector<int>& v) {base_quickSort(v, 0, v.size() - 1); }, "base_quickSort");
        measureTime(arr2, [](vector<int>& v) {parallelQuickSort(v); }, "new_quickSort");
        measureTime(arr1, [](vector<int>& v) {binarySearch(v, 0, v.size() - 1, 10000); }, "base_binary_search");
        measureTime(arr1, [](vector<int>& v) {binarySearchParallel(v, 0, v.size() - 1, 10000); }, "new_binary_search");
    }

    return 0;
}

//sorting is more efficient with parallelization, searching is more efficient without parallelization.
