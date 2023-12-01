#include "IO/LeetcodeIO.h"
#include "MATH/CantorSequence_ex.h"
using namespace std;

/*
[1643. 第 K 条最小指令](https://leetcode.cn/problems/kth-smallest-instructions/)
*/
/**
 * 本题求可重集的第 k 个排列，需要用到康托逆展开
 */

class Solution {
public:
    string kthSmallestPath(vector<int> &destination, int k) {
        string s = string(destination[0], 'V') + string(destination[1], 'H');
        auto vec = OY::get_Cantor_sequence_ex(s.begin(), s.end(), k - 1);
        return string(vec.begin(), vec.end());
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(kthSmallestPath);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif