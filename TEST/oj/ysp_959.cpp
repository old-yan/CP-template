#include "DS/RangeMode.h"
#include "IO/FastIO.h"

/*
[Static Range Mode Query](https://judge.yosupo.jp/problem/static_range_mode_query)(https://github.com/yosupo06/library-checker-problems/issues/959)
*/
/**
 * 本题需要对静态区间进行区间众数查询
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::RangeMode<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        auto res = S.query(l, r - 1);
        cout << res.m_val << ' ' << res.m_cnt << endl;
    }
}