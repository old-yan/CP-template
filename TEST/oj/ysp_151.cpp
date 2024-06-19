#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

/*
[Maximum Independent Set](https://judge.yosupo.jp/problem/maximum_independent_set)(https://github.com/yosupo06/library-checker-problems/issues/151)
*/
/**
 * 本题为最大独立集模板题
 */
/*
[Enumerate Cliques](https://judge.yosupo.jp/problem/enumerate_cliques)(https://github.com/yosupo06/library-checker-problems/issues/151)
*/
/**
 * 本题为枚举团的模板题
 * 注意，是枚举团而不是枚举最大团
 */

void solve_maximum_independent_set() {
    static constexpr uint32_t N = 40;
    uint32_t n, m;
    cin >> n >> m;
    OY::BK::Graph<uint64_t, N> G(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    auto res = G.get_max_independant_set();
    cout << res.size() << endl;
    for (auto a : res) cout << a << ' ';
}

#include "MATH/StaticModInt32.h"
void solve_enumerate_cliques() {
    static constexpr uint32_t N = 100;
    using mint = OY::mint998244353;
    uint32_t n, m;
    cin >> n >> m;
    std::vector<mint> x(n);
    for (auto &e : x) cin >> e;
    OY::BK::Graph<uint64_t, N> G(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    mint sum{};
    G.enumerate_clique([&](auto clique) {
        mint prod{1};
        for (auto e : clique) prod *= x[e];
        sum += prod;
    });
    cout << sum;
}

int main() {
    solve_maximum_independent_set();
    // solve_enumerate_cliques();
}