// using NTT to avoid precision problem
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

using ull = unsigned long long;
const size_t MOD = 998244353;
const size_t g = 3;
// g ^ (119 * 2^23) = 1 (mod 998244353)

size_t higher(size_t x) {
    bool f = 0;
    while (x != (x & -x)) {
        f = 1;
        x -= (x & -x);
    }
    return x << f;
}

size_t fpow(size_t x, size_t y) {
    size_t a = 1;
    while (y) {
        if (y & 1) a = ull(a) * x % MOD;
        x = ull(x) * x % MOD;
        y >>= 1;
    }
    return a;
}

void change(
    std::vector<size_t>& a
) {
    std::vector<size_t> res(a.size()); res[0] = 0;
    for (size_t i = 1; i < a.size(); ++i) {
        res[i] = (res[i >> 1] >> 1);
        if (i & 1) res[i] |= (a.size() / 2);
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (i < res[i]) std::swap(a[i], a[res[i]]);
    }
}

void Interpolate(
    std::vector<size_t>& a,
    bool rev
) {
    change(a);
    const size_t n = a.size();
    for (size_t m = 1; m <= n / 2; m <<= 1) {
        size_t omega = fpow(g, (MOD - 1) / (m * 2));
        if (rev) omega = fpow(omega, MOD - 2);
        for (size_t i = 0; i < n; i += m * 2) {
            size_t o = 1;
            for (size_t j = i; j < i + m; ++j, o = ull(o) * omega % MOD) {
                size_t v0 = a[j];
                size_t v1 = ull(a[j + m]) * o % MOD;
                a[j] = (v0 + v1) % MOD;
                a[j + m] = (v0 + MOD - v1) % MOD;
            }
        }
    }
}

// f[l, m) is calculated
// f[m, r) is zero
// f <- f * g
void convolution(
    std::vector<size_t>& f,
    const std::vector<size_t>& g,
    const size_t l,
    const size_t r
) {
    const size_t m = ((l + r) >> 1);
    const size_t n = r - l;
    std::vector<size_t> a(n, 0);
    std::vector<size_t> b(n);
    for (size_t i = 0; i < n/2; ++i) a[i] = f[i + l];
    for (size_t i = 0; i < n; ++i) b[i] = g[i];
    Interpolate(a, 0);
    Interpolate(b, 0);
    for (size_t i = 0; i < n; ++i) a[i] = ull(a[i]) * b[i] % MOD;
    Interpolate(a, 1);
    size_t inv_n = fpow(n, MOD - 2);
    for (size_t i = m; i < r; ++i) f[i] = (f[i] + ull(a[i - l]) * inv_n % MOD) % MOD;
}

void Print(
    const std::vector<size_t>& f,
    const size_t l, const size_t r
) {
    printf("l = %d, r = %d\nf = ", l, r);
    for (size_t i = 0; i < f.size(); ++i) printf("%d ", f[i]);
    printf("\n");
}

// assuming f[0, l) is done (and done the contribution to [l, r))
// calculate f[l, r)
void solve(
    std::vector<size_t>& f,
    const std::vector<size_t>& g,
    const size_t l, 
    const size_t r
) {
    if (r - l == 1) return;
    // Print(f, l, r);
    const size_t m = ((l + r) >> 1);
    solve(f, g, l, m); // solve for [l, m)
    convolution(f, g, l, r); // calculate contribution of [l, m) to [m, r)
    solve(f, g, m, r); // solve for [m, r)
}

signed main() {
    size_t n; std::cin >> n;
    size_t sz = higher(n);
    std::vector<size_t> f(sz, 0); f[0] = 1;
    std::vector<size_t> g(sz); g[0] = 0;
    for (size_t i = 1; i < n; ++i) std::cin >> g[i];
    solve(f, g, 0, sz);
    for (size_t i = 0; i < n; ++i) std::cout << f[i] << ' ';
    std::cout << std::endl;
    return 0;
}