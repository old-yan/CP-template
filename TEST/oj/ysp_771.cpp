#include "DS/OfflineRectAddRectSumCounter2D.h"
#include "IO/FastIO.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[Static Rectangle Add Rectangle Sum](https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/771)
*/

int main() {
    using mint = OY::mgint998244353;
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINERARSC2D::Solver<uint32_t, mint, mint> S(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, d, r, u;
        mint w;
        cin >> l >> d >> r >> u >> w;
        S.add_rect(l, r - 1, d, u - 1, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        S.add_query(l, r - 1, d, u - 1);
    }
    auto res = S.solve();
    for (auto a : res) cout << a << endl;
}