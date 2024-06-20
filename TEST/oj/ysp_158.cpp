#include "IO/FastIO.h"
#include "MATH/FastSieve.h"

/*
[Enumerate Primes](https://judge.yosupo.jp/problem/enumerate_primes)(https://github.com/yosupo06/library-checker-problems/issues/158)
*/
/**
 * 筛模板题
 */

uint32_t n, a, b;
//推荐在全局构造这个模板，避免局部空间的分配
auto s = [] {
    cin >> n >> a >> b;
    return OY::FASTSIEVE::Sieve<500000000>(n);
}();
int main() {
    uint32_t x = s.count() < b + 1 ? 0 : (s.count() - 1 - b) / a + 1;
    cout << s.count() << ' ' << x << endl;
    for (uint32_t i = 0, j = b; i != x; i++, j += a)
        cout << s.query_kth_prime(j) << ' ';
}
