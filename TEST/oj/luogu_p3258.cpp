#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"

/*
[P3258 [JLOI2014] 松鼠的新家](https://www.luogu.com.cn/problem/P3258)
*/
/**
 * 本题要进行若干次树上路径增值，最后进行树上单点查询
 * 显然为树上差分模板题
 */

static constexpr uint32_t N = 300000;
uint32_t parent[N], A[N];

void solve_adj() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 0; i < n; i++) cin >> A[i], A[i]--;
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});

    OY::RMQLCA::Table<decltype(S), OY::SqrtMinTable<uint32_t, N>, N> LCA(&S);
    // OY::DoubleLCA::Table<decltype(S), N, N * 20> LCA(&S);
    OY::AdjDiffTree::Table<uint32_t, decltype(S), false, N> T(&S);
    T.switch_to_difference_upward();
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a = A[i - 1], b = A[i];
        uint32_t lca = LCA.calc(a, b);
        T.add_path(a, b, lca, parent[lca], 1);
    }
    T.switch_to_value();
    T.add(A[0], 1);
    for (uint32_t i = 0; i < n; i++) cout << T.query(i) - 1 << '\n';
}

void solve_hldbit() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 0; i < n; i++) cin >> A[i], A[i]--;
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::HLDBIT::TreeBIT<decltype(S), uint32_t, N> T(&S);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a = A[i - 1], b = A[i];
        T.add_path<true>(a, b, 1);
    }
    T.add(A[0], 1);
    for (uint32_t i = 0; i < n; i++) cout << T.query(i) - 1 << '\n';
}

int main() {
    solve_adj();
    // solve_hldbit();
}