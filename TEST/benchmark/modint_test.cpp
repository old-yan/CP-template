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
void test_998244353_sum(const char* name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp sum = 0;
    for (uint32_t i = 0; i < 50000000; i++) {
        sum += i;
    }
    timer_end;
    cout << "time cost = " << duration_get << ", sum = " << sum << endl;
}

template <typename Tp>
void test_998244353_prod(const char* name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp prod = 1;
    for (uint32_t i = 1; i <= 10000000; i++) prod *= i;
    timer_end;
    cout << "time cost = " << duration_get << ", prod = " << prod << endl;
}

template <typename Tp>
void test_4611686018427388039_sum(const char* name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp sum = 0;
    for (uint32_t i = 0; i < 50000000; i++) sum += i;
    timer_end;
    cout << "time cost = " << duration_get << ", sum = " << sum << endl;
}

template <typename Tp>
void test_4611686018427388039_prod(const char* name) {
    cout << "test of " << name << ":\t";
    timer_start;
    Tp prod = 1;
    for (uint32_t i = 1; i <= 10000000; i++) prod *= i;
    timer_end;
    cout << "time cost = " << duration_get << ", prod = " << prod << endl;
}

int main() {
    uint32_t P = 998244353;
    test_998244353_sum<OY::mint998244353>("static  mint32, sum over 998244353");
    test_998244353_sum<OY::StaticModInt64<998244353, true>>("static  mint64, sum over 998244353");
    test_998244353_sum<OY::mgint998244353>("s_mg    mint32, sum over 998244353");
    test_998244353_sum<OY::StaticMontgomeryModInt64<998244353, true>>("s_mg    mint64, sum over 998244353");

    if (rr() == 0) P += rr();
    OY::DynamicModInt32<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicModInt32<1>>("dynamic mint32, sum over 998244353");
    OY::DynamicModInt64<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicModInt64<1>>("dynamic mint64, sum over 998244353");
    OY::DynamicMontgomeryModInt32<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicMontgomeryModInt32<1>>("d_mg    mint32, sum over 998244353");
    OY::DynamicMontgomeryModInt64<1>::set_mod(P, true);
    test_998244353_sum<OY::DynamicMontgomeryModInt64<1>>("d_mg    mint64, sum over 998244353");

    test_998244353_prod<OY::mint998244353>("static  mint32, prod over 998244353");
    test_998244353_prod<OY::StaticModInt64<998244353, true>>("static  mint64, prod over 998244353");
    test_998244353_prod<OY::mgint998244353>("s_mg    mint32, prod over 998244353");
    test_998244353_prod<OY::StaticMontgomeryModInt64<998244353, true>>("s_mg    mint64, prod over 998244353");

    if (rr() == 0) P += rr();
    OY::DynamicModInt32<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicModInt32<1>>("dynamic mint32, prod over 998244353");
    OY::DynamicModInt64<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicModInt64<1>>("dynamic mint64, prod over 998244353");
    OY::DynamicMontgomeryModInt32<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicMontgomeryModInt32<1>>("d_mg    mint32, prod over 998244353");
    OY::DynamicMontgomeryModInt64<1>::set_mod(P, true);
    test_998244353_prod<OY::DynamicMontgomeryModInt64<1>>("d_mg    mint64, prod over 998244353");

    uint64_t P2 = 4611686018427388039;
    test_4611686018427388039_sum<OY::mint4611686018427388039>("static  mint64, sum over 4611686018427388039");
    test_4611686018427388039_sum<OY::mgint4611686018427388039>("s_mg    mint64, sum over 4611686018427388039");
    if (rr() == 0) P2 += rr();
    OY::DynamicModInt64<2>::set_mod(P2, true);
    test_4611686018427388039_sum<OY::DynamicModInt64<2>>("dynamic mint64, sum over 4611686018427388039");
    OY::DynamicMontgomeryModInt64<2>::set_mod(P2, true);
    test_4611686018427388039_sum<OY::DynamicMontgomeryModInt64<2>>("d_mg    mint64, sum over 4611686018427388039");

    test_4611686018427388039_prod<OY::mint4611686018427388039>("static  mint64, prod over 4611686018427388039");
    test_4611686018427388039_prod<OY::mgint4611686018427388039>("s_mg    mint64, prod over 4611686018427388039");
    if (rr() == 0) P2 += rr();
    OY::DynamicModInt64<2>::set_mod(P2, true);
    test_4611686018427388039_prod<OY::DynamicModInt64<2>>("dynamic mint64, prod over 4611686018427388039");
    OY::DynamicMontgomeryModInt64<2>::set_mod(P2, true);
    test_4611686018427388039_prod<OY::DynamicMontgomeryModInt64<2>>("d_mg    mint64, prod over 4611686018427388039");
}
/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
test of static  mint32, sum over 998244353:	time cost = 48, sum = 392662106
test of static  mint64, sum over 998244353:	time cost = 47, sum = 392662106
test of s_mg    mint32, sum over 998244353:	time cost = 53, sum = 392662106
test of s_mg    mint64, sum over 998244353:	time cost = 53, sum = 392662106
test of dynamic mint32, sum over 998244353:	time cost = 55, sum = 392662106
test of dynamic mint64, sum over 998244353:	time cost = 59, sum = 392662106
test of d_mg    mint32, sum over 998244353:	time cost = 114, sum = 392662106
test of d_mg    mint64, sum over 998244353:	time cost = 114, sum = 392662106
test of static  mint32, prod over 998244353:	time cost = 28, prod = 295201906
test of static  mint64, prod over 998244353:	time cost = 112, prod = 295201906
test of s_mg    mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of s_mg    mint64, prod over 998244353:	time cost = 30, prod = 295201906
test of dynamic mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of dynamic mint64, prod over 998244353:	time cost = 112, prod = 295201906
test of d_mg    mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of d_mg    mint64, prod over 998244353:	time cost = 32, prod = 295201906
test of static  mint64, sum over 4611686018427388039:	time cost = 46, sum = 1249999975000000
test of s_mg    mint64, sum over 4611686018427388039:	time cost = 46, sum = 1249999975000000
test of dynamic mint64, sum over 4611686018427388039:	time cost = 55, sum = 1249999975000000
test of d_mg    mint64, sum over 4611686018427388039:	time cost = 115, sum = 1249999975000000
test of static  mint64, prod over 4611686018427388039:	time cost = 111, prod = 3349637021238779782
test of s_mg    mint64, prod over 4611686018427388039:	time cost = 35, prod = 3349637021238779782
test of dynamic mint64, prod over 4611686018427388039:	time cost = 115, prod = 3349637021238779782
test of d_mg    mint64, prod over 4611686018427388039:	time cost = 34, prod = 3349637021238779782

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
test of static  mint32, sum over 998244353:	time cost = 48, sum = 392662106
test of static  mint64, sum over 998244353:	time cost = 54, sum = 392662106
test of s_mg    mint32, sum over 998244353:	time cost = 51, sum = 392662106
test of s_mg    mint64, sum over 998244353:	time cost = 45, sum = 392662106
test of dynamic mint32, sum over 998244353:	time cost = 108, sum = 392662106
test of dynamic mint64, sum over 998244353:	time cost = 91, sum = 392662106
test of d_mg    mint32, sum over 998244353:	time cost = 136, sum = 392662106
test of d_mg    mint64, sum over 998244353:	time cost = 121, sum = 392662106
test of static  mint32, prod over 998244353:	time cost = 27, prod = 295201906
test of static  mint64, prod over 998244353:	time cost = 139, prod = 295201906
test of s_mg    mint32, prod over 998244353:	time cost = 31, prod = 295201906
test of s_mg    mint64, prod over 998244353:	time cost = 33, prod = 295201906
test of dynamic mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of dynamic mint64, prod over 998244353:	time cost = 146, prod = 295201906
test of d_mg    mint32, prod over 998244353:	time cost = 36, prod = 295201906
test of d_mg    mint64, prod over 998244353:	time cost = 35, prod = 295201906
test of static  mint64, sum over 4611686018427388039:	time cost = 67, sum = 1249999975000000
test of s_mg    mint64, sum over 4611686018427388039:	time cost = 49, sum = 1249999975000000
test of dynamic mint64, sum over 4611686018427388039:	time cost = 96, sum = 1249999975000000
test of d_mg    mint64, sum over 4611686018427388039:	time cost = 123, sum = 1249999975000000
test of static  mint64, prod over 4611686018427388039:	time cost = 144, prod = 3349637021238779782
test of s_mg    mint64, prod over 4611686018427388039:	time cost = 33, prod = 3349637021238779782
test of dynamic mint64, prod over 4611686018427388039:	time cost = 148, prod = 3349637021238779782
test of d_mg    mint64, prod over 4611686018427388039:	time cost = 35, prod = 3349637021238779782

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
test of static  mint32, sum over 998244353:	time cost = 75, sum = 392662106
test of static  mint64, sum over 998244353:	time cost = 50, sum = 392662106
test of s_mg    mint32, sum over 998244353:	time cost = 97, sum = 392662106
test of s_mg    mint64, sum over 998244353:	time cost = 108, sum = 392662106
test of dynamic mint32, sum over 998244353:	time cost = 61, sum = 392662106
test of dynamic mint64, sum over 998244353:	time cost = 49, sum = 392662106
test of d_mg    mint32, sum over 998244353:	time cost = 119, sum = 392662106
test of d_mg    mint64, sum over 998244353:	time cost = 142, sum = 392662106
test of static  mint32, prod over 998244353:	time cost = 28, prod = 295201906
test of static  mint64, prod over 998244353:	time cost = 86, prod = 295201906
test of s_mg    mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of s_mg    mint64, prod over 998244353:	time cost = 34, prod = 295201906
test of dynamic mint32, prod over 998244353:	time cost = 26, prod = 295201906
test of dynamic mint64, prod over 998244353:	time cost = 85, prod = 295201906
test of d_mg    mint32, prod over 998244353:	time cost = 33, prod = 295201906
test of d_mg    mint64, prod over 998244353:	time cost = 32, prod = 295201906
test of static  mint64, sum over 4611686018427388039:	time cost = 265, sum = 1249999975000000
test of s_mg    mint64, sum over 4611686018427388039:	time cost = 326, sum = 1249999975000000
test of dynamic mint64, sum over 4611686018427388039:	time cost = 37, sum = 1249999975000000
test of d_mg    mint64, sum over 4611686018427388039:	time cost = 92, sum = 1249999975000000
test of static  mint64, prod over 4611686018427388039:	time cost = 280, prod = 3349637021238779782
test of s_mg    mint64, prod over 4611686018427388039:	time cost = 93, prod = 3349637021238779782
test of dynamic mint64, prod over 4611686018427388039:	time cost = 212, prod = 3349637021238779782
test of d_mg    mint64, prod over 4611686018427388039:	time cost = 43, prod = 3349637021238779782

*/