#include "IO/LeetcodeIO.h"
#include "MISC/Spiral.h"
using namespace std;

/*
[59. 螺旋矩阵 II](https://leetcode.cn/problems/spiral-matrix-ii/)
*/
/**
 * 本题为螺旋矩阵模板题
 */

class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> ans(n, vector<int>(n));
        OY::SpiralSquare S(n);
        for (int i = 0; i < S.size(); i++) {
            auto [r, c] = S[i];
            ans[r][c] = i + 1;
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(generateMatrix);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif