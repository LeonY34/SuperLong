// naive implementation
#include <iostream>
#include <vector>

using uint = size_t;
using ull = unsigned long long;
const uint MOD = 998244353;

signed main() {
    uint n; std::cin >> n;
    std::vector<uint> f(n, 0); f[0] = 1;
    std::vector<uint> g(n);
    for (uint i = 1; i < n; ++i) std::cin >> g[i];
    for (uint i = 1; i < n; ++i) {
        for (uint j = 1; j <= i; ++j) {
            f[i] = (f[i] + ull(f[i - j]) * g[j] % MOD) % MOD;
        }
    }
    for (uint i = 0; i < n; ++i) std::cout << f[i] << ' ';
    std::cout << std::endl;
    return 0;
}