#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

/*
[P2483 【模板】k 短路 / [SDOI2010] 魔法猪学院](https://www.luogu.com.cn/problem/P2483)
*/
/**
 * 本题为 k 短路模板题
 */

int main() {
    uint32_t n, m;
    double E;
    cin >> n >> m >> E;
    OY::KPATH::Graph<double, double, false> G(n, m);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        double d;
        cin >> a >> b >> d;
        G.add_edge(a - 1, b - 1, d);
    }

    G.calc(0, n - 1);
    uint32_t ans = 0;
    while (true) {
        auto p = G.next();
        if (p > E) break;
        E -= p, ans++;
    }
    cout << ans;
}