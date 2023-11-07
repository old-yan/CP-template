#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P5960 【模板】差分约束](https://www.luogu.com.cn/problem/P5960)
*/
/**
 * 本题为差分约束模板题
 * 在 SPFA 求解过程中，需要把所有的点的初始距离都归零，以及入队
 */

static constexpr uint32_t N = 5000, M = 5000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SPFA::Graph<int, N, M> G(n, m);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t a, b;
        int y;
        cin >> a >> b >> y;
        G.add_edge(b - 1, a - 1, y);
    }

    OY::SPFA::Solver<int, false, N> sol(n);
    for (uint32_t i = 0; i < n; i++) sol.set_distance(i, 0);
    if (!sol.run(G))
        cout << "NO";
    else
        for (uint32_t i = 0; i < n; i++) cout << sol.query(i) << ' ';
}