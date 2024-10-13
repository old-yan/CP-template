#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"

/*
[P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
*/
/**
 * 本题要进行若干次树上路径增值，最后进行树上单点查询
 * 显然为树上差分模板题
 */

static constexpr uint32_t N = 50000;
uint32_t parent[N];
void solve_adj() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});

    OY::RMQLCA::Table<decltype(S), OY::SqrtMinTable<uint32_t, OY::SQRT::RandomController<>, 9>> LCA(&S);
    OY::AdjSumTreeTable<uint32_t, decltype(S), false> T(&S);
    T.switch_to_difference_upward();
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        uint32_t lca = LCA.calc(a - 1, b - 1);
        T.add_path(a - 1, b - 1, lca, parent[lca], 1);
    }
    T.switch_to_value();
    uint32_t ans = 0;
    for (uint32_t i = 0; i < n; i++) ans = std::max(ans, T.query(i));
    cout << ans;
}

void solve_hldbit() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::HLDBIT::TreeBIT<decltype(S), uint32_t> T(&S);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        T.add_path<true>(a - 1, b - 1, 1);
    }
    uint32_t ans = 0;
    T.m_bit.enumerate([&](auto val) {
        ans = std::max(ans, val);
    });
    cout << ans;
}

int main() {
    solve_adj();
    // solve_hldbit();
}