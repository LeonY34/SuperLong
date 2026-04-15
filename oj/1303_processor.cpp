#include <iostream>
#include <vector>

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i];
    std::vector<int> b((1<<n)-1);
    for (int i = 1; i < (1 << n); ++i) {
        b[i - 1] = 0;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) b[i - 1] += a[j];
        }
    }
    std::cout << n << std::endl;
    for (int i = 0; i < (1 << n) - 1; ++i) std::cout << b[i] << ' ';
    std::cout << std::endl;
    return 0;
}