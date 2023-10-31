#include "GRAPH/EulerPath_dg.h"
#include "GRAPH/EulerPath_udg.h"
#include "IO/FastIO.h"

/*
[#117. 欧拉回路](https://uoj.ac/problem/117)
*/
/**
 * 本题为欧拉回路模板题
 */
static constexpr uint32_t N = 100000, M = 200000;

int main() {
    char type;
    cin >> type;
    uint32_t n, m;
    cin >> n >> m;
    if (!m) {
        cout << "YES\n";
        return 0;
    }
    if (type == '1') {
        OY::EulerPathUDG::Graph<N, M> G(n, m);
        for (uint32_t i = 0; i < m; i++) {
            uint32_t a, b;
            cin >> a >> b;
            G.add_edge(a - 1, b - 1);
        }
        auto sol = G.calc();
        if (!sol.is_Euler_graph())
            cout << "NO\n";
        else {
            cout << "YES\n";
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                if (from == G.m_edges[index].m_from)
                    cout << index + 1 << ' ';
                else
                    cout << '-' << index + 1 << ' ';
            });
        }
    } else {
        OY::EulerPathDG::Graph<N, M> G(n, m);
        for (uint32_t i = 0; i < m; i++) {
            uint32_t a, b;
            cin >> a >> b;
            G.add_edge(a - 1, b - 1);
        }
        auto sol = G.calc();
        if (!sol.is_Euler_graph())
            cout << "NO\n";
        else {
            cout << "YES\n";
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                cout << ' ' << index + 1;
            });
        }
    }
}
