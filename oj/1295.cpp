#include <iostream>
#include <vector>
#include <iomanip>
signed main() {
    int n; std::cin >> n;
    if (n <= 1e6) {
        std::vector<bool> vis(n + 1, 0);
        std::vector<int> p;
        for (int i = 2; i <= n; ++i) {
            if (!vis[i]) p.push_back(i);
            for (const int j : p) {
                if (i * j > n) break;
                vis[i * j] = 1;
                if (i % j == 0) break;
            }
        }
        std::cout << std::fixed << std::setprecision(10) << double(p.size()) * 2 / n / n << std::endl;
    } else {
        std::cout << std::fixed << std::setprecision(10) << 0.0 << std::endl;
    }
}