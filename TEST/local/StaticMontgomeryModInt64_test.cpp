#include "IO/FastIO.h"
#include "MATH/StaticMontgomeryModInt64.h"

#include <random>

void test_Fermat_10000019() {
    // 验证费马定理
    using mint = OY::StaticMontgomeryModInt64<10000019, true>;
    mint prod(1);
    for (auto i = 1; i != mint::mod() - 1; i++) {
        prod *= i;
    }
    cout << mint::mod() - 2 << "! mod " << mint::mod() << " = " << prod << endl;
}

void test_small_modint() {
    // 验证区间内随机数的和
    // 模数取个小点的奇数
    using mint = OY::StaticMontgomeryModInt64<75, false>;
    std::mt19937_64 rand;
    long long sum_ll = 0;
    mint sum_mint;
    uint64_t start = rand() % (1ull << 40), len = 10000001;
    for (uint64_t a = 0; a < len; a++) {
        sum_ll += start + a;
        sum_mint += start + a;
    }
    cout << "sum from " << start << " to " << start + len - 1 << " = " << sum_ll << endl;
    cout << "sum from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << sum_mint << endl;
}

void test_big_modint() {
    // 计算区间内随机数的乘积
    // 模数取个大点的质数
    // 验证就算了，可以用 python 验证
    using mint = OY::mgint4611686018427387847;
    std::mt19937_64 rand;
    mint prod = 1;
    uint64_t start = rand(), len = 10000001;
    for (uint64_t a = 0; a < len; a++)
        prod *= start + a;
    cout << "prod from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << prod << endl;
}

int main() {
    test_Fermat_10000019();
    test_small_modint();
    test_big_modint();
}
/*
#输出如下
10000017! mod 10000019 = 1
sum from 124402445990 to 124412445990 = 1244074584307445990
sum from 124402445990 to 124412445990 mod 75 = 65
prod from 14514284786278117030 to 14514284786288117030 mod 4611686018427387847 = 986469850098377379

*/