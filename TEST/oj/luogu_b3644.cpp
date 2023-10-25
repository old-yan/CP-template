#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

/*
[B3644 【模板】拓扑排序 / 家谱树](https://www.luogu.com.cn/problem/B3644)
*/
/**
 * 本题为拓扑排序模板题
*/
static constexpr uint32_t N = 100, M = 10000;
struct Edge {
    uint32_t from, to;
} e[M];
int main() {
    uint32_t n, m = 0;
    cin >> n;

    for (uint32_t i = 0; i < n; i++) {
        while (true) {
            uint32_t s;
            cin >> s;
            if (!s) break;
            e[m++] = {i, s - 1};
        }
    }

    OY::TOPO::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) G.add_edge(e[i].from, e[i].to);

    auto sol = G.calc();
    sol.first.trace([](int i) {
        cout << i + 1 << ' ';
    });
}