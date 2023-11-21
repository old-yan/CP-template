#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/LucasTable.h"

int main() {
    using mint = OY::DynamicModInt32<0>;
    // 设置模数为 10007
    mint::set_mod(10007, false);
    cout << mint::mod() << endl;

    OY::LucasTable<mint> table;

    // 求组合数 C(15, 2) mod 10007
    auto a = table.query(15, 2);
    cout << "Comb(15, 2) mod " << mint::mod() << " ≡ " << a << endl;

    // 求组合数 C(1515, 222) mod 10007
    auto b = table.query(1515, 222);
    cout << "Comb(1515, 222) mod " << mint::mod() << " ≡ " << b << endl;
    
    // 求组合数 C(1515151515151515, 222222222222) mod 10007
    auto c = table.query(1515151515151515, 222222222222);
    cout << "Comb(1515151515151515, 222222222222) mod " << mint::mod() << " ≡ " << c << endl;
}
/*
#输出如下
10007
Comb(15, 2) mod 10007 ≡ 105
Comb(1515, 222) mod 10007 ≡ 3179
Comb(1515151515151515, 222222222222) mod 10007 ≡ 4153

*/