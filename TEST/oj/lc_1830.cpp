#include "IO/LeetcodeIO.h"
#include "MATH/CantorRank_ex.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"
using namespace std;

/*
[1830. 使字符串有序的最少操作次数](https://leetcode.cn/problems/minimum-number-of-operations-to-make-string-sorted/)
*/
/**
 * 本题求可重集的某个排列在所有排列里的排名，需要用到康托展开
 */

using mint = OY::mint1000000007;
// using mint=OY::mgint1000000007;
OY::CantorRank_ex<mint, 3000> cr;
class Solution {
public:
    int makeStringSorted(string s) {
        return cr.query(s.begin(), s.end()).val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(makeStringSorted);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif