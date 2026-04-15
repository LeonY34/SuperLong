// median of medians，差不多比随机慢三倍。
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
    // for (int i = 0; i < n; ++i) {
    //     std::cout << a[i] << ' ';
    // }
    // std::cout << std::endl;
}

void read_input_data_2(int& n, int& k, std::vector<int>& a) {
    std::cin >> n >> k; a.resize(n);
    for (int i = 0; i < n; i++){
        std::cin >> a[i];
    }
}

int find_pivot(std::vector<int>& a, const int l, const int r);

int kth_element_me(std::vector<int>& a, const int l, const int r, const int k) {
    if (l == r) return l;
    std::swap(a[l], a[find_pivot(a, l, r)]);
    int i = l + 1, j = r;
    while (i < j) {
        while (i < j && a[i] <= a[l]) ++i;
        while (i < j && a[j] > a[l]) --j;
        if (i < j) std::swap(a[i], a[j]);
    }
    if (a[i] > a[l]) --i;
    std::swap(a[l], a[i]);
    if (k == i - l + 1) return i;
    if (k < i - l + 1) return kth_element_me(a, l, i - 1, k);
    return kth_element_me(a, i + 1, r, k - (i - l + 1));
}

int find_pivot(std::vector<int>& a, const int l, const int r) {
    int p = l - 1;
    for (int i = l; i <= r; i += 5) {
        int j = std::min(r, i + 4);
        std::sort(a.begin() + i, a.begin() + j + 1);
        std::swap(a[(i + j) >> 1], a[++p]);
    }
    return kth_element_me(a, l, p, (p - l + 2) >> 1);
}

signed main() {
    int n, k;
    std::vector<int> a;
    read_input_data(n, k, a);
    // read_input_data_2(n, k, a);
    std::cout << a[kth_element_me(a, 0, n - 1, k)] << std::endl;
    return 0;
}