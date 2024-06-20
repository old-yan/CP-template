#include "IO/FastIO.h"
#include "MATH/PrimeCheck.h"

/*
[Primality Test](https://judge.yosupo.jp/problem/primality_test)(https://github.com/yosupo06/library-checker-problems/issues/880)
*/
/**
 * 素性检验模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint64_t x;
        cin >> x;
        cout << (OY::is_prime64(x) ? "Yes\n" : "No\n");
    }
}
