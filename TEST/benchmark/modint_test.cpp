/*
本文件在 C++20 标准下编译运行，比较各种模数类的效率高低
*/
#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicModInt64.h"
#include "MATH/DynamicMontgomeryModInt32.h"
#include "MATH/DynamicMontgomeryModInt64.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticModInt64.h"
#include "MATH/StaticMontgomeryModInt32.h"
#include "MATH/StaticMontgomeryModInt64.h"

#include <chrono>
#include <random>

std::mt19937_64 rr;
std::chrono::high_resolution_clock::time_point t0, t1;
#define timer_start t0 = std::chrono::high_resolution_clock::now()
#define timer_end t1 = std::chrono::high_resolution_clock::now()
#define duration_get std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()

template <typename Tp>
void test_998244353_sum(const char *name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp sum = 0;
    for (uint32_t i = 0; i < 300000000; i++) {
        sum += Tp::raw(i);
    }
    timer_end;
    cout << "time cost = " << duration_get << ", sum = " << sum << endl;
}

template <typename Tp>
void test_998244353_prod(const char *name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp prod = 1;
    for (uint32_t i = 1; i <= 50000000; i++) {
        prod *= Tp::raw(i);
    }
    timer_end;
    cout << "time cost = " << duration_get << ", prod = " << prod << endl;
}

template <typename Tp>
void test_4611686018427387847_sum(const char *name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp sum = 0;
    for (uint32_t i = 0; i < 300000000; i++) {
        sum += Tp::raw(i);
    }
    timer_end;
    cout << "time cost = " << duration_get << ", sum = " << sum << endl;
}

template <typename Tp>
void test_4611686018427387847_prod(const char *name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp prod = 1;
    for (uint32_t i = 1; i <= 50000000; i++) {
        prod *= Tp::raw(i);
    }
    timer_end;
    cout << "time cost = " << duration_get << ", prod = " << prod << endl;
}

int main() {
    uint32_t P = 998244353;
    test_998244353_sum<OY::mint998244353>("static   mint32, sum over 998244353");
    test_998244353_sum<OY::StaticModInt64<998244353, true>>("static   mint64, sum over 998244353");
    test_998244353_sum<OY::mgint998244353>("static  mgint32, sum over 998244353");
    test_998244353_sum<OY::StaticMontgomeryModInt64<998244353, true>>("static  mgint64, sum over 998244353");
    cout << endl;

    if (rr() == 0) P += rr();
    OY::DynamicModInt32<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicModInt32<1>>("dynamic  mint32, sum over 998244353");
    OY::DynamicModInt64<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicModInt64<1>>("dynamic  mint64, sum over 998244353");
    OY::DynamicMontgomeryModInt32<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicMontgomeryModInt32<1>>("dynamic mgint32, sum over 998244353");
    OY::DynamicMontgomeryModInt64<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicMontgomeryModInt64<1>>("dynamic mgint64, sum over 998244353");
    cout << endl;

    test_998244353_prod<OY::mint998244353>("static   mint32, prod over 998244353");
    test_998244353_prod<OY::StaticModInt64<998244353, true>>("static   mint64, prod over 998244353");
    test_998244353_prod<OY::mgint998244353>("static  mgint32, prod over 998244353");
    test_998244353_prod<OY::StaticMontgomeryModInt64<998244353, true>>("static  mgint64, prod over 998244353");
    cout << endl;

    if (rr() == 0) P += rr();
    OY::DynamicModInt32<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicModInt32<1>>("dynamic  mint32, prod over 998244353");
    OY::DynamicModInt64<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicModInt64<1>>("dynamic  mint64, prod over 998244353");
    OY::DynamicMontgomeryModInt32<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicMontgomeryModInt32<1>>("dynamic mgint32, prod over 998244353");
    OY::DynamicMontgomeryModInt64<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicMontgomeryModInt64<1>>("dynamic mgint64, prod over 998244353");
    cout << endl;

    uint64_t P2 = 4611686018427387847;
    test_4611686018427387847_sum<OY::mint4611686018427387847>("static   mint64, sum over 4611686018427387847");
    test_4611686018427387847_sum<OY::mgint4611686018427387847>("static  mgint64, sum over 4611686018427387847");
    if (rr() == 0) P2 += rr();
    OY::DynamicModInt64<2>::set_mod(P2, true);
    test_4611686018427387847_sum<OY::DynamicModInt64<2>>("dynamic  mint64, sum over 4611686018427387847");
    OY::DynamicMontgomeryModInt64<2>::set_mod(P2, true);
    test_4611686018427387847_sum<OY::DynamicMontgomeryModInt64<2>>("dynamic mgint64, sum over 4611686018427387847");
    cout << endl;

    test_4611686018427387847_prod<OY::mint4611686018427387847>("static   mint64, prod over 4611686018427387847");
    test_4611686018427387847_prod<OY::mgint4611686018427387847>("static  mgint64, prod over 4611686018427387847");
    if (rr() == 0) P2 += rr();
    OY::DynamicModInt64<2>::set_mod(P2, true);
    test_4611686018427387847_prod<OY::DynamicModInt64<2>>("dynamic  mint64, prod over 4611686018427387847");
    OY::DynamicMontgomeryModInt64<2>::set_mod(P2, true);
    test_4611686018427387847_prod<OY::DynamicMontgomeryModInt64<2>>("dynamic mgint64, prod over 4611686018427387847");
}
/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
test of static   mint32, sum over 998244353:	time cost = 299, sum = 910414874
test of static   mint64, sum over 998244353:	time cost = 289, sum = 910414874
test of static  mgint32, sum over 998244353:	time cost = 234, sum = 910414874
test of static  mgint64, sum over 998244353:	time cost = 329, sum = 910414874

test of dynamic  mint32, sum over 998244353:	time cost = 290, sum = 910414874
test of dynamic  mint64, sum over 998244353:	time cost = 290, sum = 910414874
test of dynamic mgint32, sum over 998244353:	time cost = 233, sum = 910414874
test of dynamic mgint64, sum over 998244353:	time cost = 250, sum = 910414874

test of static   mint32, prod over 998244353:	time cost = 143, prod = 213689172
test of static   mint64, prod over 998244353:	time cost = 546, prod = 213689172
test of static  mgint32, prod over 998244353:	time cost = 127, prod = 213689172
test of static  mgint64, prod over 998244353:	time cost = 139, prod = 213689172

test of dynamic  mint32, prod over 998244353:	time cost = 162, prod = 213689172
test of dynamic  mint64, prod over 998244353:	time cost = 556, prod = 213689172
test of dynamic mgint32, prod over 998244353:	time cost = 139, prod = 213689172
test of dynamic mgint64, prod over 998244353:	time cost = 172, prod = 213689172

test of static   mint64, sum over 4611686018427387847:	time cost = 285, sum = 44999999850000000
test of static  mgint64, sum over 4611686018427387847:	time cost = 210, sum = 44999999850000000
test of dynamic  mint64, sum over 4611686018427387847:	time cost = 283, sum = 44999999850000000
test of dynamic mgint64, sum over 4611686018427387847:	time cost = 249, sum = 44999999850000000

test of static   mint64, prod over 4611686018427387847:	time cost = 540, prod = 4380181483440239289
test of static  mgint64, prod over 4611686018427387847:	time cost = 131, prod = 4380181483440239289
test of dynamic  mint64, prod over 4611686018427387847:	time cost = 543, prod = 4380181483440239289
test of dynamic mgint64, prod over 4611686018427387847:	time cost = 170, prod = 4380181483440239289

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
test of static   mint32, sum over 998244353:	time cost = 307, sum = 910414874
test of static   mint64, sum over 998244353:	time cost = 305, sum = 910414874
test of static  mgint32, sum over 998244353:	time cost = 324, sum = 910414874
test of static  mgint64, sum over 998244353:	time cost = 387, sum = 910414874

test of dynamic  mint32, sum over 998244353:	time cost = 147, sum = 910414874
test of dynamic  mint64, sum over 998244353:	time cost = 139, sum = 910414874
test of dynamic mgint32, sum over 998244353:	time cost = 232, sum = 910414874
test of dynamic mgint64, sum over 998244353:	time cost = 308, sum = 910414874

test of static   mint32, prod over 998244353:	time cost = 152, prod = 213689172
test of static   mint64, prod over 998244353:	time cost = 740, prod = 213689172
test of static  mgint32, prod over 998244353:	time cost = 135, prod = 213689172
test of static  mgint64, prod over 998244353:	time cost = 133, prod = 213689172

test of dynamic  mint32, prod over 998244353:	time cost = 178, prod = 213689172
test of dynamic  mint64, prod over 998244353:	time cost = 719, prod = 213689172
test of dynamic mgint32, prod over 998244353:	time cost = 129, prod = 213689172
test of dynamic mgint64, prod over 998244353:	time cost = 129, prod = 213689172

test of static   mint64, sum over 4611686018427387847:	time cost = 362, sum = 44999999850000000
test of static  mgint64, sum over 4611686018427387847:	time cost = 299, sum = 44999999850000000
test of dynamic  mint64, sum over 4611686018427387847:	time cost = 129, sum = 44999999850000000
test of dynamic mgint64, sum over 4611686018427387847:	time cost = 296, sum = 44999999850000000

test of static   mint64, prod over 4611686018427387847:	time cost = 729, prod = 4380181483440239289
test of static  mgint64, prod over 4611686018427387847:	time cost = 134, prod = 4380181483440239289
test of dynamic  mint64, prod over 4611686018427387847:	time cost = 732, prod = 4380181483440239289
test of dynamic mgint64, prod over 4611686018427387847:	time cost = 128, prod = 4380181483440239289

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
test of static   mint32, sum over 998244353:	time cost = 286, sum = 910414874
test of static   mint64, sum over 998244353:	time cost = 280, sum = 910414874
test of static  mgint32, sum over 998244353:	time cost = 359, sum = 910414874
test of static  mgint64, sum over 998244353:	time cost = 542, sum = 910414874

test of dynamic  mint32, sum over 998244353:	time cost = 157, sum = 910414874
test of dynamic  mint64, sum over 998244353:	time cost = 156, sum = 910414874
test of dynamic mgint32, sum over 998244353:	time cost = 260, sum = 910414874
test of dynamic mgint64, sum over 998244353:	time cost = 526, sum = 910414874

test of static   mint32, prod over 998244353:	time cost = 154, prod = 213689172
test of static   mint64, prod over 998244353:	time cost = 440, prod = 213689172
test of static  mgint32, prod over 998244353:	time cost = 130, prod = 213689172
test of static  mgint64, prod over 998244353:	time cost = 150, prod = 213689172

test of dynamic  mint32, prod over 998244353:	time cost = 131, prod = 213689172
test of dynamic  mint64, prod over 998244353:	time cost = 441, prod = 213689172
test of dynamic mgint32, prod over 998244353:	time cost = 129, prod = 213689172
test of dynamic mgint64, prod over 998244353:	time cost = 149, prod = 213689172

test of static   mint64, sum over 4611686018427387847:	time cost = 275, sum = 44999999850000000
test of static  mgint64, sum over 4611686018427387847:	time cost = 509, sum = 44999999850000000
test of dynamic  mint64, sum over 4611686018427387847:	time cost = 115, sum = 44999999850000000
test of dynamic mgint64, sum over 4611686018427387847:	time cost = 543, sum = 44999999850000000

test of static   mint64, prod over 4611686018427387847:	time cost = 1083, prod = 4380181483440239289
test of static  mgint64, prod over 4611686018427387847:	time cost = 152, prod = 4380181483440239289
test of dynamic  mint64, prod over 4611686018427387847:	time cost = 1068, prod = 4380181483440239289
test of dynamic mgint64, prod over 4611686018427387847:	time cost = 150, prod = 4380181483440239289

*/