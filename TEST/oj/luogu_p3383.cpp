#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/EulerSieve.h"
#include "MATH/FastSieve.h"

/*
[P3383 【模板】线性筛素数](https://www.luogu.com.cn/problem/P3383)
*/
/**
 * 本题为质数筛模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::SIEVE::FastSieve<100000000> ps(n);
    // OY::Eratosthenes::Sieve<100000000> ps(n);
    // OY::EulerSieve::Sieve<100000000> ps(n);
    while (q--) {
        uint32_t k;
        cin >> k;
        cout << ps.query_kth_prime(k - 1) << endl;
    }
}