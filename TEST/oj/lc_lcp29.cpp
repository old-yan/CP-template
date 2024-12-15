#include "IO/LeetcodeIO.h"
#include "MISC/Spiral.h"
using namespace std;

/*
[LCP 29. 乐团站位](https://leetcode.cn/problems/SNJvJP/)
*/
/**
 * 本题为螺旋矩阵模板题
 */

class Solution {
public:
    int orchestraLayout(int num, uint32_t xPos, uint32_t yPos) {
        OY::SpiralSquare S(num);
        auto rank = S.index_of({xPos, yPos});
        return rank % 9 + 1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(orchestraLayout);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif