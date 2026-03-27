// even slower than recursion version
// main reason: cache loss (jump in data with big strides)
// ~2200ms
#include <complex>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

using Comp = std::complex<double>;
const double PI = std::acos(-1);

std::vector<Comp> Interpolate(
    const std::vector<Comp>& data,
    bool rev
) {
    int n = data.size();
    std::vector<Comp> res(data);
    std::vector<Comp> pre(data.size());
    for (int stride = (n >> 1), num = 2; stride >= 1; stride >>= 1, num <<= 1) {
        std::swap(res, pre);
        const Comp omega = rev ? std::polar(double(1.0), -PI * 2 / num) : std::polar(double(1.0), PI * 2 / num);
        for (int off = 0; off < stride; ++off) {
            Comp o1 = 1;
            for (int j = 0; j < num / 2; ++j, o1 *= omega) {
                const int p0 = off + j * 2 * stride;
                const int p1 = off + (j * 2 + 1) * stride;
                const int p = off + j * stride;
                const Comp now = pre[p1] * o1;
                res[p] = pre[p0] + now;
                res[p + (n >> 1)] = pre[p0] - now;
            }
        }
    }
    return res;
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
    std::vector<Comp> a = Interpolate(f, 0);
    std::vector<Comp> b = Interpolate(g, 0);
    for (int i = 0; i < sz; ++i) a[i] *= b[i]; // a = c
    std::vector<Comp> res = Interpolate(a, 1);
    for (int i = 0; i < n + m - 1; ++i) std::cout << int(res[i].real() / sz + 0.5) << ' ';
    std::cout << std::endl;
    return 0;
}