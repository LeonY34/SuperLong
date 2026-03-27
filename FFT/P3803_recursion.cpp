// original version
// ~1880ms
#include <complex>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

using Comp = std::complex<double>;
const double PI = std::acos(-1);

std::vector<Comp> Interpolate_naive(
    const std::vector<Comp>& data,
    const std::vector<Comp>& o
) {
    if (data.size() != o.size()) {
        throw std::runtime_error("size error\n");
    }
    std::vector<Comp> a(data.size(), 0);
    for (int i = 0; i < int(o.size()); ++i) {
        Comp now = 1;
        for (int j = 0; j < int(data.size()); ++j) {
            a[i] += now * data[j];
            now *= o[i];
        }
    }
    return a;
}

std::vector<Comp> Interpolate(
    const std::vector<Comp>& data,
    const std::vector<Comp>& o
) {
    if (data.size() != o.size()) {
        throw std::runtime_error("size error\n");
    }
    if (data.size() == 1) {
        return std::vector<Comp>(1, data[0]);
    }
    int sz = data.size();
    std::vector<Comp> o_(sz >> 1);
    std::vector<Comp> d[2] = {std::vector<Comp>(sz >> 1), std::vector<Comp>(sz >> 1)};
    std::vector<Comp> hf[2];
    std::vector<Comp> a(sz);
    for (int i = 0; i < sz; i += 2) o_[i >> 1] = o[i];
    for (int i = 0; i < sz; ++i) d[i & 1][i >> 1] = data[i];
    for (int i = 0; i < 2; ++i) hf[i] = Interpolate(d[i], o_);
    for (int i = 0; i < (sz >> 1); ++i) {
        const Comp tmp = o[i] * hf[1][i];
        a[i] = tmp + hf[0][i];
        a[i + (sz >> 1)] = hf[0][i] - tmp;
    }
    return a;
}

int higher(int x) {
    bool f = 0;
    while (x != (x & -x)) {
        x -= (x & -x);
        f = 1;
    }
    return x << f;
}

signed main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0), std::cout.tie(0);
    int n, m; std::cin >> n >> m; ++n, ++m;
    int sz = higher(n + m - 1);
    std::vector<Comp> f(sz, 0);
    std::vector<Comp> g(sz, 0);
    for (int i = 0; i < n; ++i) std::cin >> f[i];
    for (int i = 0; i < m; ++i) std::cin >> g[i];
    std::vector<Comp> o(sz);
    const Comp omega = std::polar((double)1.0, PI * 2 / sz);
    o[0] = 1;
    for (int i = 1; i < sz; ++i) o[i] = o[i - 1] * omega;
    std::vector<Comp> a = Interpolate(f, o);
    std::vector<Comp> b = Interpolate(g, o);
    for (int i = 0; i < sz; ++i) a[i] *= b[i]; // a = c
    std::reverse(o.begin() + 1, o.end());
    std::vector<Comp> res = Interpolate(a, o);
    for (int i = 0; i < n + m - 1; ++i) std::cout << int(res[i].real() / sz + 0.5) << ' ';
    std::cout << std::endl;
    return 0;
}