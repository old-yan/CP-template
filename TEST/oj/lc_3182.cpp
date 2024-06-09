#include "DS/DynamicBitset.h"
#include "DS/StaticBitset.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[3182. 执行操作可获得的最大总奖励 II](https://leetcode.cn/problems/maximum-total-reward-using-operations-ii/)
*/
/**
 * 本题要做的操作是将 bitset 的某个范围的 1 左移，实现背包
 * 可以调用 bitor_lshift 方法
 */

class Solution {
public:
    int maxTotalReward(vector<int> &rewardValues) {
        sort(rewardValues.begin(), rewardValues.end());
        rewardValues.erase(unique(rewardValues.begin(), rewardValues.end()), rewardValues.end());
        // OY::DynamicBitset a(rewardValues.back() * 2);
        OY::StaticBitset<100001> a{};
        a.set(0);
        for (auto e : rewardValues) a.bitor_lshift(e, e);
        return a.last_one();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maxTotalReward);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif
