#include<bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin>>s;
    set<char> unique_chars(s.begin(), s.end());
    if(unique_chars.size()%2==0){
        cout<<"CHAT WITH HER!\n";
    }
    else{
        cout<<"IGNORE HIM!\n";
    }
return 0;
}