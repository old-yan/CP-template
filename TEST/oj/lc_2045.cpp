#include "GRAPH/DijkstraKPath.h"
#include "GRAPH/SPFAKPath.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2045. 到达目的地的第二短时间](https://leetcode.cn/problems/second-minimum-time-to-reach-destination/)
*/
/**
 * 本题求严格次短路
 */

class Solution {
    int solve_dijk(int n, vector<vector<int>> &edges, int time, int change) {
        OY::DijkstraKPath::Graph<bool> G(n, edges.size() * 2);
        for (auto &e : edges) G.add_edge(e[0] - 1, e[1] - 1, {}), G.add_edge(e[1] - 1, e[0] - 1, {});

        static int Time, Change;
        Time = time, Change = change;
        struct monoid {
            using value_type = bool;
            using sum_type = int;
            using compare_type = less<int>;
            static sum_type op(const sum_type &x, const value_type &y) {
                auto q = x / (Change * 2), r = x - Change * 2 * q;
                return r < Change ? x + Time : (q + 1) * Change * 2 + Time;
            }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return 0x3f3f3f3f; }
        };
        auto sol = G.calc<2, monoid>(0, n - 1);
        return sol.query(n - 1, 1);
    }
    int solve_spfa(int n, vector<vector<int>> &edges, int time, int change) {
        OY::SPFAKPath::Graph<bool> G(n, edges.size() * 2);
        for (auto &e : edges) G.add_edge(e[0] - 1, e[1] - 1, {}), G.add_edge(e[1] - 1, e[0] - 1, {});

        static int Time, Change;
        Time = time, Change = change;
        struct monoid {
            using value_type = bool;
            using sum_type = int;
            using compare_type = less<int>;
            static sum_type op(const sum_type &x, const value_type &y) {
                auto q = x / (Change * 2), r = x - Change * 2 * q;
                return r < Change ? x + Time : (q + 1) * Change * 2 + Time;
            }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return 0x3f3f3f3f; }
        };
        auto [sol, flag] = G.calc<2, monoid>(0);
        return sol.query(n - 1, 1);
    }

public:
    int secondMinimum(int n, vector<vector<int>> &edges, int time, int change) {
        return solve_spfa(n, edges, time, change);
        // return solve_dijk(n, edges, time, change);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(secondMinimum);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif