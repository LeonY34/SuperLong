#include <map>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
signed main() {
    int n; cin >> n;
    vector<string> a(n);
    map<string, string> mp;
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) {
        string b; cin >> b;
        if (mp.count(a[i])) {
            if (b != mp[a[i]]) {
                cout << "No" << endl;
                return 0;
            }
        } else {
            mp[a[i]] = b;
        }
    }
    cout << "Yes" << endl;
}