#include "GRAPH/Dijkstra.h"
#include "GRAPH/SPFA.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[3123. 最短路径中的边](https://leetcode.cn/problems/find-edges-in-shortest-paths/)
*/
/**
 * 最短路模板
 * 求可能在最短路上的边
 */

class Solution {
    vector<bool> solve_dijk(int n, vector<vector<int>> &edges) {
        OY::Dijkstra::Graph<int> G(n, edges.size() * 2);
        for (auto &e : edges) {
            int a = e[0], b = e[1], c = e[2];
            G.add_edge(a, b, c);
            G.add_edge(b, a, c);
        }

        auto sol1 = G.calc(0, n - 1);
        auto sol2 = G.calc(n - 1, 0);
        vector<bool> ans;
        for (auto &e : edges) {
            int a = e[0], b = e[1], c = e[2];
            if (sol1.query(a) + sol2.query(b) == sol1.query(n - 1) - c)
                ans.push_back(true);
            else if (sol1.query(b) + sol2.query(a) == sol1.query(n - 1) - c)
                ans.push_back(true);
            else
                ans.push_back(false);
        }
        return ans;
    }
    vector<bool> solve_spfa(int n, vector<vector<int>> &edges) {
        OY::SPFA::Graph<int> G(n, edges.size() * 2);
        for (auto &e : edges) {
            int a = e[0], b = e[1], c = e[2];
            G.add_edge(a, b, c);
            G.add_edge(b, a, c);
        }

        auto sol1 = G.calc(0).first;
        auto sol2 = G.calc(n - 1).first;
        vector<bool> ans;
        for (auto &e : edges) {
            int a = e[0], b = e[1], c = e[2];
            if (sol1.query(a) + sol2.query(b) == sol1.query(n - 1) - c)
                ans.push_back(true);
            else if (sol1.query(b) + sol2.query(a) == sol1.query(n - 1) - c)
                ans.push_back(true);
            else
                ans.push_back(false);
        }
        return ans;
    }

public:
    vector<bool> findAnswer(int n, vector<vector<int>> &edges) {
        return solve_spfa(n, edges);
        // return solve_dijk(n, edges);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(findAnswer);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif