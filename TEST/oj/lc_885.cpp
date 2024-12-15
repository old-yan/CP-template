#include "IO/LeetcodeIO.h"
#include "MISC/Spiral.h"
using namespace std;

/*
[885. 螺旋矩阵 III](https://leetcode.cn/problems/spiral-matrix-iii/)
*/
/**
 * 本题为螺旋矩阵模板题
 * 我们习惯的模板，是从外而内，顺时针遍历；
 * 本题要求，从内而外，顺时针遍历。
 * 所以，要做一个水平的镜像转换
 *
 * 由于起点为一个点，然后开始画圈，所以我们最好整一个方阵
 * 方阵边长为 a+1
 *
 * 原行号 rstart -> 现行号 a
 * 原行号 i -> 现行号 a+i-rstart
 * 现行号 i -> 原行号 rstart+i-a
 *
 * 原列 cstart -> 现列号 a
 * 原列号 j -> 现列号 a+cstart-j
 * 现列号 j -> 原列号 a+cstart-j
 *
 * 所以
 * 原行号范围 0~rows-1 对应现行号 a-rstart~a+rows-1-rstart
 * 原列号范围 0~cols-1 对应现列号 a+cstart-cols+1~a+cstart
 *
 */

class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        int a = max({rStart, cStart, rows - rStart - 1, cols - cStart - 1});
        OY::SpiralSquare S(a * 2 + 1);
        vector<vector<int>> ans;
        ans.reserve(rows * cols);
        int row_floor = a - rStart;
        int row_ceil = a + rows - 1 - rStart;
        int col_floor = a + cStart - cols + 1;
        int col_ceil = a + cStart;
        for (int idx = S.size() - 1; ~idx; idx--) {
            auto [i, j] = S[idx];
            if (i >= row_floor && i <= row_ceil && j >= col_floor && j <= col_ceil) {
                ans.push_back({rStart + int(i) - a, a + cStart - int(j)});
            }
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(spiralMatrixIII);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif