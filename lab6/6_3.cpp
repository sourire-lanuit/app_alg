#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> 
#include <numeric>
#include <functional>

struct Stats {
    long long comp, cop, mem;
};

static const int cut = 16;

static void ins_sort(std::vector<int>& a, int l, int r, Stats& s) {
    for (int i = l + 1; i <= r; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= l) {
            s.comp++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                s.cop++;
                j--;
            } else break;
        }
        a[j + 1] = key;
        s.cop++;
    }
}

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

void merge_sortOpt(std::vector<int>& a, Stats& s) {
    int n = a.size();
    std::vector<int> aux(n);
    s.mem += n * sizeof(int);

    for (int i = 0; i < n; i += cut)
        ins_sort(a, i, std::min(i + cut - 1, n - 1), s);

    for (int sz = cut; sz < n; sz *= 2) {
        for (int l = 0; l < n - sz; l += 2 * sz) {
            int m = l + sz - 1;
            int r = std::min(l + 2 * sz - 1, n - 1);
            s.comp++;
            if (a[m] <= a[m + 1]) continue;
            merge_arrays(a, aux, l, m, r, s);
        }
    }
}
