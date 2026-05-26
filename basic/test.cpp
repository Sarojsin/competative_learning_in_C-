#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;
int main(){
int a=5;
cout<<a<<"\n";
cout<<"Hello World"<<"\n";

    set<int> s;   // Just to show that sets work with any type, including pairs and maps.
    s.insert(1);
    s.insert(10);  
    s.insert(20);
    s.insert(30);
    s.insert(20);  // duplicate, will not be added again
    cout << "Set contains: ";
    for (int x : s) {
        cout << x << " ";
    }
    cout << "\n";
    
    map<int, string> mp;   // Just to show that vectors work with any type, including pairs and maps.
    // mp["key"] = "value";
    // cout << "mp[\"key\"] = " << mp["key"] << "\n";
    mp[1] = "value2";
    mp[2] = "value3";
    mp[3] = "value4";
    cout << "mp[1] = " << mp[1] << "\n";
    for(auto it: mp){
        cout<<it.first<<" "<<it.second<<"\n";
    }
return 0;
}