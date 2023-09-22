#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"

/*
[P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
*/
static constexpr uint32_t N = 500000;
int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::FlatTree::Tree<bool, N> S(n);
    // OY::LinkTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();
    S.set_root(s - 1);

    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, uint64_t, N>, N> T(&S);
    // OY::DoubleLCA::Table<decltype(S), N, N * 19> T(&S);
    // OY::HLD::Table<decltype(S), N> T(&S);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        cout << T.calc(a - 1, b - 1) + 1 << '\n';
    }
}