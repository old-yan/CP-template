#include "GRAPH/Psuedotree_ig.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2359. 找到离给定两个节点最近的节点](https://leetcode.cn/problems/find-closest-node-to-given-two-nodes/)
*/
/**
 * 本题可以当基环树处理
 * 没出度就给他拉自环
 */

class Solution {
public:
    int closestMeetingNode(vector<int> &edges, int node1, int node2) {
        int n = edges.size();
        OY::PsuedoIG::Graph<bool, uint32_t, true> G(n);
        for (int i = 0; i < n; i++) G.set_edge(i, ~edges[i] ? edges[i] : i);
        G.prepare();
        if (G.get_vertex_info(node1).cycle_id() == G.get_vertex_info(node2).cycle_id()) {
            auto cycle = G.get_cycle_info(G.get_vertex_info(node1).cycle_id());
            int i1 = G.get_vertex_info(node1).cycle_join();
            int i2 = G.get_vertex_info(node2).cycle_join();
            if (i1 == i2) return G.join(node1, node2);
            int dis1 = G.get_vertex_info(node1).step_to_cycle();
            int dis2 = G.get_vertex_info(node2).step_to_cycle();
            int dis12 = i1 < i2 ? cycle.position(i2) - cycle.position(i1) : cycle.length() + cycle.position(i2) - cycle.position(i1);
            int dis21 = cycle.length() - dis12;
            int min_dis = min(max(dis1 + dis12, dis2), max(dis1, dis2 + dis21));
            uint32_t ans = 0x3f3f3f3f;
            if (min_dis == max(dis1 + dis12, dis2)) ans = min(ans, cycle[i2]);
            if (min_dis == max(dis1, dis2 + dis21)) ans = min(ans, cycle[i1]);
            return ans;
        } else
            return -1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(closestMeetingNode);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif