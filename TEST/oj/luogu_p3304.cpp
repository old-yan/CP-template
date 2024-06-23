#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RaySeg.h"
#include "TREE/TreeTransfer.h"

/*
[P3304 [SDOI2013] 直径](https://www.luogu.com.cn/problem/P3304)
*/
/**
 * 与直径相关的问题，可以使用最长射线线段助手解决
 */

static constexpr uint32_t N = 200000;
void solve_treetrans() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    S.prepare(), S.set_root(0);

    struct Node {
        uint64_t m_ray[3], m_seg[2];
        void add_ray(uint64_t ray) {
            if (ray > m_ray[0])
                m_ray[2] = m_ray[1], m_ray[1] = m_ray[0], m_ray[0] = ray;
            else if (ray > m_ray[1])
                m_ray[2] = m_ray[1], m_ray[1] = ray;
            else if (ray > m_ray[2])
                m_ray[2] = ray;
        }
        void add_seg(uint64_t seg) {
            if (seg > m_seg[0])
                m_seg[1] = m_seg[0], m_seg[0] = seg;
            else if (seg > m_seg[1])
                m_seg[1] = seg;
        }
        void add(uint64_t ray, uint64_t seg) {
            add_ray(ray), add_seg(std::max(ray, seg));
        }
        uint64_t ray() const { return m_ray[0]; }
        uint64_t seg() const { return std::max(m_ray[0] + m_ray[1], m_seg[0]); }
        uint64_t ray_except(uint64_t x) const { return x == m_ray[0] ? m_ray[1] : m_ray[0]; }
        uint64_t seg_except(uint64_t except_ray, uint64_t except_seg) const {
            except_seg = std::max(except_ray, except_seg);
            uint64_t most_seg = except_seg == m_seg[0] ? m_seg[1] : m_seg[0];
            uint64_t most_rays = except_ray == m_ray[0] ? m_ray[1] + m_ray[2] : (except_ray == m_ray[1] ? m_ray[0] + m_ray[2] : m_ray[0] + m_ray[1]);
            return std::max(most_seg, most_rays);
        }
    };
    auto mapping = [](auto...) -> Node { return {}; };
    auto merge = [](Node &dp_a, const Node &dp_to, uint32_t a, uint32_t to, uint32_t e) { dp_a.add(dp_to.ray() + e, dp_to.seg()); };
    uint64_t maxd{}, cnt{};
    auto exclude = [&](Node &dp_a, const Node &dp_p, uint32_t a, uint32_t p, uint32_t e) {
        if (~p) {
            uint64_t d1 = dp_p.seg_except(dp_a.ray() + e, dp_a.seg());
            uint64_t d2 = dp_a.seg();
            uint64_t d = dp_p.ray_except(dp_a.ray() + e) + e + dp_a.ray();
            if (d >= maxd) {
                if (d > maxd) maxd = d, cnt = 0;
                if (d > d1 && d > d2) cnt++;
            }
            dp_a.add(d - dp_a.ray(), d1);
        }
    };
    OY::TreeTransfer::solve<Node>(S, mapping, merge, exclude);
    cout << maxd << endl
         << cnt << endl;
}

void solve_rayseg() {
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

int main() {
    solve_treetrans();
    // solve_rayseg();
}