#include "DS/CatTree.h"
#include "DS/RollbackSqrtTree.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[3117. 划分数组得到最小的值之和](https://leetcode.cn/problems/minimum-sum-of-values-by-dividing-array/)
*/
/**
 * 本题为经典线段树优化 dp
 * 由于每层的线段树都是从左往右填充，所以可以换成回滚根树优化复杂度
 */

class Solution {
public:
    int minimumValueSum(vector<int> &nums, vector<int> &andValues) {
        using BitAndTree = OY::CAT::Table<OY::CAT::CustomNode<int, bit_and<int>>, 1 << 24>;
        using Vector = OY::RollbackSqrtMinTable<int, OY::RollbackSqrt::RandomController<5>, 11>;
        int n = nums.size();
        BitAndTree S(nums.begin(), nums.end());
        Vector dp, dp2;
        dp.reserve(n), dp2.reserve(n);
        for (int i = 0; i < n; i++)
            if (S.query(0, i) == andValues[0])
                dp.push_back(nums[i]);
            else
                dp.push_back(999999999);
        for (int j = 1; j < andValues.size(); j++) {
            while (dp2.size()) dp2.pop_back();
            // l1 维护满足 S.query(l, r) < target 的 l 的临界点
            // l2 维护满足 S.query(l, r) <= target 的 l 的临界点
            // [l1+1, l2] 满足 S.query(l, r) == target
            for (int l1 = 0, l2 = -1, r = 0; r < n; r++) {
                while (l1 < r and S.query(l1 + 1, r) < andValues[j]) l1++;
                while (l2 < r and S.query(l2 + 1, r) <= andValues[j]) l2++;
                if (l1 < l2)
                    dp2.push_back(dp.query(l1, l2 - 1) + nums[r]);
                else
                    dp2.push_back(999999999);
            }
            swap(dp, dp2);
        }
        return dp.query(nums.size() - 1) >= 999999999 ? -1 : dp.query(nums.size() - 1);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minimumValueSum);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif