#include "IO/FastIO.h"
#include "MATH/Cipolla.h"
#include "MATH/DynamicMontgomeryModInt32.h"

/*
[Sqrt Mod](https://judge.yosupo.jp/problem/sqrt_mod)(https://github.com/yosupo06/library-checker-problems/issues/27)
*/
/**
 * 本题为二次剩余模板
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t y, p;
        cin >> y >> p;
        if (y < 2)
            cout << y << endl;
        else {
            using mint = OY::DynamicMontgomeryModInt32<0>;
            mint::set_mod(p, true);
            auto res = OY::Cipolla(mint(y));
            if (res)
                cout << res << endl;
            else
                cout << "-1\n";
        }
    }
}