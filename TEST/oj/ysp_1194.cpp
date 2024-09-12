#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

#include <array>

/*
[Unionfind with Potential](https://judge.yosupo.jp/problem/unionfind_with_potential)(https://github.com/yosupo06/library-checker-problems/issues/1194)
*/
/*
[Unionfind with Potential (Non-Commutative Group)](https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group)(https://github.com/yosupo06/library-checker-problems/issues/1194)
*/
/**
 * 维护势能的并查集模板题
 * 维护每个点到自己父结点的势能差；压缩路径时做相应修改
 */

int main_commutative() {
    uint32_t n, q;
    cin >> n >> q;
    static constexpr uint32_t Mod = 998244353;
    auto reduce = [](uint32_t x) { return x >= Mod ? x - Mod : x; };
    OY::ModAddPDSUTable<uint32_t, Mod, true> U(n);
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b, dis;
            cin >> a >> b >> dis;
            if (U.unite_by_size(b, a, dis) || U.calc(b, a).second == dis)
                cout << "1\n";
            else
                cout << "0\n";
        } else {
            uint32_t a, b;
            cin >> a >> b;
            auto res = U.calc(b, a);
            if (res.first)
                cout << res.second << endl;
            else
                cout << "-1\n";
        }
    }
    return 0;
}

int main_non_commutative() {
    uint32_t n, q;
    cin >> n >> q;
    static constexpr uint32_t Mod = 998244353;
    auto reduce = [](uint32_t x) { return x >= Mod ? x - Mod : x; };
    struct Group {
        using value_type = std::array<std::array<uint32_t, 2>, 2>;
        static value_type identity() { return {1, 0, 0, 1}; }
        static value_type op(value_type x, value_type y) { return {(1ull * x[0][0] * y[0][0] + 1ull * x[0][1] * y[1][0]) % Mod, (1ull * x[0][0] * y[0][1] + 1ull * x[0][1] * y[1][1]) % Mod, (1ull * x[1][0] * y[0][0] + 1ull * x[1][1] * y[1][0]) % Mod, (1ull * x[1][0] * y[0][1] + 1ull * x[1][1] * y[1][1]) % Mod}; }
        static value_type inverse(value_type x) { return {x[1][1], x[0][1] ? Mod - x[0][1] : 0, x[1][0] ? Mod - x[1][0] : 0, x[0][0]}; }
    };
    using value_type = Group::value_type;
    OY::PDSU::Table<Group, true> U(n);
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b;
            value_type dis;
            cin >> a >> b >> dis[0][0] >> dis[0][1] >> dis[1][0] >> dis[1][1];
            if (U.unite_by_size(b, a, dis) || U.calc(b, a).second == dis)
                cout << "1\n";
            else
                cout << "0\n";
        } else {
            uint32_t a, b;
            cin >> a >> b;
            auto res = U.calc(b, a);
            if (res.first)
                cout << res.second[0][0] << ' ' << res.second[0][1] << ' ' << res.second[1][0] << ' ' << res.second[1][1] << endl;
            else
                cout << "-1\n";
        }
    }
    return 0;
}

int main() {
    main_commutative();
    // main_non_commutative();
}