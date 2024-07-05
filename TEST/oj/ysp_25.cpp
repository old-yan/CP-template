#include "GRAPH/EulerPath_dg.h"
#include "GRAPH/EulerPath_udg.h"
#include "IO/FastIO.h"

/*
[Eulerian Trail (Directed)](https://judge.yosupo.jp/problem/eulerian_trail_directed)(https://github.com/yosupo06/library-checker-problems/issues/25)
*/
/**
 * 本题为有向图欧拉回路模板题
 */
/*
[Eulerian Trail (Undirected)](https://judge.yosupo.jp/problem/eulerian_trail_undirected)(https://github.com/yosupo06/library-checker-problems/issues/25)
*/
/**
 * 本题为无向图欧拉回路模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    OY::EulerPathDG::Graph G;
    // OY::EulerPathUDG::Graph G;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        G.resize(n, m);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b;
            cin >> a >> b;
            G.add_edge(a, b);
        }
        auto sol = G.calc();
        auto source = sol.get_source();
        if (!~source)
            cout << "No\n";
        else {
            cout << "Yes\n";
            bool started = false;
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                if (!started) started = true, cout << from;
                cout << ' ' << to;
            });
            if (!started) cout << 0;
            cout << endl;
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                cout << index << ' ';
            });
            cout << endl;
        }
    }
}