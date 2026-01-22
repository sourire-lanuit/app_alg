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

void merge_sortBU(std::vector<int>& a, Stats& s) {
    int n = a.size();
    std::vector<int> aux(n);
    s.mem += n * sizeof(int);

    for (int sz = 1; sz < n; sz *= 2) {
        for (int l = 0; l < n - sz; l += 2 * sz) {
            int m = l + sz - 1;
            int r = std::min(l + 2 * sz - 1, n - 1);
            merge_arrays(a, aux, l, m, r, s);
        }
    }
}
