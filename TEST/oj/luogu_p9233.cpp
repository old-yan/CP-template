#include "DS/FHQCounter.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"
#include "TREE/LinkTree.h"

/*
[P9233 [蓝桥杯 2023 省 A] 颜色平衡树](https://www.luogu.com.cn/problem/P9233)
*/
/**
 * 本题为树上启发式合并模板题
 */

static constexpr uint32_t N = 200000;
void solve_counter() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> S(n);
    std::vector<uint32_t> color(n);
    for (uint32_t i = 0; i != n; i++) {
        cin >> color[i];
        uint32_t f;
        cin >> f;
        if (f) S.add_edge(f - 1, i);
    }
    S.prepare(), S.set_root(0);

    using ColorCounter = OY::StaticSegCounter<uint32_t, uint32_t, false, true, false, N * 4>;
    // using ColorCounter = OY::FHQCNT::Table<uint32_t, uint32_t, false, true, N * 2>;
    using ColorCountCounter = OY::StaticSegCounter<uint32_t, uint32_t, false, true, false, N * 4>;
    // using ColorCountCounter = OY::FHQCNT::Table<uint32_t, uint32_t, false, true, N * 2>;
    struct node {
        // m_colors 维护颜色频率
        ColorCounter m_colors;
        // m_cnts 维护频率的频率
        ColorCountCounter m_cnts;
        void insert(uint32_t x) {
            uint32_t now = m_colors.query(x);
            if (now) m_cnts.add(now, -1);
            m_cnts.add(now + 1, 1);
            m_colors.add(x, 1);
        }
        void merge(node &rhs) {
            if (m_colors.size() < rhs.m_colors.size()) std::swap(*this, rhs);
            rhs.m_colors.enumerate([&](auto k, auto v) {
                uint32_t now = m_colors.query(k);
                if (now) m_cnts.add(now, -1);
                m_cnts.add(now + v, 1);
            });
            m_colors.merge(rhs.m_colors);
        }
    };
    std::vector<node> cnts(n);
    uint32_t ans = 0;
    auto report = [&](uint32_t a, uint32_t to) {
        cnts[a].merge(cnts[to]);
    };
    auto after = [&](uint32_t a) {
        cnts[a].insert(color[a]);
        if (cnts[a].m_cnts.size() == 1) ans++;
    };
    S.tree_dp_vertex(0, {}, report, after);
    cout << ans;
}

int main() {
    solve_counter();
}