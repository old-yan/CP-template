#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"

#include <random>

void test() {
    // 计算区间内随机数的和
    using mint = OY::mintu32;
    std::mt19937_64 rand;
    mint sum = {};
    uint64_t start = rand(), len = 10000001;
    for (uint64_t a = 0; a < len; a++)
        sum += start + a;
    cout << "sum from " << start << " to " << start + len - 1 << " mod 4294967296 = " << sum << endl;
}

int main() {
    test();
}
/*
#输出如下
sum from 14514284786278117030 to 14514284786288117030 mod 4294967296 = 935112934

*/