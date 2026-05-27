#include<bits/stdc++.h>
#include<vector>
using namespace std;
int main(){
int inst,days;
cin>>inst>>days;
vector<pair<int,int>> v;
for(int i=1;i<=inst;i++){
    int temp;
    cin>>temp;
    v.push_back({temp,i});
}
sort(v.begin(),v.end());
//core logic
int count=0;
int temp=days;
for(int i=0;i<inst;i++){
    if(v[i].first<=temp){
        count++;
        temp-=v[i].first;
    }
    else{
        break;
    }
}
cout<<count<<"\n";
// It does not sort the output indices, which may cause false negatives on strict judges.
// To be safe, store chosen indices in a separate vector and sort them before printing:
vector<int> chosen_indices;
for (int i = 0; i < count; i++)
{
    chosen_indices.push_back(v[i].second);
}
sort(chosen_indices.begin(), chosen_indices.end());
for (int i = 0; i < chosen_indices.size(); i++)
{
    cout << chosen_indices[i] << " ";
}
return 0;
}