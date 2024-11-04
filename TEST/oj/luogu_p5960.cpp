#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P5960 【模板】差分约束](https://www.luogu.com.cn/problem/P5960)
*/
/**
 * 本题为差分约束模板题
 * 在 SPFA 求解过程中，需要把所有的点的初始距离都归零，以及入队
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SPFA::Graph<int> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        int y;
        cin >> a >> b >> y;
        // f(a) <= f(b)+y ，说明 b 到 a 有 y 的路径
        G.add_edge(b - 1, a - 1, y);
    }
    G._prepare();

    OY::SPFA::Solver<OY::SPFA::AddGroup<int>> sol(n);
    for (uint32_t i = 0; i < n; i++) sol.set_distance(i, 0);
    if (!sol.run(G))
        cout << "NO";
    else
        for (uint32_t i = 0; i < n; i++) cout << sol.query(i) << ' ';
}