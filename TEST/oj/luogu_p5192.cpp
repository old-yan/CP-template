#include "GRAPH/Dinic.h"
#include "IO/FastIO.h"

/*
[P5192 Zoj3229 Shoot the Bullet|东方文花帖|【模板】有源汇上下界最大流](https://www.luogu.com.cn/problem/P5192)
*/
/**
 * 本题为有源汇上下界最大流的模板题
 */

static constexpr uint32_t N = 365, M = 1000;
int main() {
    uint32_t n, m;
    while (cin >> n >> m) {
        /**
         * 0 为摄影师
         * 1~n 为天数
         * n+1~n+m 为少女
         * n+m+1 为虚拟汇点
         */
        OY::DINIC::BoundGraph<int64_t, (N + M + 2) * 10, (M + N + 300 * N) * 10> G(n + m + 2, m + n + 300 * n);
        for (uint32_t i = 0; i < m; i++) {
            uint32_t x;
            cin >> x;
            G.add_edge(n + 1 + i, n + m + 1, x, 100000);
        }
        for (uint32_t i = 0; i < n; i++) {
            uint32_t C, D;
            cin >> C >> D;
            G.add_edge(0, 1 + i, 0, D);
            while (C--) {
                uint32_t t, l, r;
                cin >> t >> l >> r;
                G.add_edge(1 + i, n + 1 + t, l, r);
            }
        }
        G.set(0, n + m + 1);
        if (!G.is_possible().second)
            cout << "-1\n\n";
        else
            cout << G.max_flow() << "\n\n";
    }
}
