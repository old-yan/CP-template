#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/LucasTable_ex.h"

int main() {
    using mint = OY::DynamicModInt32<0>;
    // 设置模数为 7265082
    mint::set_mod(2 * 3 * 11 * 11 * 10007, false);
    cout << mint::mod() << endl;

    OY::LucasTable_ex<mint> table;

    // 求组合数 C(15, 2) mod 7265082
    auto a = table.query(15, 2);
    cout << "Comb(15, 2) mod " << mint::mod() << " ≡ " << a << endl;

    // 求组合数 C(1515, 222) mod 7265082
    auto b = table.query(1515, 222);
    cout << "Comb(1515, 222) mod " << mint::mod() << " ≡ " << b << endl;
    
    // 求组合数 C(1515151515151515, 222222222222) mod 7265082
    auto c = table.query(1515151515151515, 222222222222);
    cout << "Comb(1515151515151515, 222222222222) mod " << mint::mod() << " ≡ " << c << endl;
}
/*
#输出如下
7265082
Comb(15, 2) mod 7265082 ≡ 105
Comb(1515, 222) mod 7265082 ≡ 113256
Comb(1515151515151515, 222222222222) mod 7265082 ≡ 4757478

*/