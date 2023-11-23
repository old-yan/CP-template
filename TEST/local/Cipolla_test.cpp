#include "IO/FastIO.h"
#include "MATH/Cipolla.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::StaticModInt32<7, true>;
    auto x = OY::Cipolla<mint>(2);
    cout << x << " * " << x << " ≡ " << mint(x) * x << " mod " << mint::mod() << "\n";
    cout << mint::mod() - x << " * " << mint::mod() - x << " ≡ " << mint(mint::mod() - x) * (mint::mod() - x) << " mod " << mint::mod() << "\n";

    auto x2 = OY::Cipolla<mint>(3);
    if (x2) {
        cout << "3 is quadratic residue mod " << mint::mod() << "\n";
    } else {
        cout << "3 is not quadratic residue mod " << mint::mod() << "\n";
    }
}
/*
#输出如下
3 * 3 ≡ 2 mod 7
4 * 4 ≡ 2 mod 7
3 is not quadratic residue mod 7

*/