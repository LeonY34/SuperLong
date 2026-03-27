// use customized Comp
// ~590ms
#include <complex>
#include <iostream>
#include <vector>
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
    std::vector<Comp>& data
) {
    const int n = data.size();
    static std::vector<int> rev(data.size());
    static bool init = false;
    if (!init) {
        for (int i = 0; i < n; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i & 1) rev[i] |= (n >> 1);
        }
        init = true;
    } else if (data.size() != rev.size()) {
        throw std::runtime_error("size not match.");
    }
    for (int i = 0; i < n; ++i) {
        if (i < rev[i]) std::swap(data[i], data[rev[i]]);
    }
}

// warning: this function will change the initial data
void Interpolate(
    std::vector<Comp>& data,
    bool rev
) {
    const int n = data.size();
    change(data);
    for (int len = 1; len <= n / 2; len <<= 1) {
        const Comp omega(std::cos(PI / len), std::sin(rev ? -PI / len : PI / len));
        for (int i = 0; i < n; i += len * 2) {
            // [i, i + len) is even part, [i + len, i + len * 2) is odd part
            Comp o = 1;
            for (int j = i; j < i + len; ++j, o *= omega) {
                Comp v0 = data[j];
                Comp v1 = o * data[j + len];
                data[j] = v0 + v1;
                data[j + len] = v0 - v1;
            }
        }
    }
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
    Interpolate(f, 0); // f <- F(f)
    Interpolate(g, 0); // g <- G(g)
    for (int i = 0; i < sz; ++i) f[i] *= g[i]; // f <- F(f) * G(g)
    Interpolate(f, 1); // f <- inv(F(f) * G(g))
    for (int i = 0; i < n + m - 1; ++i) std::cout << int(f[i].a / sz + 0.5) << ' ';
    std::cout << std::endl;
    return 0;
}