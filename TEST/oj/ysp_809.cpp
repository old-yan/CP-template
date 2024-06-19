#include "IO/FastIO.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"

/*
[Jump on Tree](https://judge.yosupo.jp/problem/jump_on_tree)(https://github.com/yosupo06/library-checker-problems/issues/809)
 */
/**
 * 本题为 la 模板题
 * 一般数据下均为倍增法最快
 * 树退化成链时，换 hld 最快
 */

static constexpr uint32_t N = 500000;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::FlatTree::Tree<bool, N> S(n);
    uint32_t a, b;
    for (uint32_t i = 1; i != n; i++) {
        cin >> a >> b;
        S.add_edge(a, b);
    }
    S.prepare(), S.set_root(0);
    auto solve = [q](auto &&T) {
        for (uint32_t i = 0; i != q; i++) {
            uint32_t s, t, k;
            cin >> s >> t >> k;
            auto lca = T.calc(s, t);
            auto get_dep = [&](uint32_t x) {
                return T.get_depth(x);
            };
            uint32_t ds = get_dep(s);
            uint32_t dt = get_dep(t);
            uint32_t d0 = get_dep(lca);
            uint32_t len = ds + dt - d0 * 2;
            if (k > len)
                cout << "-1\n";
            else if (k <= ds - d0)
                cout << T.get_ancestor(s, k) << endl;
            else
                cout << T.get_ancestor(t, len - k) << endl;
        }
    };
    if (b == a + 1) 
        solve(OY::HLD::Table<decltype(S)>(&S));
    else
        solve(OY::DoubleLCA::Table<decltype(S)>(&S));
}