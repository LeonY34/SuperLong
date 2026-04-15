#include <iostream>
#include <cstring>

int a[1024 + 1];
int b[1024 + 1];
int query(int x, int y);

void merge(int l, int r) {
    if (l + 1 == r) return;
    int m = (l + r) >> 1;
    merge(l, m); merge(m, r);
    int i = l, j = m, tot = 0;
    while (i < m && j < r) {
        if (query(a[i], a[j]) != 1) b[tot++] = a[i++];
        else b[tot++] = a[j++];
    }
    while (i < m) b[tot++] = a[i++];
    while (j < r) b[tot++] = a[j++];
    memcpy(a + l, b, tot * sizeof(int));
}

void optimal_sort(int n) {
    for (int i = 1; i <= n; i++) a[i] = i;
    merge(1, n + 1);
    for (int i = 1; i <= n; i++) printf("%d%c", a[i], i == n ? '\n' : ' ');
}

// int __qcnt = 0;
// int __hidden_array[1024 + 1];
// int query (int x, int y) {
//     __qcnt ++;
//     int d = __hidden_array[x] - __hidden_array[y];
//     return d > 0 ? 1 : (d == 0 ? 0 : -1);
// }
// int main() {
//     int n;
//     std::cin >> n;
//     for (int i = 1; i <= n; i++) std::cin >> __hidden_array[i];
//     optimal_sort(n);
//     std::cout << "number of queries : " << __qcnt << std::endl;
// }