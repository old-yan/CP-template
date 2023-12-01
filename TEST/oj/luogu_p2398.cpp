#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/FastSieve.h"

/*
[P2398 GCD SUM](https://www.luogu.com.cn/problem/P2398)
*/
/**
 * 本题可以视为 gcd 卷积模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::SIEVE::FastSieve<100100> ps(n + 100);
    std::vector<uint64_t> arr(n + 1);
    std::fill(arr.begin() + 1, arr.end(), 1);
    OY::FASTTRANS::fast_gcd_transform<true>(arr.begin(), arr.end(), [&](uint32_t k) { return ps.query_kth_prime(k); });
    for (auto &a : arr) a *= a;
    OY::FASTTRANS::fast_gcd_transform<false>(arr.begin(), arr.end(), [&](uint32_t k) { return ps.query_kth_prime(k); });
    uint64_t ans = 0;
    for (uint64_t val = 1; val <= n; val++) ans += val * arr[val];
    cout << ans;
}
