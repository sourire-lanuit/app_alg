#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> 
#include <numeric>
#include <functional>

struct Stats {
    long long comp, cop, mem;
};

static void merge_arrays(std::vector<int>& a, std::vector<int>& aux, int l, int m, int r, Stats& s) {
    for (int i = l; i <= r; i++) {
        aux[i] = a[i];
        s.cop++;
    }

    int i = l, j = m + 1;
    for (int k = l; k <= r; k++) {
        if (i > m) a[k] = aux[j++];
        else if (j > r) a[k] = aux[i++];
        else {
            s.comp++;
            if (aux[j] < aux[i]) a[k] = aux[j++];
            else a[k] = aux[i++];
        }
        s.cop++;
    }
}

static void ms_rec(std::vector<int>& a, std::vector<int>& aux, int l, int r, Stats& s) {
    if (l >= r) return;
    int m = (l + r) / 2;
    ms_rec(a, aux, l, m, s);
    ms_rec(a, aux, m + 1, r, s);
    merge_arrays(a, aux, l, m, r, s);
}

void merge_sortTD(std::vector<int>& a, Stats& s) {
    std::vector<int> aux(a.size());
    s.mem += a.size() * sizeof(int);
    ms_rec(a, aux, 0, a.size() - 1, s);
}