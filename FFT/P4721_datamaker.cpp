#include <iostream>
#include <random>
#include <chrono>
using namespace std;
signed main() {
    int n = 100; cout << n << endl;
    int maxn = 10;
    int seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 rnd(seed);
    for (int i = 1; i < n; ++i) {
        cout << rnd() % maxn << ' ';
    }
    cout << endl;
    return 0;
}