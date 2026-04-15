#include <iostream>
#include <vector>
#include <algorithm>

struct BIT {
    std::vector<int> data;
    int n;
    BIT(int _n) : data(_n + 1, 0), n(_n) {}
    void add(int x, int p) {
        for (; x <= n; x += x & -x) data[x] += p;
    }
    int query(int x) {
        int res = 0;
        for (; x; x -= x & -x) res += data[x];
        return res;
    }
};

signed main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0), std::cout.tie(0);
    int n; std::cin >> n;
    std::vector<int> a(n + 1), ans(n + 1);
    for (int i = 1; i <= n; ++i) std::cin >> a[i];
    BIT t(n);
    for (int i = 1; i <= n; ++i) {
        ans[a[i]] = i - 1 - t.query(a[i]);
        t.add(a[i], 1);
    }
    for (int i = 1; i <= n; ++i) {
        t.add(a[i], -1);
        ans[a[i]] += t.query(a[i]);
    }
    for (int i = 1; i <= n; ++i) std::cout << ans[i] << ' ';
    std::cout << std::endl;
    return 0;
}