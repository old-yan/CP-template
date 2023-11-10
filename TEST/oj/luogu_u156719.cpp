#include "DS/IntegerSet.h"

/*
[U156719 Dynamic Predecessor Problem](https://www.luogu.com.cn/problem/U156719)
*/
/**
 * 本题为数字集寻找前驱后继的模板题
*/

#include <cstdio>
namespace GenHelper {
    unsigned z3, z4, b;
    inline unsigned rand_() {
        z3 = ((z3 & 4294967280U) << 7) ^ ((z3 << 13) ^ z3) >> 21;
        z4 = ((z4 & 4294967168U) << 13) ^ ((z4 << 3) ^ z4) >> 12;
        return (z3 ^ z4);
    }
}
inline void srand_(unsigned x) {
    using namespace GenHelper;
    z3 = x ^ 0x1234598766U;
    z4 = (~x) + 51;
}
int ans;
OY::INTSET::Table<30> S;
inline void work() {
    int x = GenHelper::rand_() & ((1 << 30) - 1), op = (GenHelper::rand_() >> 15) & 3;
    /* Your code begins here */
    if (op == 0)
        S.set(x);
    else if (op == 1)
        S.reset(x);
    else if (op == 2) {
        auto y = S.smaller_bound(x);
        if (~y) ans ^= y;
    } else {
        auto y = S.upper_bound(x);
        if (~y) ans ^= y;
    }
}
int n, seed;
int main() {
    scanf("%d%d", &n, &seed);
    srand_(seed);
    while (n--) work();
    printf("%d\n", ans);
}