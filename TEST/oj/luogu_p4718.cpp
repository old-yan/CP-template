#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

/*
[P4718 【模板】Pollard-Rho](https://www.luogu.com.cn/problem/P4718)
*/
/**
 * 本题为 Pollard Rho 算法模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint64_t x, mx{}, cnt{};
        cin >> x;
        OY::PollardRho::enumerate_prime_factors(x, [&](auto p) {
            mx = std::max(mx, p);
            cnt++;
        });
        if (cnt == 1)
            cout << "Prime\n";
        else
            cout << mx << endl;
    }
}