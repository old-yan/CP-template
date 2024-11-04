#include "GRAPH/BFS.h"
#include "IO/FastIO.h"

#include <array>

/*
[Frogger](https://acm.hdu.edu.cn/showproblem.php?pid=1918)
*/
/**
 * 本题为分层最短路
 */

void solve_bfs() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t lim, m, n;
        cin >> lim >> m >> n;
        std::vector<std::string> grid(m + 2);
        for (auto &s : grid) cin >> s;
        // 由于结点会在有车和没车的状态之间变化，所以需要加入时间维度
        // [r,c,t] 表示过去时间 t 之后的 [r,c] 位置
        // 时间循环周期为 n
        OY::BFS::Graph G((m + 2) * n * n);
        auto make_id = [&](uint32_t i, uint32_t j, uint32_t t) {
            return (i * n + j) * n + t;
        };

        uint32_t source = make_id(m + 1, grid[m + 1].find('F'), 0);
        auto get_char = [&](uint32_t i, uint32_t j, uint32_t t) {
            // 第 m 行，车右行
            if ((i ^ m) & 1) { // 左行
                return j + t < n ? grid[i][j + t] : grid[i][j + t - n];
            } else { // 右行
                return j >= t ? grid[i][j - t] : grid[i][j + n - t];
            }
        };
        static constexpr std::array<std::array<int, 2>, 5> dirs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0}}};
        for (uint32_t t = 0; t != n; t++)
            for (uint32_t i = 0; i != m + 2; i++)
                for (uint32_t j = 0; j != n; j++)
                    if (get_char(i, j, t) != 'X')
                        for (auto &[di, dj] : dirs)
                            if (uint32_t i2 = i + di, j2 = j + dj; i2 < m + 2 && j2 < n && get_char(i2, j2, t + 1) != 'X')
                                G.add_edge(make_id(i, j, t), make_id(i2, j2, t + 1 == n ? 0 : t + 1));
        auto sol = G.calc(source);
        uint32_t ans = sol.infinite();
        for (uint32_t t = 0; t != n; t++)
            ans = std::min(ans, sol.query(make_id(0, grid[0].find('G'), t)));
        if (ans > lim)
            cout << "The problem has no solution.\n";
        else
            cout << "The minimum number of turns is " << ans << ".\n";
    }
}

int main() {
    solve_bfs();
}