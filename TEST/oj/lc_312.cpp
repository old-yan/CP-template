#include "IO/LeetcodeIO.h"
#include "MISC/CachedLambda.h"
#include "MISC/GridCachedLambda.h"
using namespace std;

/*
[312. 戳气球](https://leetcode.cn/problems/burst-balloons)
*/
/**
 * 本题为记忆化搜索题目
 */

class Solution {
    int solve1(vector<int> &nums) {
        int n = nums.size();
        // dfs(l, r) 表示保持 nums[l] 和 nums[r] 存在的情况下，把 l+1~r-1 都戳破的最大值
        // 为了支持下标 -1，所以参数统一右移 1
        auto dfs = OY::make_DynamicGridCacheSolver<false>(
            [&](auto &&self, int l, int r) -> int {
                int val_l = l ? nums[l - 1] : 1;
                int val_r = r <= n ? nums[r - 1] : 1;
                int res = 0;
                for (int last = l + 1; last < r; last++)
                    res = max(res, self(self, l, last) + self(self, last, r) + val_l * nums[last - 1] * val_r);
                return res;
            },
            n + 1, n + 2);
        return dfs.query(0, n + 1);
    }
    int solve2(vector<int> &nums) {
        int n = nums.size();
        // dfs(l, r) 表示保持 nums[l] 和 nums[r] 存在的情况下，把 l+1~r-1 都戳破的最大值
        auto dfs = OY::make_CacheSolver<false, 1 << 17>(
            [&](auto &&self, int l, int r) -> int {
                int val_l = ~l ? nums[l] : 1;
                int val_r = r < n ? nums[r] : 1;
                int res = 0;
                for (int last = l + 1; last < r; last++)
                    res = max(res, self(self, l, last) + self(self, last, r) + val_l * nums[last] * val_r);
                return res;
            });
        return dfs.query(-1, n);
    }
    int solve3(vector<int> &_nums) {
        static vector<int> nums;
        static int n;
        // dfs(l, r) 表示保持 nums[l] 和 nums[r] 存在的情况下，把 l+1~r-1 都戳破的最大值
        // 为了支持下标 -1，所以参数统一右移 1
        static auto dfs = OY::make_StaticGridCacheSolver<true, 301, 302>(
            [](auto &&self, int l, int r) -> int {
                int val_l = l ? nums[l - 1] : 1;
                int val_r = r <= n ? nums[r - 1] : 1;
                int res = 0;
                for (int last = l + 1; last < r; last++)
                    res = max(res, self(self, l, last) + self(self, last, r) + val_l * nums[last - 1] * val_r);
                return res;
            });
        nums = _nums;
        n = nums.size();
        dfs.cache_clear();
        return dfs.query(0, n + 1);
    }

public:
    int maxCoins(vector<int> &nums) {
        return solve1(nums);
        // return solve2(nums);
        // return solve3(nums);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maxCoins);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif