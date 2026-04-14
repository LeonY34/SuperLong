#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>
using LL = long long;
using PII = std::pair<int, int>;
const LL INF = 1e18;

LL dis(const PII& a, const PII& b) {
    return LL(a.first - b.first) * (a.first - b.first) + LL(a.second - b.second) * (a.second - b.second);
}

struct Comp {
    bool operator()(const PII& a, const PII& b) const {
        return a.second == b.second ? a.first < b.first : a.second < b.second;
    }
};

inline LL sq(int x) {
    return LL(x) * x;
}

LL closest_pair_multiset(
    const std::vector<PII>& a,
    int n
) {
    LL res = INF;
    // std::multiset<PII, Comp> mp;
    std::set<PII, Comp> mp; // 用set也行，如果坐标相同，则已经为0了
    for (int i = 0, j = 0; i < n; ++i) {
        const PII& x = a[i];
        while (j < i && sq(x.first - a[j].first) >= res) {
            // mp.erase(mp.find(a[j++]));
            mp.erase(a[j++]); // 全删了都行，如果坐标相同，则已经为0了
        }
        auto itl = mp.insert(x).first;
        auto itr = itl; ++itr;
        while (itl != mp.begin()) {
            --itl;
            if (sq(x.second - itl->second) >= res) {
                break;
            }
            res = std::min(res, dis(*itl, x));
        }
        while (itr != mp.end()) {
            if (sq(itr->second - x.second) >= res) {
                break;
            }
            res = std::min(res, dis(*itr, x));
            ++itr;
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
    std::cout << closest_pair_multiset(a, n) << std::endl;
    return 0;
}