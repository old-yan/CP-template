#include "IO/FastIO.h"
#include "MATH/Cipolla.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicMontgomeryModInt32.h"

/*
[P5491 【模板】二次剩余](https://www.luogu.com.cn/problem/P5491)
*/
/**
 * 本题为奇质数模数下的二次剩余模板题
*/

int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        using mint = OY::DynamicModInt32<0>;
        // using mint = OY::DynamicMontgomeryModInt32<0>;
        uint32_t n, p;
        cin >> n >> p;
        mint::set_mod(p, true);
        auto a = OY::Cipolla<mint>(n);
        if (a)
            cout << a << ' ' << p - a << '\n';
        else
            cout << "Hola!\n";
    }
}