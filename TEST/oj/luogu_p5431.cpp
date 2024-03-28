#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicMontgomeryModInt32.h"
#include "MATH/ModularInverseTable.h"

/*
[P5431 【模板】模意义下的乘法逆元 2](https://www.luogu.com.cn/problem/P5431)
*/
/**
 *本题为逆元打表模板题
 */

int main() {
    using mint = OY::DynamicMontgomeryModInt32<0>;
    // using mint = OY::DynamicModInt32<0>;
    uint32_t n, p, k;
    cin >> n >> p >> k;
    mint::set_mod(p, true);
    std::vector<mint> input(n);
    mint kinv = mint::raw(k).inv(), kk = mint::raw(1);
    for (auto &a : input) cin >> a, a *= (kk *= kinv);

    mint ans = 0;
    auto res = OY::get_modular_inverse_table<mint>(input.begin(), input.end());
    for (auto &inv : res) ans += inv;
    cout << ans;
}