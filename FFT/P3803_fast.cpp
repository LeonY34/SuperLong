// use customized Comp & pointers
// ~470ms
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

struct Comp {
    double a, b;
    Comp() {}
    Comp(const double a_, const double b_) : a(a_), b(b_) {}
    Comp(const int x) : a(x), b(0) {}
    Comp(const Comp& other) : a(other.a), b(other.b) {}
    inline Comp& operator = (const int x) {
        a = x, b = 0; return *this;
    }
    inline Comp operator * (const Comp& other) const {
        return Comp(a * other.a - b * other.b, a * other.b + b * other.a);
    }
    inline Comp& operator *= (const Comp& other) {
        double x = a * other.a - b * other.b;
        double y = a * other.b + b * other.a;
        a = x, b = y;
        return *this;
    }
    inline Comp operator + (const Comp& other) const {
        return Comp(a + other.a, b + other.b);
    }
    inline Comp operator - (const Comp& other) const {
        return Comp(a - other.a, b - other.b);
    }
    friend std::istream& operator >> (std::istream& in, Comp& cp) {
        int x; in >> x;
        cp = x;
        return in;
    }
};

const double PI = std::acos(-1);

// change data form to [even_0, even_1, ..., even_n; odd_0, odd_1, ..., odd_n]
// instead of [even_0, odd_0, even_1, odd_1, ..., even_n, odd_n]
void change(
    Comp* data,
    const size_t n,
    bool clean=false
) {
    static size_t* rev;
    static bool init = false;
    if (clean) {
        delete[] rev;
        return;
    }
    if (!init) {
        rev = new size_t[n];
        for (size_t i = 0; i < n; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i & 1) rev[i] |= (n >> 1);
        }
        init = true;
    }
    for (size_t i = 0; i < n; ++i) {
        if (i < rev[i]) std::swap(data[i], data[rev[i]]);
    }
}

// warning: this function will change the initial data
void Interpolate(
    Comp* data,
    const size_t n,
    bool rev
) {
    change(data, n);
    for (size_t len = 1; len <= n / 2; len <<= 1) {
        const Comp omega(std::cos(PI / len), std::sin(rev ? -PI / len : PI / len));
        for (size_t i = 0; i < n; i += len * 2) {
            // [i, i + len) is even part, [i + len, i + len * 2) is odd part
            Comp o = 1;
            for (size_t j = i; j < i + len; ++j, o *= omega) {
                Comp v0 = data[j];
                Comp v1 = o * data[j + len];
                data[j] = v0 + v1;
                data[j + len] = v0 - v1;
            }
        }
    }
}

size_t higher(size_t x) {
    bool f = 0;
    while (x != (x & -x)) {
        x -= (x & -x);
        f = 1;
    }
    return x << f;
}

signed main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0), std::cout.tie(0);
    size_t n, m; std::cin >> n >> m; ++n, ++m;
    size_t sz = higher(n + m - 1);
    Comp* f = new Comp[sz];
    Comp* g = new Comp[sz];
    for (int i = 0; i < n; ++i) std::cin >> f[i];
    for (int i = 0; i < m; ++i) std::cin >> g[i];
    Interpolate(f, sz, 0); // f <- F(f)
    Interpolate(g, sz, 0); // g <- G(g)
    for (int i = 0; i < sz; ++i) f[i] *= g[i]; // f <- F(f) * G(g)
    delete[] g;
    Interpolate(f, sz, 1); // f <- inv(F(f) * G(g))
    change(f, 0, 1);
    for (int i = 0; i < n + m - 1; ++i) std::cout << int(f[i].a / sz + 0.5) << ' ';
    delete[] f;
    std::cout << std::endl;
    return 0;
}