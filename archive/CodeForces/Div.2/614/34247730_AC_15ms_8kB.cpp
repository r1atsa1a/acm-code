#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    double ans = 0;
    cin >> n;
    for (int i = 1; i <= n; i ++) ans += 1.0 / i;
    printf("%.12lf", ans);
}