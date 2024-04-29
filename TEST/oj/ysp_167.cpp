#include "DS/WTree.h"
#include "IO/FastIO.h"
#include "TREE/DfnController.h"
#include "TREE/LinkTree.h"

/*
[Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/167)
*/
/**
 * 本题为
 */

static constexpr uint32_t N = 500000;
uint32_t val[N];
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 0; i != n; i++) cin >> val[i];
    for (uint32_t i = 1; i != n; i++) {
        uint32_t p;
        cin >> p;
        S.add_edge(i, p);
    }
    S.prepare(), S.set_root(0);

    OY::DFN::Table<decltype(S)> T(&S);
    OY::WTree::Tree<uint64_t> bit(n, [&](uint32_t i) {
        return val[T.m_seq[i]];
    });

    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t u, x;
            cin >> u >> x;
            T.do_for_vertex(u, [&](uint32_t pos) {
                bit.add(pos, x);
            });
        } else {
            uint32_t u;
            cin >> u;
            uint64_t res{};
            T.do_for_subtree(u, [&](uint32_t l, uint32_t r) {
                res += bit.query(l, r);
            });
            cout << res << endl;
        }
    }
}