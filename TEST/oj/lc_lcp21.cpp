#include "GRAPH/BFS.h"
#include "GRAPH/Psuedotree_ug.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[LCP 21. 追逐游戏](https://leetcode.cn/problems/Za25hA/)
*/
/**
 * 本题为基环树题目
 * 做分类讨论
 */

class Solution {
public:
    int chaseGame(vector<vector<int>> &edges, int startA, int startB) {
        int n = edges.size();
        OY::PsuedoUG::Graph<bool, uint32_t, true> G(n);
        for (auto &e : edges) G.add_edge(e[0] - 1, e[1] - 1);
        G.prepare();
        auto cycle = G.get_cycle_info(0);
        auto info_a = G.get_vertex_info(--startA);
        auto info_b = G.get_vertex_info(--startB);

        int i1 = info_a.cycle_join();
        int i2 = info_b.cycle_join();
        int dis1_to_cycle = info_a.step_to_cycle();
        int dis2_to_cycle = info_b.step_to_cycle();
        int dis1_to_2 = i1 <= i2 ? cycle.position(i2) - cycle.position(i1) : cycle.length() + cycle.position(i2) - cycle.position(i1);
        dis1_to_2 = min<int>(dis1_to_2, cycle.length() - dis1_to_2);
        if (cycle.length() <= 3 || dis1_to_cycle + dis1_to_2 <= dis2_to_cycle + 1) {
            // 如果 a 能在 b 到环上之前，到达环的入口。那么 b 无法入环，b 一定会被抓
            // 而环太小，也无法玩秦王绕柱
            // 可以发现，a 追踪 b 的路径一定不会走冤枉路
            // 所以从 a 出发 bfs，从 b 出发 bfs，找离 a 远的点就可以充当逃跑点
            OY::BFS::Solver<false> sol_a(n), sol_b(n);
            sol_a.set_distance(startA, 0);
            sol_b.set_distance(startB, 0);
            auto traverser = [&](int from, auto &&call) {
                G.do_for_edges(from, call);
            };
            sol_a.run(traverser);
            sol_b.run(traverser);
            int ans = 1;
            for (int i = 0; i < n; i++)
                if (sol_a.query(i) > sol_b.query(i) + 1) ans = max<int>(ans, sol_a.query(i));
            return ans;
        } else
            // 如果 a 不能在 b 到环上之前，到达环的入口。那么 a b 一定会在环上秦王绕柱
            return -1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(chaseGame);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif