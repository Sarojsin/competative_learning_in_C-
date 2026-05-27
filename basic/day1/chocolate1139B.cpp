#include<bits/stdc++.h>
#include<cmath>
using namespace std;
int main(){
int size,max,temp;
cin>>size;
vector<int> v(size);
for(int i=0;i<size;i++){//input the array
    cin>>v[i];
}// core logic;
max=v[size-1];
temp=max;
for(int i=size-2;i>=0;i--){
    if(v[i]< temp){
        max=max+v[i];
        temp=v[i];
    }
    else{
        if(temp==0){
            break;
        }
        else{        
            temp=temp-1;
            max=max+temp;
            }
        }
}
cout<<max<<"\n";
return 0;
}

//deepseek solution
// #include <bits/stdc++.h>
// using namespace std;

// int main() {
//     int n;
//     cin >> n;
//     vector<long long> a(n);
//     for (int i = 0; i < n; ++i) cin >> a[i];
    
//     long long total = a[n-1];
//     long long limit = a[n-1];
    
//     for (int i = n-2; i >= 0; --i) {
//         long long take = min(a[i], limit - 1);
//         if (take < 0) {
//             take = 0;
//         }
//         total += take;
//         limit = take;
//     }
    
//     cout << total << "\n";
//     return 0;
// }