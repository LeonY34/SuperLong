#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

void read_input_data(int& n, int& k, std::vector<int>& a) {
    int m;
    std::cin >> n >> k >> m; a.resize(n);
    for (int i = 0; i < m; i++){
        std::cin >> a[i];
    }
    unsigned int z = a[m - 1];
    for (int i = m; i < n; i++) {
        z ^= z << 13;
        z ^= z >> 17;
        z ^= z << 5;
        a[i] = z & 0x7fffffff;
    }
}

int randint(const int l, const int r) {
    static int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::mt19937 rnd(seed);
    return rnd() % (r - l + 1) + l;
}

int kth_element_q(std::vector<int>& a, const int l, const int r, const int k) {
    if (l == r) return a[l];
    std::swap(a[l], a[randint(l, r)]);
    int i = l + 1, j = r;
    while (i < j) {
        while (i < j && a[i] <= a[l]) ++i;
        while (i < j && a[j] > a[l]) --j;
        if (i < j) std::swap(a[i], a[j]);
    }
    if (a[i] > a[l]) --i;
    std::swap(a[l], a[i]);
    if (k == i - l + 1) return a[i];
    if (k < i - l + 1) return kth_element_q(a, l, i - 1, k);
    return kth_element_q(a, i + 1, r, k - (i - l + 1));
}

signed main() {
    int n, k;
    std::vector<int> a;
    read_input_data(n, k, a);
    std::cout << kth_element_q(a, 0, n - 1, k) << std::endl;
    return 0;
}