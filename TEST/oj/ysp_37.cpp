#include "GRAPH/HopcroftKarp.h"
#include "IO/FastIO.h"

/*
[Matching on Bipartite Graph](https://judge.yosupo.jp/problem/bipartitematching)(https://github.com/yosupo06/library-checker-problems/issues/37)
*/
/**
 * 本题为二分图最大匹配模板题
 */

int main() {
    uint32_t l, r, m;
    cin >> l >> r >> m;
    OY::HK::Graph G(std::max(l, r), m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    cout << G.calc() << endl;
    for (uint32_t i = 0; i != l; i++)
        if (~G.find_right(i)) cout << i << ' ' << G.find_right(i) << endl;
}