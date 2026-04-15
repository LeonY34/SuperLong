#include <iostream>
#include <vector>
#include <algorithm>

using PII = std::pair<int, int>;

void merge(
    std::vector<PII>& a,
    const int l, 
    const int r
) {
    if (r - l == 1) return;
    const int m = (l + r) >> 1;
    merge(a, l, m); merge(a, m, r);
    static std::vector<PII> tmp;
    tmp.clear(); 
    tmp.reserve(r - l + 1);
    int i = l, j = m;
    while (i < m && j < r) {
        if (a[i] <= a[j]) a[i].second += j - m, tmp.push_back(a[i++]);
        else a[j].second += m - i, tmp.push_back(a[j++]);
    }
    while (i < m) a[i].second += j - m, tmp.push_back(a[i++]);
    while (j < r) tmp.push_back(a[j++]);
    std::copy(tmp.begin(), tmp.end(), a.begin() + l);
}

signed main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0), std::cout.tie(0);
    int n; std::cin >> n;
    std::vector<PII> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i].first, a[i].second = 0;
    merge(a, 0, n);
    for (int i = 0; i < n; ++i) std::cout << a[i].second << ' ';
    std::cout << std::endl;
    return 0;
}