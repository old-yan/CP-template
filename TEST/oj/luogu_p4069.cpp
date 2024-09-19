#include "DS/LichaoSegTree_ex.h"
#include "DS/LichaoZkwTree_ex.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"

/*
[P4069 [SDOI2016] 游戏](https://www.luogu.com.cn/problem/P4069)
*/
/**
 * 显然，本题为维护区间最值的李超线段树
 */

static constexpr uint32_t N = 100000;
static constexpr int64_t inf = 123456789123456789;
uint64_t pos[N];
void solve_no_discretize() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b, w;
        cin >> a >> b >> w;
        S.add_edge(a - 1, b - 1, w);
    }
    S.prepare(), S.set_root(0);
    auto hld = OY::HLD::Table<decltype(S)>(&S);

    // 预处理每个点和父点的距离，每个点到根的距离
    std::vector<uint32_t> up_edge(n);
    std::vector<uint64_t> to_root(n);
    auto pre = [&](uint32_t a, uint32_t p, uint32_t e) {
        up_edge[a] = e;
        to_root[a] = ~p ? to_root[p] + e : e;
    };
    S.tree_dp_edge(0, pre, {}, {});

    // 找到每个点的 dfs 序还不够，还要找到每个点的离散权值
    // hld.m_seq[i] 对应位置 pos[i]
    uint64_t cur = 0;
    for (uint32_t i = 0, j; i != n; i = j) {
        pos[i] = cur;
        for (j = i + 1; j != n && hld.find_parent(hld.m_seq[j]) == hld.m_seq[j - 1]; j++) pos[j] = pos[j - 1] + up_edge[hld.m_seq[j]];
        cur = pos[j - 1] + 1;
    }
    OY::VectorLichaoSlopeChminSegTree_ex<int64_t, inf, false>::_reserve(4500000);
    OY::VectorLichaoSlopeChminSegTree_ex<int64_t, inf, false> T(cur);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t s, t;
            int64_t a, b;
            cin >> s >> t >> a >> b;
            uint32_t lca = hld.calc(--s, --t);
            bool pass_lca = false;
            auto call = [&](uint32_t l, uint32_t r) {
                if (!pass_lca && l >= r) {
                    // 上升段
                    uint64_t dis_s_l = to_root[s] - to_root[hld.m_seq[l]];
                    uint64_t x_l = pos[l], x_r = pos[r];
                    T.add_line(x_r, x_l, {-a, a * int64_t(dis_s_l + x_l) + b});
                    pass_lca = pass_lca || (r <= hld.m_info[lca].m_dfn && hld.m_info[lca].m_dfn <= l);
                } else {
                    pass_lca = true;
                    // 下降段
                    uint64_t dis_s_l = to_root[s] + to_root[hld.m_seq[l]] - to_root[lca] * 2;
                    uint64_t x_l = pos[l], x_r = pos[r];
                    T.add_line(x_l, x_r, {a, a * int64_t(dis_s_l - x_l) + b});
                }
            };
            hld.do_for_directed_path(s, t, call);
        } else {
            uint32_t s, t;
            cin >> s >> t;
            int64_t ans = INT64_MAX;
            auto call = [&](uint32_t l, uint32_t r) {
                ans = std::min(ans, T.query(pos[l], pos[r]));
            };
            hld.do_for_path<true>(s - 1, t - 1, call);
            cout << ans << endl;
        }
    }
}

void solve_discretize() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b, w;
        cin >> a >> b >> w;
        S.add_edge(a - 1, b - 1, w);
    }
    S.prepare(), S.set_root(0);
    auto hld = OY::HLD::Table<decltype(S)>(&S);

    // 预处理每个点和父点的距离，每个点到根的距离
    std::vector<uint32_t> up_edge(n);
    std::vector<uint64_t> to_root(n);
    auto pre = [&](uint32_t a, uint32_t p, uint32_t e) {
        up_edge[a] = e;
        to_root[a] = ~p ? to_root[p] + e : e;
    };
    S.tree_dp_edge(0, pre, {}, {});

    // 找到每个点的 dfs 序还不够，还要找到每个点的离散权值
    // hld.m_seq[i] 对应位置 pos[i]
    uint64_t cur = 0;
    for (uint32_t i = 0, j; i != n; i = j) {
        pos[i] = cur;
        for (j = i + 1; j != n && hld.find_parent(hld.m_seq[j]) == hld.m_seq[j - 1]; j++) pos[j] = pos[j - 1] + up_edge[hld.m_seq[j]];
        cur = pos[j - 1] + 1;
    }
    pos[n] = cur;
    struct Line {
        int64_t m_k, m_b;
        static Line make_constant_line(int64_t val) { return Line(0, val); }
        Line() = default;
        Line(int64_t k, int64_t b) : m_k(k), m_b(b) {}
        void add(int64_t inc) { m_b += inc; }
        int64_t calc(uint32_t i) const { return m_k * pos[i] + m_b; }
        bool is_ascending() const { return m_k > 0; }
        bool is_horizontal() const { return m_k == 0; }
    };
    OY::LCZKWEX::Tree<int64_t, inf, false, false, Line> T(n);
    // OY::LCSEGEX::Tree<int64_t, inf, false, false, Line, uint32_t>::_reserve(200000);
    // OY::LCSEGEX::Tree<int64_t, inf, false, false, Line, uint32_t> T(n);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t s, t;
            int64_t a, b;
            cin >> s >> t >> a >> b;
            uint32_t lca = hld.calc(--s, --t);
            bool pass_lca = false;
            auto call = [&](uint32_t l, uint32_t r) {
                if (!pass_lca && l >= r) {
                    // 上升段
                    uint64_t dis_s_l = to_root[s] - to_root[hld.m_seq[l]];
                    T.add_line(r, l, {-a, a * int64_t(dis_s_l + pos[l]) + b});
                    pass_lca = pass_lca || (r <= hld.m_info[lca].m_dfn && hld.m_info[lca].m_dfn <= l);
                } else {
                    pass_lca = true;
                    // 下降段
                    uint64_t dis_s_l = to_root[s] + to_root[hld.m_seq[l]] - to_root[lca] * 2;
                    T.add_line(l, r, {a, a * int64_t(dis_s_l - pos[l]) + b});
                }
            };
            hld.do_for_directed_path(s, t, call);
        } else {
            int s, t;
            cin >> s >> t;
            int64_t ans = INT64_MAX;
            auto call = [&](uint32_t l, uint32_t r) {
                ans = std::min(ans, T.query(l, r));
            };
            hld.do_for_path<true>(s - 1, t - 1, call);
            cout << ans << endl;
        }
    }
}

int main() {
    solve_discretize();
    // solve_no_discretize();
}