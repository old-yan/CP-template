#include "IO/FastIO.h"
#include "MATH/FastSieve.h"

/*
[Enumerate Primes](https://judge.yosupo.jp/problem/enumerate_primes)(https://github.com/yosupo06/library-checker-problems/issues/158)
*/
/**
 * 筛模板题
 */

OY::FASTSIEVE::Sieve<500000000> s{0};
int main() {
    uint32_t n, a, b;
    cin >> n >> a >> b;
    s.resize(n);
    uint32_t x = s.count() < b + 1 ? 0 : (s.count() - 1 - b) / a + 1;
    cout << s.count() << ' ' << x << endl;
    for (uint32_t i = 0, j = b; i != x; i++, j += a)
        cout << s.query_kth_prime(j) << ' ';
}
