#include "IO/FastIO.h"
#include "MATH/DynamicMontgomeryModInt64.h"
#include "MATH/PrimitiveRoot.h"

/*
[Primitive Root](https://judge.yosupo.jp/problem/primitive_root)(https://github.com/yosupo06/library-checker-problems/issues/851)
*/

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint64_t x;
        cin >> x;
        if (x == 2)
            cout << "1\n";
        else {
            using mint = OY::DynamicMontgomeryModInt64<0>;
            mint::set_mod(x, true);
            cout << OY::PROOT::PollardRhoSolver<mint>().query_random() << endl;
        }
    }
}