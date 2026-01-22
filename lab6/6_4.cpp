#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> 
#include <numeric>
#include <functional>

struct Stats {
    long long comp, cop, mem;
};

static void merge10(std::vector<int>& a, int l, int r, Stats& s) {
    if (r - l <= 1) return;

    int parts = 10;
    int len = (r - l) / parts;
    std::vector<int> cuts;

    for (int i = 0; i < parts; i++) cuts.push_back(l + i * len);
    cuts.push_back(r);

    for (int i = 0; i < parts; i++) merge10(a, cuts[i], cuts[i + 1], s);

    std::vector<int> tmp;
    for (int i = l; i < r; i++) {
        tmp.push_back(a[i]);
        s.cop++;
    }

    sort(tmp.begin(), tmp.end());

    for (int i = l; i < r; i++) {
        a[i] = tmp[i - l];
        s.cop++;
    }
}

void merge_sort10(std::vector<int>& a, Stats& s) {
    merge10(a, 0, a.size(), s);
}
