#include "IO/LeetcodeIO.h"
#include "MATH/CantorSequence.h"
using namespace std;

/*
[60. 排列序列](https://leetcode.cn/problems/permutation-sequence/)
*/
/**
 * 本题求无重集的第 k 个排列，需要用到康托逆展开
 */

class Solution {
public:
    string getPermutation(int n, int k) {
        auto vec = OY::get_Cantor_sequence(n, k - 1);
        string ans;
        for (auto id : vec) ans += '1' + id;
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(getPermutation);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif