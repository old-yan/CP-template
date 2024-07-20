#include "DS/BiTrie.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)
*/
/**
 * 本题求指定异或值的数对数，可以借助 01 字典树
 */

class Solution {
public:
    int countPairs(vector<int> &nums, int low, int high) {
        OY::StaticCountBiTrie32<15, 53000> S;
        int ans = 0;
        for (int a : nums) {
            ans += S.rank_bitxor(a, high + 1) - S.rank_bitxor(a, low);
            S.insert_one(a);
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countPairs);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif