#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[Bitwise Xor Convolution](https://judge.yosupo.jp/problem/bitwise_xor_convolution)(https://github.com/yosupo06/library-checker-problems/issues/622)
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
    auto div_by_2 = [twoinv = mint(2).inv()](auto x) {
        return x * twoinv;
    };
    OY::FASTTRANS::fast_bitxor_transform<true>(a.begin(), a.end(), div_by_2);
    OY::FASTTRANS::fast_bitxor_transform<true>(b.begin(), b.end(), div_by_2);
    for (uint32_t i = 0; i != 1 << n; i++) a[i] *= b[i];
    OY::FASTTRANS::fast_bitxor_transform<false>(a.begin(), a.end(), div_by_2);
    for (uint32_t i = 0; i != 1 << n; i++) cout << a[i] << ' ';
}