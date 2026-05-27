#include <iostream>
#include <string>
using namespace std;

int main() {
    int n;
    string s;
    cin >> n >> s;
    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        int digit = s[i] - '0';
        if (digit % 2 == 0) {
            ans += (i + 1); // number of substrings ending at i
        }
    }
    cout << ans << endl;
    return 0;
}