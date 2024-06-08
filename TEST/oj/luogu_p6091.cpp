#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/PrimitiveRoot.h"

/*
[P6091 【模板】原根](https://www.luogu.com.cn/problem/P6091)
*/
/**
 * 本题为原根模板题
 * 先找出最小原根，然后把所有幂次找出来即可
 */

int main() {
    uint32_t t;
    cin >> t;
    using mint = OY::DynamicModInt32<0>;
    using Solver = OY::PROOT::PollardRhoSolver<mint>;
    for (uint32_t i = 0; i < t; i++) {
        uint32_t n, d;
        cin >> n >> d;
        mint::set_mod(n, false);
        if (Solver::has_primitive_root()) {
            Solver sol;
            cout << sol.count() << '\n';
            std::vector<bool> is_proot(n + 1);
            sol.do_for_each([&](auto x) { is_proot[x.val()] = true; });
            for (uint32_t j = 1, k = 0; j <= n; j++)
                if (is_proot[j]) {
                    if (++k == d) k = 0;
                    if (!k) cout << j << ' ';
                }
            cout << endl;
        } else
            cout << "0\n\n";
    }
}