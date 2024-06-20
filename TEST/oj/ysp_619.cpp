#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[Bitwise And Convolution](https://judge.yosupo.jp/problem/bitwise_and_convolution)(https://github.com/yosupo06/library-checker-problems/issues/619)
*/
/**
 * 本题为卷积模板
 */

int main() {
    using mint = OY::mgint998244353;
    uint32_t n;
    cin >> n;
    std::vector<mint> a(1 << n), b(1 << n);
    for (auto &e : a) cin >> e;
    for (auto &e : b) cin >> e;
    OY::FASTTRANS::fast_bitand_transform<true>(a.begin(), a.end());
    OY::FASTTRANS::fast_bitand_transform<true>(b.begin(), b.end());
    for (uint32_t i = 0; i != 1 << n; i++) a[i] *= b[i];
    OY::FASTTRANS::fast_bitand_transform<false>(a.begin(), a.end());
    for (uint32_t i = 0; i != 1 << n; i++) cout << a[i] << ' ';
}