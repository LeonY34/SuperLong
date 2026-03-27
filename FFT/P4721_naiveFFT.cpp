// using FFT, have precision problem
// correctness vertified, but precision not good.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using LD = long double;
using ull = unsigned long long;
const size_t MOD = 998244353;

struct Comp {
    LD a, b;
    Comp() {}
    Comp(const LD x, const LD y) : a(x), b(y) {}
    Comp(const size_t x) : a(x), b(0) {}
    Comp(const Comp& other) : a(other.a), b(other.b) {}
    Comp& operator = (const size_t x) {
        a = x, b = 0; return *this;
    }
    Comp& operator = (const Comp& other) {
        a = other.a, b = other.b; return *this;
    }
    Comp& operator *= (const Comp& other) {
        LD x = a * other.a - b * other.b;
        LD y = a * other.b + b * other.a;
        a = x, b = y; return *this;
    }
    Comp operator * (const Comp& other) const {
        return Comp(a * other.a - b * other.b, a * other.b + b * other.a);
    }
    Comp operator + (const Comp& other) const {
        return Comp(a + other.a, b + other.b);
    }
    Comp operator - (const Comp& other) const {
        return Comp(a - other.a, b - other.b);
    }
};

inline size_t higher(size_t x) {
    bool f = 0;
    while (x != (x & -x)) {
        f = 1;
        x -= (x & -x);
    }
    return x << f;
}

void change(
    std::vector<Comp>& a
) {
    std::vector<size_t> res(a.size()); res[0] = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        res[i] = (res[i >> 1] >> 1);
        if (i & 1) res[i] |= (a.size() / 2);
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (i < res[i]) std::swap(a[i], a[res[i]]);
    }
}

const LD PI = std::acos(-1);

void Interpolate(
    std::vector<Comp>& a,
    bool rev
) {
    change(a);
    const size_t n = a.size();
    for (size_t m = 1; m <= n / 2; m <<= 1) {
        Comp omega(std::cos(PI / m), std::sin(rev ? -PI / m : PI / m));
        for (size_t i = 0; i < n; i += m * 2) {
            Comp o(1);
            for (size_t j = i; j < i + m; ++j, o *= omega) {
                Comp v0 = a[j];
                Comp v1 = a[j + m] * o;
                a[j] = v0 + v1;
                a[j + m] = v0 - v1;
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
    std::vector<Comp> a(n, 0);
    std::vector<Comp> b(n);
    for (size_t i = 0; i < n/2; ++i) a[i] = f[i + l];
    for (size_t i = 0; i < n; ++i) b[i] = g[i];
    Interpolate(a, 0);
    Interpolate(b, 0);
    for (size_t i = 0; i < n; ++i) a[i] *= b[i];
    Interpolate(a, 1);
    for (size_t i = m; i < r; ++i) f[i] = (ull(a[i - l].a / n + 0.5) + f[i]) % MOD;
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