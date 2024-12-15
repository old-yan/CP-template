#include "IO/LeetcodeIO.h"
#include "MISC/Spiral.h"
using namespace std;

/*
[54. 螺旋矩阵](https://leetcode.cn/problems/spiral-matrix/)
*/
/**
 * 本题为螺旋矩阵模板题
 */

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>> &matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> ans(m * n);
        OY::SpiralMatrix S(m, n);
        for (int idx = 0; idx < ans.size(); idx++) {
            auto [i, j] = S[idx];
            ans[idx] = matrix[i][j];
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(spiralOrder);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif