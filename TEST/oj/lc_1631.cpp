#include "GRAPH/Dijkstra.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1631. 最小体力消耗路径](https://leetcode.cn/problems/path-with-minimum-effort/)
*/
/**
 * 最短路模板
 */

class Solution {
    int solve_dijk(vector<vector<int>> &heights) {
        int m = heights.size();
        int n = heights[0].size();
        OY::Dijkstra::Graph<int> G(m * n, (m - 1) * (n - 1));
        static constexpr std::array<std::array<int, 2>, 4> dirs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                for (auto [di, dj] : dirs) {
                    int i2 = i + di, j2 = j + dj;
                    if (i2 >= 0 and i2 < m and j2 >= 0 and j2 < n) {
                        G.add_edge(i * n + j, i2 * n + j2, abs(heights[i][j] - heights[i2][j2]));
                    }
                }
            }
        auto sol = G.calc<OY::Dijkstra::MaxGroup<int>>(0, m * n - 1);
        return sol.query(m * n - 1);
    }

public:
    int minimumEffortPath(vector<vector<int>> &heights) {
        return solve_dijk(heights);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minimumEffortPath);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif