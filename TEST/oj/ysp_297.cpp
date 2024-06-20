#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[Subset Convolution](https://judge.yosupo.jp/problem/subset_convolution)(https://github.com/yosupo06/library-checker-problems/issues/297)
*/
/**
 * 本题为卷积模板
 */

static constexpr uint32_t N = 20;
using mint = OY::mgint998244353;
mint arr1[N + 1][1 << N], arr2[N + 1][1 << N], arr3[N + 1][1 << N], sum[1 << N];

int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < 1 << n; i++) {
        uint32_t x;
        cin >> x;
        arr1[std::popcount(i)][i] = mint::raw(x);
    }
    for (uint32_t i = 0; i <= n; i++) OY::FASTTRANS::fast_bitor_transform<true>(arr1[i], arr1[i] + (1 << n));

    for (uint32_t i = 0; i < 1 << n; i++) {
        uint32_t x;
        cin >> x;
        arr2[std::popcount(i)][i] = mint::raw(x);
    }
    for (uint32_t i = 0; i <= n; i++) OY::FASTTRANS::fast_bitor_transform<true>(arr2[i], arr2[i] + (1 << n));

    for (uint32_t s = 0; s <= n; s++)
        for (uint32_t i = 0; i <= s; i++)
            for (uint32_t k = 0; k < 1 << n; k++) arr3[s][k] += arr1[i][k] * arr2[s - i][k];

    for (uint32_t s = 0; s <= n; s++) OY::FASTTRANS::fast_bitor_transform<false>(arr3[s], arr3[s] + (1 << n));
    for (uint32_t i = 0; i < 1 << n; i++) cout << arr3[std::popcount(i)][i] << ' ';
}
