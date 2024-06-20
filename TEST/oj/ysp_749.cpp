#include "IO/FastIO.h"
#include "MATH/FastSieve.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[Gcd Convolution](https://judge.yosupo.jp/problem/gcd_convolution)(https://github.com/yosupo06/library-checker-problems/issues/749)
*/
/**
 * 本题为卷积模板
 */
/*
[Lcm Convolution](https://judge.yosupo.jp/problem/lcm_convolution)(https://github.com/yosupo06/library-checker-problems/issues/749)
*/
/**
 * 本题为卷积模板
 */

OY::FASTSIEVE::Sieve<1000000> ps;
void solve_gcd() {
    using mint = OY::mgint998244353;
    uint32_t n;
    cin >> n;
    std::vector<mint> a(n + 1), b(n + 1);
    for (uint32_t i = 1; i <= n; i++) cin >> a[i];
    for (uint32_t i = 1; i <= n; i++) cin >> b[i];
    auto find = [](uint32_t k) { return ps.query_kth_prime(k); };
    OY::FASTTRANS::fast_gcd_transform<true>(a.begin(), a.end(), find);
    OY::FASTTRANS::fast_gcd_transform<true>(b.begin(), b.end(), find);
    for (uint32_t i = 0; i <= n; i++) a[i] *= b[i];
    OY::FASTTRANS::fast_gcd_transform<false>(a.begin(), a.end(), find);
    for (uint32_t i = 1; i <= n; i++) cout << a[i] << ' ';
}

void solve_lcm() {
    using mint = OY::mgint998244353;
    uint32_t n;
    cin >> n;
    std::vector<mint> a(n + 1), b(n + 1);
    for (uint32_t i = 1; i <= n; i++) cin >> a[i];
    for (uint32_t i = 1; i <= n; i++) cin >> b[i];
    auto find = [](uint32_t k) { return ps.query_kth_prime(k); };
    OY::FASTTRANS::fast_lcm_transform<true>(a.begin(), a.end(), find);
    OY::FASTTRANS::fast_lcm_transform<true>(b.begin(), b.end(), find);
    for (uint32_t i = 0; i <= n; i++) a[i] *= b[i];
    OY::FASTTRANS::fast_lcm_transform<false>(a.begin(), a.end(), find);
    for (uint32_t i = 1; i <= n; i++) cout << a[i] << ' ';
}

int main(){
    solve_gcd();
    // solve_lcm();
}