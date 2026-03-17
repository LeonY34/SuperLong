#include <iostream>
#include "SuperLong.hpp"

signed main() {
    int n, case_num;
    std::cin >> n >> case_num;
    for (int i = 0; i < n; ++i) {
        if (case_num == 1) { // 大整数+小整数
            SuperLong a; SHF b;
            std::cin >> a >> b;
            a += b;
            std::cout << a << std::endl;
        } else if (case_num == 2) { // 大整数+大整数
            SuperLong a, b;
            std::cin >> a >> b;
            a += b;
            std::cout << a << std::endl;
        } else if (case_num == 3) { // 大整数-小整数
            SuperLong a; SHF b;
            std::cin >> a >> b;
            a -= b;
            std::cout << a << std::endl;
        } else if (case_num == 4) { // 大整数-大整数
            SuperLong a, b;
            std::cin >> a >> b;
            a -= b;
            std::cout << a << std::endl;
        } else if (case_num == 5) { // 大整数*小整数
            SuperLong a; SHF b;
            std::cin >> a >> b;
            a *= b;
            std::cout << a << std::endl;
        } else if (case_num == 6) { // 大整数*大整数
            SuperLong a, b;
            std::cin >> a >> b;
            a *= b;
            std::cout << a << std::endl;
        } else if (case_num == 7) { // 大整数/小整数
            SuperLong a; SHF b;
            std::cin >> a >> b;
            a /= b;
            std::cout << a << std::endl;
        } else if (case_num == 8) { // 大整数/大整数
            SuperLong a, b;
            std::cin >> a >> b;
            a /= b;
            std::cout << a << std::endl;
        } else if (case_num == 9) { // 大整数%小整数
            SuperLong a; SHF b;
            std::cin >> a >> b;
            a %= b;
            std::cout << a << std::endl;
        } else if (case_num == 10) { // 大整数%大整数
            SuperLong a, b;
            std::cin >> a >> b;
            a %= b;
            std::cout << a << std::endl;
        } else if (case_num == 11) { // 大整数<<x
            SuperLong a; size_t x;
            std::cin >> a >> x;
            a <<= x;
            std::cout << a << std::endl;
        } else if (case_num == 12) { // 大整数>>x
            SuperLong a; size_t x;
            std::cin >> a >> x;
            a >>= x;
            std::cout << a << std::endl;
        }
    }
    return 0;
}

/*
g++ -std=c++17 main.cpp SuperLong.cpp -o build/superlong && ./build/superlong
*/