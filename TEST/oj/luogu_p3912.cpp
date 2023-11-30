#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/EulerSieve.h"
#include "MATH/FastSieve.h"

/*
[P3912 素数个数](https://www.luogu.com.cn/problem/P3912)
*/
/**
 * 本题为素数筛模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::SIEVE::FastSieve<100000100> ps(n + 100);
    // OY::Eratosthenes::Sieve<100000100> ps(n + 100);
    // OY::EulerSieve::Sieve<100000100> ps(n + 100);
    uint32_t ans = 0;
    while (ps.query_kth_prime(ans) <= n) ans++;
    cout << ans;
}
