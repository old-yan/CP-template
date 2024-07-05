#include "GRAPH/HopcroftKarp.h"
#include "GRAPH/Hungarian.h"
#include "IO/FastIO.h"

/*
[P3386 【模板】二分图最大匹配](https://www.luogu.com.cn/problem/P3386)
*/
/**
 * 本题为二分图最大匹配模板题
 */

int main() {
    uint32_t n1, n2, m;
    cin >> n1 >> n2 >> m;
    OY::HK::Graph G(std::max(n1, n2), m);
    // OY::HG::Graph G(std::max(n1, n2), m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    cout << G.calc();
}
