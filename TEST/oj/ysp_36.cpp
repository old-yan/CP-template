#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

/*
[Strongly Connected Components](https://judge.yosupo.jp/problem/scc)(https://github.com/yosupo06/library-checker-problems/issues/36)
*/
/**
 * 本题为 tarjan 算法求 scc 模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SCC::Graph G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    auto sol = G.calc();
    auto gs = sol.get_groups();
    cout << gs.size() << endl;
    for (uint32_t i = 0; i < gs.size(); i++) {
        cout << gs[i].size() << ' ';
        for (auto e : gs[i]) cout << e << ' ';
        cout << endl;
    }
}