#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstring>
using LL = long long;
signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n + 1);
    std::vector<int> p(n + 1);
    std::vector<bool> vis(n + 1, 0);
    bool flag = 0;
    for (int i = 1; i <= n; ++i) {
        std::cin >> a[i];
        p[a[i]] = i;
    }
    std::vector<std::pair<int, int> > mod;
    auto sw = [&](int i, int j){
        mod.push_back({i, j});
        vis[i] = 1;
        vis[j] = 1;
        p[a[i]] = j;
        p[a[j]] = i;
        std::swap(a[i], a[j]);
    };
    for (int i = 1; i <= n; ++i) {
        if (a[i] == i) continue;
        if (a[a[i]] == i) {
            if (vis[i] || vis[a[i]]) continue;
            sw(i, a[i]); continue;
        }
        flag = 1;
        int j = i;
        while (a[a[j]] != j) {
            int k = p[j];
            assert(!vis[p[j]] && !vis[a[j]]);
            sw(p[j], a[j]);
            j = k;
        }
    }
    if (flag) {
        std::cout << 2 << std::endl;
        std::cout << mod.size() << ' ';
        for (const auto& x : mod) std::cout << x.first << ' ' << x.second << ' ';
        std::cout << std::endl;
        mod.clear();
        std::fill(vis.begin(), vis.end(), 0);
        for (int i = 1; i <= n; ++i) {
            if (a[i] == i) continue;
            if (a[a[i]] == i) {
                if (vis[i] || vis[a[i]]) continue;
                sw(i, a[i]); continue;
            }
            assert(false);
        }
        std::cout << mod.size() << ' ';
        for (const auto& x : mod) std::cout << x.first << ' ' << x.second << ' ';
        std::cout << std::endl;
    } else {
        if (mod.empty()) {
            std::cout << 0 << std::endl;
            return 0;
        }
        std::cout << 1 << std::endl;
        std::cout << mod.size() << ' ';
        for (const auto& x : mod) std::cout << x.first << ' ' << x.second << ' ';
        std::cout << std::endl;
    }
    return 0;
}