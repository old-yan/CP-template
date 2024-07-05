#include "GRAPH/Steiner.h"
#include "IO/FastIO.h"

/*
[P4294 [WC2008] 游览计划](https://www.luogu.com.cn/problem/P4294)
*/
// 本题显然为点权和的最小斯坦纳树模板

static constexpr uint32_t M = 10, N = 10, K = 10;
uint32_t grid[M * N];
void solve_steiner() {
    uint32_t m, n;
    cin >> m >> n;
    for (uint32_t i = 0; i < m; i++)
        for (uint32_t j = 0; j < n; j++) cin >> grid[i * n + j];

    OY::STEINER::Graph<void, uint32_t> G(m * n, m * n * 2);
    for (uint32_t i = 0; i < m; i++)
        for (uint32_t j = 0; j < n; j++) {
            if (i) G.add_edge(i * n + j, (i - 1) * n + j);
            if (j) G.add_edge(i * n + j, i * n + j - 1);
            if (!grid[i * n + j])
                G.set_key(n * i + j);
            else
                G.set_value(n * i + j, grid[i * n + j]);
        }
    if (auto cnt = std::count_if(G.m_is_key.begin(), G.m_is_key.end(), [](auto v) { return v; })) {
        auto res = G.calc_spfa<true>();
        auto &&sol = res.first;

        // 输出方案
        sol.do_for_used_edges([&](uint32_t index) {
            auto [from, to] = G.m_edges[index];
            if (grid[from]) grid[from] = -1;
            if (grid[to]) grid[to] = -1;
        });
        cout << sol.total_cost() << endl;
    } else
        cout << "0\n";
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            uint32_t c = grid[i * n + j];
            if (!c)
                cout << 'x';
            else if (c == -1)
                cout << 'o';
            else
                cout << '_';
        }
        cout << endl;
    }
}

int main() {
    solve_steiner();
}