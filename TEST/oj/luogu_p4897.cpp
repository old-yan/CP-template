#include "GRAPH/Gusfield.h"
#include "IO/FastIO.h"

/*
[P4897 【模板】最小割树（Gomory-Hu Tree）](https://www.luogu.com.cn/problem/P4897)
*/
/**
 * 本题为最小割树模板题
 */
static constexpr uint32_t N = 501, M = 1500;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::Gusfield::Graph<uint32_t, N, M, N * N> G(n + 1, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a, b, cost);
    }
    G.calc();

    uint32_t q;
    cin >> q;
    for (uint32_t i = 0; i < q; i++) {
        uint32_t a, b;
        cin >> a >> b;
        cout << G.query(a, b) << endl;
    }
}
