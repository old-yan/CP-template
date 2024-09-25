#include "DS/WTree.h"
#include "IO/FastIO.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"

/*
[Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)(https://github.com/yosupo06/library-checker-problems/issues/125)
*/
/**
 * 本题为树链剖分套普通树状数组模板题
 * 当然普通树状数组可以被 SIMD 线段树代替
 */

static constexpr uint32_t N = 500000;
uint32_t val[N];
int main() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> val[i];
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v;
        cin >> u >> v;
        S.add_edge(u, v);
    }
    S.prepare(), S.set_root(0);

    auto hld = OY::HLD::Table<decltype(S)>(&S);
    OY::WSumTree<uint64_t> W(n, [&](uint32_t i) {
        return val[hld.m_seq[i]];
    });

    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t a, b;
        cin >> op >> a >> b;
        if (op == '0')
            hld.do_for_vertex(a, [&](uint32_t pos) {
                W.add(pos, b);
            });
        else {
            uint64_t res{};
            hld.do_for_path<true>(a, b, [&](uint32_t l, uint32_t r) {
                res += W.query(l, r);
            });
            cout << res << endl;
        }
    }
}