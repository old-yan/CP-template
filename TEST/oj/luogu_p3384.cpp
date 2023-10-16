#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/HLDZkw.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P3384 【模板】重链剖分/树链剖分](https://www.luogu.com.cn/problem/P3384)
*/
/**
 * 本题为树链剖分模板题
 * 树链剖分结合数据结构，可以实现各种树上区间操作
*/
static constexpr uint32_t N = 100000;
uint64_t val[N];
int main() {
    uint32_t n, m, r, p;
    cin >> n >> m >> r >> p;
    OY::FlatTree::Tree<bool, N> S(n);
    // OY::LinkTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 0; i < n; i++) cin >> val[i];
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(r - 1);

    OY::HLDBIT::TreeBIT<decltype(S), uint64_t, N> T(&S, [&](uint32_t i) { return val[i]; });
    // OY::HLDZkwLazySumTree<decltype(S), N> T(&S, [&](uint32_t i) { return val[i]; });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            uint32_t z;
            cin >> x >> y >> z;
            T.add_path<true>(x - 1, y - 1, z);
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            cout << T.query_path<true>(x - 1, y - 1) % p << endl;
        } else if (op == '3') {
            uint32_t x;
            uint32_t z;
            cin >> x >> z;
            T.add_subtree(x - 1, z);
        } else {
            uint32_t x;
            cin >> x;
            cout << T.query_subtree(x - 1) % p << endl;
        }
    }
}
