#include<bits/stdc++.h>
using namespace std;
int main(){
// int n;
// cin>>n;
// vector<int> num;
// //old method
// while(n>0){
//     num.push_back(n%10);
//     n=n/10;
// }
// for(int i = 0; i < num.size(); i++)
// {
//     cout<<num[i]<<" ";
// }
// cout<<"\n";
// reverse(num.begin(),num.end());
// for(int itr:num){
//     cout<<itr<<" ";
// }
// cout<<endl;
// for (int i = 0; i < num.size(); i++)
// {
//     if(num[i]>=5 && num[i]!=9){
//         num[i]=9-num[i];
//     }
// }
// for (int i = 0; i < num.size(); i++)
// {
//     cout<<num[i];
// }
string s;
int y;
cin>>s;
cout<<s.size()<<"\n";
for (int i = 0; i <s.size(); i++)
{
   char ch=s[i];
   int num=ch-'0';
   if(num>=5 && num!=9){
        y=9-num;
   }
   cout<<min(num,y)<<" ";

}
cout<<"\n";
return 0;
}