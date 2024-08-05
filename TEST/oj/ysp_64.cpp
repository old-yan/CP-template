#include "IO/FastIO.h"
#include "MATH/KthRoot.h"

/*
[Kth Root (Integer)](https://judge.yosupo.jp/problem/kth_root_integer)(https://github.com/yosupo06/library-checker-problems/issues/64)
*/
/**
 * 本题为模板
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint64_t x;
        uint32_t k;
        cin >> x >> k;
        cout << OY::KthRoot::floor_iroot(x, k) << endl;
    }
}
