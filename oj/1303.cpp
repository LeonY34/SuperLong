#include <iostream>
#include <vector>

const int B = 1000000;

bool trydel(std::vector<int>& c, int x) {
    if (x == 0) {
        for (int i = -B; i <= B; ++i) {
            if (c[i + B] & 1) {
                for (int j = i - 1; j >= -B; --j) {
                    c[j + B] <<= 1;
                }
                return false;
            }
            c[i + B] >>= 1;
        }
    } else {
        if (x > 0) {
            for (int i = -B + x; i <= B; ++i) {
                c[i + B] -= c[i - x + B];
                if (c[i + B] < 0 || (i == 0 && c[i + B] == 0)) {
                    for (int j = i; j >= -B + x; --j) {
                        c[j + B] += c[j - x + B];
                    }
                    return false;
                }
            }
        } else {
            for (int i = B + x; i >= -B; --i) {
                c[i + B] -= c[i - x + B];
                if (c[i + B] < 0 || (i == 0 && c[i + B] == 0)) {
                    for (int j = i; j <= B + x; ++j) {
                        c[j + B] += c[j - x + B];
                    }
                    return false;
                }
            }
        }
    }
    return true;
}

int find(const std::vector<int>& c) {
    int i = -B;
    int j = B * 2;
    for (; i <= B; ++i) {
        if (c[i + B]) {
            if (j == B * 2) {
                j = i;
                if (c[i + B] > 1) break;
            }
            else break;
        }
    }
    return i - j;
}

signed main() {
    int n; std::cin >> n;
    std::vector<int> c(B * 2 + 1);
    for (int i = 0; i < (1 << n) - 1; ++i) {
        int s; std::cin >> s;
        c[s + B]++;
    }
    c[0 + B]++;
    std::vector<int> ans; ans.reserve(n);
    for (int i = 0; i < n; ++i) {
        int now = find(c);
        // printf("now = %d\n", now);
        // for (int j = -B; j <= B; ++j) {
        //     if (c[j + B]) {
        //         printf("c[%d] = %d\n", j, c[j + B]);
        //     }
        // }
        if (trydel(c, now)) ans.push_back(now);
        else if (trydel(c, -now)) ans.push_back(-now);
        else break;
    }
    if (int(ans.size()) == n) {
        std::cout << "YES" << std::endl;
        for (int i = 0; i < n; ++i) std::cout << ans[i] << ' ';
        std::cout << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
