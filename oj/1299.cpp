#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
using LL = long long;
using PII = std::pair<int, int>;
const LL INF = 1e18;

LL dis(const PII& a, const PII& b) {
    return LL(a.first - b.first) * (a.first - b.first) + LL(a.second - b.second) * (a.second - b.second);
}

bool cmp(const PII& a, const PII& b) {
    return a.second < b.second;
}

LL closest_pair(
    std::vector<PII>& a,
    int l,
    int r
) {
    if (r - l <= 2) {
        std::sort(a.begin() + l, a.begin() + r, cmp);
        return r - l == 1 ? INF : dis(a[l], a[l + 1]);
    }
    int m = (l + r) >> 1;
    int midline = a[m].first;
    LL d = std::min(closest_pair(a, l, m), closest_pair(a, m, r));
    std::inplace_merge(a.begin() + l, a.begin() + m, a.begin() + r, cmp);
    int x = l;
    static std::vector<int> got(a.size()); size_t sz = 0;
    LL res = d;
    int thr = std::sqrt(d);
    for (int x = l; x < r; ++x) {
        if (std::abs(a[x].first - midline) <= thr) {
            for (size_t i = sz - 1; ~i && a[x].second - a[got[i]].second <= thr; --i) {
                res = std::min(res, dis(a[x], a[got[i]]));
            }
            got[sz++] = x;
        }
    }
    return res;
}

signed main() {
    // std::ios::sync_with_stdio(false); std::cin.tie(0), std::cout.tie(0);
    int n; std::cin >> n;
    std::vector<PII> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i].first >> a[i].second;
    }
    std::sort(a.begin(), a.end());
    std::cout << closest_pair(a, 0, n) << std::endl;
    return 0;
}