#include <iostream>
#include <random>
#include <chrono>

signed main() {
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rnd(seed);
    const int maxn = 100;
    const int minn = 100;
    const int maxx = 10000000;
    // int n = rnd() % (maxn - 1) + 2;
    int n = rnd() % (maxn - minn + 1) + minn;
    std::cout << n << std::endl;
    for (int i = 0; i < n; ++i) {
        int x = (rnd() % maxx + 1) * (rnd() % 1 ? -1 : 1);
        int y = (rnd() % maxx + 1) * (rnd() % 1 ? -1 : 1);
        std::cout << x << ' ' << y << std::endl;
    }
    return 0;
}