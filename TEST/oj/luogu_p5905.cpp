#include "GRAPH/Johnson.h"
#include "IO/FastIO.h"

/*
[P5905 【模板】全源最短路（Johnson）](https://www.luogu.com.cn/problem/P5905)
*/
/**
 * 本题为 Johnson 算法模板题
 */
static constexpr uint32_t N = 3000, M = 6000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Johnson::Graph<int, N, M, N * N> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        int dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto [sol, res] = G.calc<false>(1000000000);
    if (!res)
        cout << "-1\n";
    else
        for (uint32_t i = 0; i < n; i++) {
            int64_t s = 0;
            for (uint32_t j = 0; j < n; j++) s += int64_t(j + 1) * sol.query(i, j);
            cout << s << endl;
        }
}
