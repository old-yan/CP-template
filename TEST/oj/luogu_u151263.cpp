#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/FastSieve.h"
#include "MATH/StaticModInt32.h"

/*
[U151263 GCD 卷积](https://www.luogu.com.cn/problem/U151263)
*/
/**
 * 本题为 gcd 卷积模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::SIEVE::FastSieve<400100> ps(n + 100);
    std::vector<OY::mint998244353> arr1(n + 1), arr2(n + 1);
    for (uint32_t i = 1; i <= n; i++) cin >> arr1[i];
    for (uint32_t i = 1; i <= n; i++) cin >> arr2[i];
    OY::FASTTRANS::fast_gcd_transform<true>(arr1.begin(), arr1.end(), [&](uint32_t k) { return ps.query_kth_prime(k); });
    OY::FASTTRANS::fast_gcd_transform<true>(arr2.begin(), arr2.end(), [&](uint32_t k) { return ps.query_kth_prime(k); });
    for (uint32_t i = 0; i <= n; i++) arr1[i] *= arr2[i];
    OY::FASTTRANS::fast_gcd_transform<false>(arr1.begin(), arr1.end(), [&](uint32_t k) { return ps.query_kth_prime(k); });
    uint32_t ans = 0;
    for (auto a : arr1) ans ^= a.val();
    cout << ans;
}
