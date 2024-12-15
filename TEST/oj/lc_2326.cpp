#include "IO/LeetcodeIO.h"
#include "MISC/Spiral.h"
using namespace std;

/*
[2326. 螺旋矩阵 IV](https://leetcode.cn/problems/spiral-matrix-iv/)
*/
/**
 * 本题为螺旋矩阵模板题
 */

class Solution {
public:
    vector<vector<int>> spiralMatrix(int m, int n, ListNode *head) {
        vector<vector<int>> ans(m, vector<int>(n, -1));
        OY::SpiralMatrix S(m, n);
        int idx = 0;
        for (auto it = head; it; it = it->next) {
            auto [i, j] = S[idx++];
            ans[i][j] = it->val;
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(spiralMatrix);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif