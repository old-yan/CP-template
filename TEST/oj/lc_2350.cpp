#include "IO/LeetcodeIO.h"
#include "STR/SequenceAutomaton.h"
using namespace std;

/*
[2350. 不可能得到的最短骰子序列](https://leetcode.cn/problems/shortest-impossible-sequence-of-rolls)
*/
/**
 * 本题可以借助序列自动机进行贪心计算
 */

class Solution {
public:
    int shortestSequence(vector<int> &rolls, int k) {
        OY::SequenceAutomaton_vector sa(rolls.begin(), rolls.end());
        int cur_pos = -1, len = 0;
        while (true) {
            int far = cur_pos;
            for (int i = 1; i <= k; i++) far = max(far, int(sa.next(cur_pos, i)));
            cur_pos = far, len++;
            if (far >= rolls.size()) break;
        }
        return len;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(shortestSequence);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif