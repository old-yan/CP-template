#include "IO/LeetcodeIO.h"
#include "MATH/FastTransform.h"
using namespace std;

/*
[982. 按位与为零的三元组](https://leetcode.cn/problems/triples-with-bitwise-and-equal-to-zero/)
*/
/**
 * 本题可以用卷积秒杀
 */

class Solution {
public:
    int countTriplets(vector<int> &nums) {
        vector<int64_t> cnt(1 << 16);
        for (auto a : nums) cnt[a]++;
        OY::FASTTRANS::fast_bitand_transform<true>(cnt.begin(), cnt.end());
        for (auto &a : cnt) a *= a * a;
        OY::FASTTRANS::fast_bitand_transform<false>(cnt.begin(), cnt.end());
        return cnt[0];
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countTriplets);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif