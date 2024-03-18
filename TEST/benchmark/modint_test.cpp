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
    cout << name << ":\t";
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
    cout << name << ":\t";
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
    cout << name << ":\t";
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
    cout << name << ":\t";
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
static   mint32, sum over 998244353:	time cost = 315, sum = 910414874
static   mint64, sum over 998244353:	time cost = 286, sum = 910414874
static  mgint32, sum over 998244353:	time cost = 230, sum = 910414874
static  mgint64, sum over 998244353:	time cost = 325, sum = 910414874

dynamic  mint32, sum over 998244353:	time cost = 290, sum = 910414874
dynamic  mint64, sum over 998244353:	time cost = 278, sum = 910414874
dynamic mgint32, sum over 998244353:	time cost = 232, sum = 910414874
dynamic mgint64, sum over 998244353:	time cost = 250, sum = 910414874

static   mint32, prod over 998244353:	time cost = 142, prod = 213689172
static   mint64, prod over 998244353:	time cost = 549, prod = 213689172
static  mgint32, prod over 998244353:	time cost = 138, prod = 213689172
static  mgint64, prod over 998244353:	time cost = 152, prod = 213689172

dynamic  mint32, prod over 998244353:	time cost = 173, prod = 213689172
dynamic  mint64, prod over 998244353:	time cost = 578, prod = 213689172
dynamic mgint32, prod over 998244353:	time cost = 147, prod = 213689172
dynamic mgint64, prod over 998244353:	time cost = 179, prod = 213689172

static   mint64, sum over 4611686018427387847:	time cost = 303, sum = 44999999850000000
static  mgint64, sum over 4611686018427387847:	time cost = 215, sum = 44999999850000000
dynamic  mint64, sum over 4611686018427387847:	time cost = 290, sum = 44999999850000000
dynamic mgint64, sum over 4611686018427387847:	time cost = 254, sum = 44999999850000000

static   mint64, prod over 4611686018427387847:	time cost = 529, prod = 4380181483440239289
static  mgint64, prod over 4611686018427387847:	time cost = 124, prod = 4380181483440239289
dynamic  mint64, prod over 4611686018427387847:	time cost = 539, prod = 4380181483440239289
dynamic mgint64, prod over 4611686018427387847:	time cost = 171, prod = 4380181483440239289

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
static   mint32, sum over 998244353:	time cost = 299, sum = 910414874
static   mint64, sum over 998244353:	time cost = 298, sum = 910414874
static  mgint32, sum over 998244353:	time cost = 325, sum = 910414874
static  mgint64, sum over 998244353:	time cost = 391, sum = 910414874

dynamic  mint32, sum over 998244353:	time cost = 150, sum = 910414874
dynamic  mint64, sum over 998244353:	time cost = 140, sum = 910414874
dynamic mgint32, sum over 998244353:	time cost = 235, sum = 910414874
dynamic mgint64, sum over 998244353:	time cost = 313, sum = 910414874

static   mint32, prod over 998244353:	time cost = 145, prod = 213689172
static   mint64, prod over 998244353:	time cost = 760, prod = 213689172
static  mgint32, prod over 998244353:	time cost = 140, prod = 213689172
static  mgint64, prod over 998244353:	time cost = 142, prod = 213689172

dynamic  mint32, prod over 998244353:	time cost = 197, prod = 213689172
dynamic  mint64, prod over 998244353:	time cost = 754, prod = 213689172
dynamic mgint32, prod over 998244353:	time cost = 126, prod = 213689172
dynamic mgint64, prod over 998244353:	time cost = 127, prod = 213689172

static   mint64, sum over 4611686018427387847:	time cost = 380, sum = 44999999850000000
static  mgint64, sum over 4611686018427387847:	time cost = 319, sum = 44999999850000000
dynamic  mint64, sum over 4611686018427387847:	time cost = 132, sum = 44999999850000000
dynamic mgint64, sum over 4611686018427387847:	time cost = 297, sum = 44999999850000000

static   mint64, prod over 4611686018427387847:	time cost = 714, prod = 4380181483440239289
static  mgint64, prod over 4611686018427387847:	time cost = 128, prod = 4380181483440239289
dynamic  mint64, prod over 4611686018427387847:	time cost = 719, prod = 4380181483440239289
dynamic mgint64, prod over 4611686018427387847:	time cost = 135, prod = 4380181483440239289

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
static   mint32, sum over 998244353:	time cost = 284, sum = 910414874
static   mint64, sum over 998244353:	time cost = 291, sum = 910414874
static  mgint32, sum over 998244353:	time cost = 377, sum = 910414874
static  mgint64, sum over 998244353:	time cost = 598, sum = 910414874

dynamic  mint32, sum over 998244353:	time cost = 152, sum = 910414874
dynamic  mint64, sum over 998244353:	time cost = 166, sum = 910414874
dynamic mgint32, sum over 998244353:	time cost = 268, sum = 910414874
dynamic mgint64, sum over 998244353:	time cost = 555, sum = 910414874

static   mint32, prod over 998244353:	time cost = 140, prod = 213689172
static   mint64, prod over 998244353:	time cost = 427, prod = 213689172
static  mgint32, prod over 998244353:	time cost = 129, prod = 213689172
static  mgint64, prod over 998244353:	time cost = 151, prod = 213689172

dynamic  mint32, prod over 998244353:	time cost = 130, prod = 213689172
dynamic  mint64, prod over 998244353:	time cost = 426, prod = 213689172
dynamic mgint32, prod over 998244353:	time cost = 132, prod = 213689172
dynamic mgint64, prod over 998244353:	time cost = 149, prod = 213689172

static   mint64, sum over 4611686018427387847:	time cost = 267, sum = 44999999850000000
static  mgint64, sum over 4611686018427387847:	time cost = 511, sum = 44999999850000000
dynamic  mint64, sum over 4611686018427387847:	time cost = 117, sum = 44999999850000000
dynamic mgint64, sum over 4611686018427387847:	time cost = 583, sum = 44999999850000000

static   mint64, prod over 4611686018427387847:	time cost = 1063, prod = 4380181483440239289
static  mgint64, prod over 4611686018427387847:	time cost = 151, prod = 4380181483440239289
dynamic  mint64, prod over 4611686018427387847:	time cost = 1050, prod = 4380181483440239289
dynamic mgint64, prod over 4611686018427387847:	time cost = 149, prod = 4380181483440239289

*/