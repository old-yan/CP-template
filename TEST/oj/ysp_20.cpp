#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[Point Add Range Sum](https://judge.yosupo.jp/problem/point_add_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/20)
*/
/**
 * 本题为普通树状数组模板题
 * 也可以通过 SIMD 线段树通过
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::WTree::Tree<int64_t> S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t p;
            int64_t x;
            cin >> p >> x;
            S.add(p, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l, r - 1) << endl;
        }
    }
}
