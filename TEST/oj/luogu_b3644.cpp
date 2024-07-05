#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

/*
[B3644 【模板】拓扑排序 / 家谱树](https://www.luogu.com.cn/problem/B3644)
*/
/**
 * 本题为拓扑排序模板题
*/

int main() {
    uint32_t n;
    cin >> n;
    OY::TOPO::Graph G(n);
    for (uint32_t i = 0; i < n; i++) {
        while (true) {
            uint32_t s;
            cin >> s;
            if (!s) break;
            G.add_edge(i,s-1);
        }
    }
    auto sol = G.calc();
    sol.first.trace([](int i) {
        cout << i + 1 << ' ';
    });
}