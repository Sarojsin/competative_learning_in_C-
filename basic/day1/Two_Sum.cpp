#include<bits/stdc++.h>
using namespace std;
int main(){
    int target;
    cin>>target;
vector<int> nums ;
int n;
cin>>n;
for(int i=0;i<n;i++){
    int x;
    cin>>x;
    nums.push_back(x);
}
for(int i=0;i<nums.size();i++){
    for(int j=i+1;j<n;j++){
        if(nums[i]+nums[j]==target){
            cout<<i<<" "<<j<<"\n";
            return 0;
        }
    }
}
return 0;
}
// #include <vector>
// using namespace std;

// class Solution {
// public:
//     vector<int> twoSum(vector<int>& nums, int target) {
//         for (int i = 0; i < nums.size(); i++) {
//             for (int j = i + 1; j < nums.size(); j++) {
//                 if (nums[i] + nums[j] == target) {
//                     return {i, j};   // return indices as a vector
//                 }
//             }
//         }
//         return {}; // No solution found (should not happen per problem statement)
//     }
// };