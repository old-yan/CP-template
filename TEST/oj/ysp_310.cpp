#include "DS/MergeSortTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)(https://github.com/yosupo06/library-checker-problems/issues/310)
*/
/**
 * 本题为小波树模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::MS::Tree<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    // OY::WaveLet::Tree<uint32_t> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << S.quantile(l, r - 1, k) << endl;
    }
}