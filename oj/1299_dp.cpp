#include <iostream>
#include <vector>
#include <algorithm>
using LL = long long;
using PII = std::pair<int, int>;
const LL INF = 1e18;

LL dis(const PII& a, const PII& b) {
    return LL(a.first - b.first) * (a.first - b.first) + LL(a.second - b.second) * (a.second - b.second);
}

LL closest_pair_naive(
    std::vector<PII>& a, int n
) {
    LL d = INF;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            d = std::min(d, dis(a[i], a[j]));
        }
    }
    return d;
}

signed main() {
    int n; std::cin >> n;
    std::vector<PII> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i].first >> a[i].second;
    }
    std::cout << closest_pair_naive(a, n) << std::endl;
    return 0;
}