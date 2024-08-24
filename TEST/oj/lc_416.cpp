#include "DS/DynamicBitset.h"
#include "DS/StaticBitset.h"
#include "IO/LeetcodeIO.h"
#include "MISC/SubsetSumKnapsack.h"
using namespace std;

/*
[416. 分割等和子集](https://leetcode.cn/problems/partition-equal-subset-sum/)
*/
/**
 * 本题求无重集的第 k 个排列，需要用到康托逆展开
 */

class Solution {
public:
    bool canPartition(vector<int> &nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % 2) return false;
        OY::StaticBitset<10001> S{};
        // OY::DynamicBitset S(sum / 2 + 1);
        OY::SSK::solve_01(S, nums);
        return S[sum / 2];
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(canPartition);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif