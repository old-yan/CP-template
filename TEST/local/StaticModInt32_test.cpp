#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

#include <random>

void test_Fermat_1000000007() {
    // 验证费马定理
    using mint = OY::StaticModInt32<1000000007, true>;
    mint prod(1);
    for (auto i = 1; i != mint::mod() - 1; i++) {
        prod *= i;
    }
    cout << mint::mod() - 2 << "! mod " << mint::mod() << " = " << prod << endl;
}

void test_Fermat_998244353() {
    // 验证费马定理
    using mint = OY::StaticModInt32<998244353, true>;
    mint prod(1);
    for (auto i = 1; i != mint::mod() - 1; i++) {
        prod *= i;
    }
    cout << mint::mod() - 2 << "! mod " << mint::mod() << " = " << prod << endl;
}

void test_random() {
    // 验证区间内随机数的和
    // 模数取个小点的偶数
    using mint = OY::StaticModInt32<16, false>;
    std::mt19937_64 rand;
    long long sum_ll = 0;
    mint sum_mint;
    uint64_t start = rand() % (1ull << 40), len = 100001;
    for (uint64_t a = 0; a < len; a++) {
        sum_ll += start + a;
        sum_mint += start + a;
    }
    cout << "sum from " << start << " to " << start + len - 1 << " = " << sum_ll << endl;
    cout << "sum from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << sum_mint << endl;
}

int main() {
    test_Fermat_1000000007();
    test_Fermat_998244353();
    test_random();
}
/*
#输出如下
1000000005! mod 1000000007 = 1
998244351! mod 998244353 = 1
sum from 124402445990 to 124402545990 = 12440374001495990
sum from 124402445990 to 124402545990 mod 16 = 6

*/