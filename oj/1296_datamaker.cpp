#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>
signed main() {
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    int n = rng() % 100000 + 1;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i + 1;
    std::shuffle(a.begin(), a.end(), rng);
    std::cout << n << std::endl;
    for (int i = 0; i < n; ++i) std::cout << a[i] << ' ';
    std::cout << std::endl;
}