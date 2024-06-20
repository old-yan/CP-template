#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

/*
[Factorize](https://judge.yosupo.jp/problem/factorize)(https://github.com/yosupo06/library-checker-problems/issues/41)
*/
/**
 * 本题为质因数分解模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    uint64_t buf[64];
    while (t--) {
        uint64_t x;
        cin >> x;
        uint32_t cnt{};
        OY::PollardRho::enumerate_prime_factors(x, [&](auto p) {
            buf[cnt++] = p;
        });
        std::sort(buf, buf + cnt);
        cout << cnt;
        for (uint32_t i = 0; i != cnt; i++) cout << ' ' << buf[i];
        cout << endl;
    }
}