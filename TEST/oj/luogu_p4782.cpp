#include "GRAPH/TwoSat.h"
#include "IO/FastIO.h"

/*
[P4782 【模板】2-SAT](https://www.luogu.com.cn/problem/P4782)
*/
/**
 * 本题为 2-SAT 模板题
 */
static constexpr uint32_t N = 1000000, M = 1000000;
uint32_t w[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::TWOSAT::Graph G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, a_val, b, b_val;
        cin >> a >> a_val >> b >> b_val;
        G.add_clause(a - 1, a_val, b - 1, b_val);
    }

    if (!G.calc())
        cout << "IMPOSSIBLE";
    else {
        cout << "POSSIBLE\n";
        for (uint32_t i = 0; i < n; i++) {
            cout << G.query(i) << ' ';
        }
    }
}