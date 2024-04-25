#include "DS/KBIT.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

/*
[#P5428. K阶求和数列](https://new.bzoj.org:88/p/P5428)
*/
/**
 * 本题显然为 KBIT 模板题
 */

// 这里递归调用 _solve_kbit，每层的 MAXK 降低 1，找到正好满足 MAXK == k + 2 的那个模板，避免性能浪费
template <uint32_t MAXK>
void _solve_kbit(uint32_t n, uint32_t m, uint32_t k) {
    if constexpr (MAXK > 1) {
        if (k + 2 < MAXK) return _solve_kbit<MAXK - 1>(n, m, k);
        using mint = OY::mint1000000007;
        using KBIT = OY::KBIT::Tree<mint, MAXK>;
        KBIT::prepare();
        KBIT S(n);
        for (uint32_t i = 0; i < m; i++) {
            char op;
            cin >> op;
            if (op == 'Q') {
                uint32_t l, r;
                cin >> l >> r;
                cout << S.query_dynamic(k + 1, l - 1, r - 1) << endl;
            } else {
                uint32_t l, r;
                mint x;
                cin >> l >> r >> x;
                S.template add<1, 1>(l - 1, r - 1, {x});
            }
        }
    }
}
void solve_kbit() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    _solve_kbit<12>(n, m, k);
}

int main() {
    solve_kbit();
}