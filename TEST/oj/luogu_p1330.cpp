#include "GRAPH/Bipartite.h"
#include "IO/FastIO.h"

/*
[P1330 封锁阳光大学](https://www.luogu.com.cn/problem/P1330)
*/
/**
 * 本题实际上要求图为二分图，且要在每个连通块内选择结点数量较少的颜色
*/
static constexpr uint32_t N = 10000, M = 100000;
uint32_t color_cnt[N << 1];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Bipartite::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }
    auto sol = G.calc();
    if (!sol.is_bipartite())
        cout << "Impossible";
    else {
        for (uint32_t i = 0; i < n; i++) color_cnt[sol.query(i)]++;
        uint32_t ans = 0;
        for (uint32_t i = 0; i < sol.m_color_cnt; i += 2) ans += std::min(color_cnt[i], color_cnt[i + 1]);
        cout << ans;
    }
}