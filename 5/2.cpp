#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>

using namespace std;
mutex cout_mutex;

void generateArray(vector<int>& arr, int start, int end) {
    random_device rd; //source of randomness
    mt19937 gen(rd()); //generator object
    uniform_int_distribution<int> dist(0, 50000); //distribution of numbers

    for (int i = start; i < end; ++i) {
        arr[i] = dist(gen);
    }
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << this_thread::get_id()
        << " add elements [" << start << "-" << end - 1 << "]" << endl;
}


int generate_num(int max) {
    random_device rd; //source of randomness
    mt19937 gen(rd()); //generator object
    uniform_int_distribution<int> dist(0, max); //distribution of numbers
    int num = dist(gen);
    return num;
}

int main() {
    int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2;
    int vector_size = 5000;
    vector<thread> threads;
    vector<int> arr(vector_size);
    int block_size = vector_size / num_threads;


    for (int i = 0; i < num_threads; ++i) {
        int start = i * block_size;
        int end;
        if (i == num_threads - 1)
            end = vector_size;
        else
            end = (i + 1) * block_size;

        threads.emplace_back(generateArray, ref(arr), start, end);
    }
    cout << "begin" << endl;
    for (auto& thread : threads) {
        thread.join();
    }
    cout << "end" << endl;
    for (int i = 0; i < 20; ++i) {
        int find_num = generate_num(vector_size);

        cout << "Find: " << find_num << " - " << arr[find_num] << endl;
    }
    return 0;
}
