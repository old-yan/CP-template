#include "DS/KBIT.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

/*
[#P5428. K阶求和数列](https://new.bzoj.org:88/p/P5428)
*/
/**
 * 本题显然为 KBIT 模板题
*/

void solve_kbit() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    using mint = OY::mint1000000007;
    using KBIT = OY::KBIT::Tree<mint, 12, 200000>;
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
            S.add<1, 1>(l - 1, r - 1, {x});
        }
    }
}

int main() {
    solve_kbit();
}