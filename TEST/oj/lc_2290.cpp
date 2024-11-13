#include "DS/LinkBucketHeap.h"
#include "GRAPH/Dijkstra.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2290. 到达角落需要移除障碍物的最小数目](https://leetcode.cn/problems/minimum-obstacle-removal-to-reach-corner/)
*/
/**
 * 本题为 01bfs 模板题
 */

class Solution {
    int solve_dijk(vector<vector<int>> &grid) {
        uint32_t m = grid.size(), n = grid[0].size();
        OY::Dijkstra::Graph<bool> G(m * n, m * n * 4);
        static constexpr std::array<std::array<int, 2>, 4> dirs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
        for (uint32_t i = 0; i != m; i++)
            for (uint32_t j = 0; j != n; j++)
                for (auto [di, dj] : dirs)
                    if (uint32_t i2 = i + di, j2 = j + dj; i2 < m && j2 < n) G.add_edge(i * n + j, i2 * n + j2, grid[i2][j2]);
        using monoid = OY::Dijkstra::AddGroup<bool, uint32_t>;
        auto sol = G.calc<monoid, void, false, OY::LinkBucketHeap01>(0, m * n - 1);
        return sol.query(m * n - 1);
    }
public:
    int minimumObstacles(vector<vector<int>> &grid) {
        return solve_dijk(grid);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minimumObstacles);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif