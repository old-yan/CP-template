#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/RaySeg.h"

/*
[P3304 [SDOI2013] 直径](https://www.luogu.com.cn/problem/P3304)
*/
/**
 * 与直径相关的问题，可以使用最长射线线段助手解决
 */

static constexpr uint32_t N = 200000;
int main() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    S.prepare(), S.set_root(0);

    OY::RAYSEG::Table<decltype(S), uint64_t, N> T(&S);

    // 直径可以通过查询根以下的最大线段得出
    uint64_t diameter = T.query_down_seg(0), ans = 0;

    auto dfs = [&](auto self, uint32_t a, uint32_t p) -> void {
        S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t dis) {
            if (to != p) {
                // 两个条件都符合的下，a 和 to 的连边会被记入
                // 条件一，断开 a 和 tp 的连边之后，a 所在连通块的线段长度小于原直径
                // 条件二，to 以下的最大线段小于原直径
                if (T.max_ray_seg_except_son(a, to, dis).m_seg < diameter && T.query_down_seg(to) < diameter) ans++;
                self(self, to, a);
            }
        });
    };
    dfs(dfs, 0, -1);
    cout << diameter << '\n'
         << ans;
}