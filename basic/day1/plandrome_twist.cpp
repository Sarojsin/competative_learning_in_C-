// #include<bits/stdc++.h>
// using namespace std;

// bool canmake(char a, char b) {
//     int diff = abs(a - b);
//     return (diff == 0 || diff == 2);
// }

// void check(vector<char> v) {
//     int n = v.size();
//     bool possible = true;
//     for (int i = 0; i < n / 2; i++) {
//         if (!canmake(v[i], v[n - 1 - i])) {
//             possible = false;
//             break;
//         }
//     }
//     cout << (possible ? "YES" : "NO") << "\n";
// }

// int main() {
//     int t;
//     cin >> t;
//     while (t--) {
//         int n;
//         vector<char> v;
//         cin >> n;
//         for (int i = 0; i < n; i++) {
//             char x;
//             cin >> x;
//             v.push_back(x);
//         }
//         check(v);
//     }
//     return 0;
// }

//deepseek code
#include<bits/stdc++.h>
using namespace std;

int main() {
    int t;
    cin >> t;
    while(t--) {
        int n;
        string s;
        cin >> n >> s;
        bool possible = true;
        for(int i = 0; i < n/2; i++) {
            int diff = abs(s[i] - s[n-1-i]);
            if(diff != 0 && diff != 2) {
                possible = false;
                break;
            }
        }
        cout << (possible ? "YES" : "NO") << "\n";
    }
    return 0;
}