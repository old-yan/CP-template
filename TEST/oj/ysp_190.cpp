#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"

/*
[Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/190)
*/
/**
 * 本题为树链剖分套普通线段树模板题
 */

static constexpr uint32_t N = 200000;
static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
node val[N];
constexpr node identity{1, 0};
int main() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> val[i].mul >> val[i].add;
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v;
        cin >> u >> v;
        S.add_edge(u, v);
    }
    S.prepare(), S.set_root(0);

    auto hld = OY::HLD::Table<decltype(S)>(&S);
    auto reverse_add = [](const auto &x, const auto &y) { return y + x; };
    auto Z1 = OY::MonoSumTree<node, identity>(n, [&](uint32_t i) {
        return val[hld.m_seq[i]];
    });
    auto Z2 = OY::MONOZKW::Tree<OY::MONOZKW::BaseMonoid<node, identity, decltype(reverse_add)>>(n, [&](uint32_t i) {
        return val[hld.m_seq[i]];
    });

    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t a, b, c;
        cin >> op >> a >> b >> c;
        if (op == '0')
            Z1.modify(hld.m_info[a].m_dfn, {b, c}), Z2.modify(hld.m_info[a].m_dfn, {b, c});
        else {
            node res{1, 0};
            hld.do_for_directed_path(a, b, [&](uint32_t l, uint32_t r) {
                if (l <= r)
                    res = res + Z1.query(l, r);
                else
                    res = res + Z2.query(r, l);
            });
            cout << res.calc(c) << endl;
        }
    }
}
