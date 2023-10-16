#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"

/*
[P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
*/
/**
 * 本题要进行若干次树上路径增值，最后进行树上单点查询
 * 显然为树上差分模板题
*/
uint32_t parent[50000];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<bool, 50000> S(n);
    // OY::LinkTree::Tree<bool, 50000> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});

    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, uint64_t, 50000>, 50000> LCA(&S);
    OY::AdjDiffTree::Table<uint32_t, decltype(S), false, 50000> T(&S);
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
