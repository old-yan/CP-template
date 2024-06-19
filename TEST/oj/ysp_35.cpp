#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"

/*
[Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca)(https://github.com/yosupo06/library-checker-problems/issues/35)
 */
/**
 * 本题为 lca 模板题
 */

static constexpr uint32_t N = 500000;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t fa;
        cin >> fa;
        S.add_edge(i, fa);
    }
    S.prepare(), S.set_root(0);

    auto T = OY::RMQLCA::Table<decltype(S), OY::SqrtMinTable<uint32_t>>(&S);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t a, b;
        cin >> a >> b;
        cout << T.calc(a, b) << endl;
    }
}
