#include "GRAPH/Psuedotree_ig.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2127. 参加会议的最多员工数](https://leetcode.cn/problems/maximum-employees-to-be-invited-to-a-meeting/)
*/
/**
 * 基环树模板题
 * 要想在圆桌上，每个人都能挨着自己的后继
 * 只有环以及依附于环的树边才能满足条件
 * 环长大于 2 时，环独占整个餐桌
 * 环长等于 2 时，环可以带个最长树链
 */

class Solution {
public:
    int maximumInvitations(vector<int> &favorite) {
        int n = favorite.size();
        OY::PsuedoIG::Graph<bool> G(n);
        for (int i = 0; i != n; i++) G.set_edge(i, favorite[i]);
        G.prepare();

        // 先求一下环上的每个点，在环外的尾巴能有多长
        vector<uint32_t> out_of_cycle(n);
        for (int i = 0; i != n; i++) {
            auto info = G.get_vertex_info(i);
            auto cycle = G.get_cycle_info(info.cycle_id());
            auto j = cycle[info.cycle_join()];
            out_of_cycle[j] = max(out_of_cycle[j], info.step_to_cycle());
        }

        uint32_t cycle1 = 0, cycle2 = 0;
        for (int i = 0; i != G.cycle_count(); i++) {
            auto cycle = G.get_cycle_info(i);
            if (cycle.size() == 2) cycle2 += 2 + out_of_cycle[cycle[0]] + out_of_cycle[cycle[1]];
            cycle1 = max(cycle1, cycle.size());
        }
        return max(cycle1, cycle2);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maximumInvitations);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif