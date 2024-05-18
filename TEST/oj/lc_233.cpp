#include "IO/LeetcodeIO.h"
#include "MISC/CachedLambda.h"
using namespace std;

/*
[233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one)
*/
/**
 * 本题为记忆化搜索题目
 */

class Solution {
public:
    int countDigitOne(int n) {
        auto dfs = OY::make_CacheSolver<false, 1 << 10>(
            [](auto &&self, int x) -> int {
                // 考虑 357x 。0~3569 是 0~356 重复十次
                int q = x / 10, r = x % 10;
                int ans1 = self(self, q - 1) * 10 + q;
                int ans2 = (self(self, q) - self(self, q - 1)) * (r + 1) + (r >= 1);
                return ans1 + ans2;
            });
        for (int i = 0; i < 10; i++)
            dfs.set_initial_value(i, i >= 1);
        return dfs.query(n);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countDigitOne);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif