#include <bits/stdc++.h>

using namespace std;
using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    n += 2;
    if (n >= 8) n -= 7;

    cout << n << '\n';

    return 0;
}