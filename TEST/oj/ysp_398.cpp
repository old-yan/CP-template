#include "DS/AdjDiff.h"
#include "IO/FastIO.h"

/*
[Static Range Sum](https://judge.yosupo.jp/problem/static_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/398)
*/
/**
 * 本题为差分模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::AdjDiff::Table<int64_t, false> S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    S.switch_to_presum();
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l, r - 1) << endl;
    }
}