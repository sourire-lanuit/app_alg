#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> 
#include <numeric>
#include <functional>

struct Stats {
    long long comp = 0;
    long long cop = 0;
    long long mem = 0;
};

double measure(std::function<void()> f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

std::vector<int> sorted(int n) {
    std::vector<int> a(n);
    std::iota(a.begin(), a.end(), 0);
    return a;
}

std::vector<int> reversed(int n) {
    auto a = sorted(n);
    reverse(a.begin(), a.end());
    return a;
}

std::vector<int> random(int n) {
    std::vector<int> a(n);
    for (int& x : a) x = rand();
    return a;
}

std::vector<int> almost_sorted(int n) {
    auto a = sorted(n);
    for (int i = 0; i < n / 20; i++) std::swap(a[rand() % n], a[rand() % n]);
    return a;
}

std::vector<int> few_val_data(int n) {
    std::vector<int> a(n);
    for (int& x : a) x = rand() % 5;
    return a;
}

void merge_sortTD(std::vector<int>&, Stats&);
void merge_sortBU(std::vector<int>&, Stats&);
void merge_sortOpt(std::vector<int>&, Stats&);
void merge_sort10(std::vector<int>&, Stats&);

int main() {
    srand(time(nullptr));

    std::vector<int> sizes = {10000, 50000, 100000};

    for (int n : sizes) {
        std::cout << "===== N = " << n << " =====\n";
        auto data = random(n);

        auto run = [&](std::string name, auto sort_func) {
            auto a = data;
            Stats s;
            double t = measure([&]() {
                sort_func(a, s);
            });
            std::cout << name << "\n";
            std::cout << "Time: " << t << " s\n";
            std::cout << "Comparisons: " << s.comp << "\n";
            std::cout << "Copies: " << s.cop << "\n";
            std::cout << "Memory: " << s.mem << " bytes\n\n";
        };

        run("Top-Down MergeSort", merge_sortTD);
        run("Bottom-Up MergeSort", merge_sortBU);
        run("Optimized MergeSort", merge_sortOpt);
        run("MergeSort (10 parts)", merge_sort10);
    }
}