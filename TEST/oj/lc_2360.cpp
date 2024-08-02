#include "GRAPH/Psuedotree_ig.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2360. 图中的最长环](https://leetcode.cn/problems/longest-cycle-in-a-graph/)
*/
/**
 * 本题可以当基环树处理
 * 没出度就给他拉自环
 * 注意，有可能原图没环，而我们拉的自环导致答案为 1
 * 所以需要特判一下原图有没有真正的自环
 */

class Solution {
public:
    int longestCycle(vector<int> &edges) {
        int n = edges.size();
        OY::PsuedoIG::Graph<bool> G(n);
        bool self_loop = false;
        for (int i = 0; i != n; i++) {
            G.set_edge(i, ~edges[i] ? edges[i] : i);
            if (edges[i] == i) self_loop = true;
        }
        G.prepare();
        uint32_t ans = 0;
        for (int i = 0; i < G.cycle_count(); i++)
            ans = max(ans, G.get_cycle_info(i).size());
        return ans > 1 ? ans : (self_loop ? 1 : -1);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(longestCycle);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif