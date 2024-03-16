#include "IO/LeetcodeIO.h"
#include "MATH/CatalanNumber.h"
#include "MATH/StaticModInt64.h"
using namespace std;

/*
[96. 不同的二叉搜索树](https://leetcode.cn/problems/unique-binary-search-trees/)
*/
/**
 * 本题求可能的括号序列数量，为卡特兰数
 */

using mint = OY::mint9223372036854775783;
OY::CatalanNumber<mint, 19> table;

class Solution {
public:
    int numTrees(int n) {
        return table[n].val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(numTrees);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif