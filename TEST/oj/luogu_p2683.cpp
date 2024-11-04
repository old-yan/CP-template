#include "GRAPH/DynamicFloyd.h"
#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

/*
[P2683 小岛](https://www.luogu.com.cn/problem/P2683)
*/
/**
 * 本题需要一个支持动态加边的 floyd
 */

void solve_floyd() {
    uint32_t n, m;
    cin >> n >> m;
    OY::StaticDynamicFloyd<OY::DFLOYD::AddGroup<uint32_t>, false, 100> G(n);
    // OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<uint32_t>, false> G(n);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t s, t;
            cin >> s >> t;
            if (G.query(s - 1, t - 1) == G.infinite())
                cout << "-1\n";
            else
                cout << G.query(s - 1, t - 1) << endl;
        } else {
            uint32_t u, v, e;
            cin >> u >> v >> e;
            G.update_by_edge(u - 1, v - 1, e);
            G.update_by_edge(v - 1, u - 1, e);
        }
    }
}

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    OY::StaticDynamicSPFAs<OY::DSPFA::AddGroup<uint32_t>, false, 100, 10000> G(n, m * 2);
    // OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<uint32_t>, false> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t s, t;
            cin >> s >> t;
            if (G.query(s - 1, t - 1) == G.infinite())
                cout << "-1\n";
            else
                cout << G.query(s - 1, t - 1) << endl;
        } else {
            uint32_t u, v, e;
            cin >> u >> v >> e;
            G.update_by_edge(u - 1, v - 1, e);
            G.update_by_edge(v - 1, u - 1, e);
        }
    }
}

int main() {
    solve_floyd();
    // solve_spfa();
}
