/*
本文件在 C++20 标准下编译运行，比较标准库 <bit> 头文件函数，std::gcd ，std::lcm 与模板库里的替代函数的效率高低
*/
#include <chrono>
#include <numeric>
#include <random>

#include "IO/FastIO.h"
#include "TEST/std_bit.h"
#include "TEST/std_gcd_lcm.h"

static constexpr size_t N = 1 << 20, M = 100;
uint32_t small[N], middle[N], big[N];
std::mt19937_64 rr;
std::chrono::high_resolution_clock::time_point t0, t1;
uint64_t sum;
std::string to_string_of_len(const char* s, size_t len) {
    std::string res(s);
    if (res.size() > 5 and res.substr(0, 5) == "std::") res = res.substr(5);
    while (res.size() < len) res += ' ';
    return res;
}
#define timer_start t0 = std::chrono::high_resolution_clock::now()
#define timer_end t1 = std::chrono::high_resolution_clock::now()
#define duration_get std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
#define test_bit_op(func, arr)           \
    timer_start;                         \
    sum = 0;                             \
    for (size_t i = 0; i < M; i++) {     \
        for (size_t i = 0; i < N; i++) { \
            sum += func(arr[i]);         \
        }                                \
    }                                    \
    timer_end;                           \
    cout << to_string_of_len(#func, 28) << " of " << to_string_of_len(#arr, 6) << ", time cost = " << duration_get << "ms, sum = " << sum << endl;

#define test_gcd_lcm(func, arr)              \
    timer_start;                             \
    sum = 0;                                 \
    for (size_t i = 0; i < M / 10; i++) {    \
        for (size_t i = 1; i < N; i++) {     \
            sum += func(arr[i - 1], arr[i]); \
        }                                    \
    }                                        \
    timer_end;                               \
    cout << to_string_of_len(#func, 28) << " of " << to_string_of_len(#arr, 6) << ", time cost = " << duration_get << "ms, sum = " << sum << endl;

#define test_bit_op_on_three_arr(func) \
    test_bit_op(func, small);          \
    test_bit_op(func, middle);         \
    test_bit_op(func, big);

int main() {
    for (size_t i = 0; i < N; i++) small[i] = rr() % (1 << 10);
    for (size_t i = 0; i < N; i++) middle[i] = rr() % (1 << 20);
    for (size_t i = 0; i < N; i++) big[i] = rr() % (1 << 30);

    test_bit_op_on_three_arr(std::my_bit_ops::bit_ceil_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::bit_ceil);
    test_bit_op_on_three_arr(std::bit_ceil);
    cout << std::bit_ceil(0u) << ' ' << std::my_bit_ops::bit_ceil(0) << ' ' << std::my_bit_ops::bit_ceil_msvc(0) << '\n';
    cout << std::bit_ceil(1u) << ' ' << std::my_bit_ops::bit_ceil(1) << ' ' << std::my_bit_ops::bit_ceil_msvc(1) << '\n';

    test_bit_op_on_three_arr(std::my_bit_ops::bit_floor_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::bit_floor);
    test_bit_op_on_three_arr(std::bit_floor);
    cout << std::bit_floor(0u) << ' ' << std::my_bit_ops::bit_floor(0) << ' ' << std::my_bit_ops::bit_floor_msvc(0) << '\n';

    test_bit_op_on_three_arr(std::my_bit_ops::bit_width_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::bit_width);
    test_bit_op_on_three_arr(std::bit_width);
    cout << std::bit_width(0u) << ' ' << std::my_bit_ops::bit_width(0) << ' ' << std::my_bit_ops::bit_width_msvc(0) << '\n';

    test_bit_op_on_three_arr(std::my_bit_ops::countl_zero_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::countl_zero);
    test_bit_op_on_three_arr(std::countl_zero);
    cout << std::countl_zero(0u) << ' ' << std::my_bit_ops::countl_zero(0) << ' ' << std::my_bit_ops::countl_zero_msvc(0) << '\n';

    test_bit_op_on_three_arr(std::my_bit_ops::countr_zero_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::countr_zero);
    test_bit_op_on_three_arr(std::countr_zero);
    cout << std::countr_zero(0u) << ' ' << std::my_bit_ops::countr_zero(0) << ' ' << std::my_bit_ops::countr_zero_msvc(0) << '\n';

    test_bit_op_on_three_arr(std::my_bit_ops::popcount_msvc);
    test_bit_op_on_three_arr(std::my_bit_ops::popcount);
    test_bit_op_on_three_arr(std::popcount);

    test_gcd_lcm(std::gcd, small);
    test_gcd_lcm(std::gcd, middle);
    test_gcd_lcm(std::gcd, big);
    test_gcd_lcm(std::my_gcd_lcm::gcd, small);
    test_gcd_lcm(std::my_gcd_lcm::gcd, middle);
    test_gcd_lcm(std::my_gcd_lcm::gcd, big);

    test_gcd_lcm(std::lcm, small);
    test_gcd_lcm(std::lcm, middle);
    test_gcd_lcm(std::lcm, big);
    test_gcd_lcm(std::my_gcd_lcm::lcm, small);
    test_gcd_lcm(std::my_gcd_lcm::lcm, middle);
    test_gcd_lcm(std::my_gcd_lcm::lcm, big);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
my_bit_ops::bit_ceil_msvc    of small , time cost = 286ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 163ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 126ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 78ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 73ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 73ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 76ms, sum = 71518895600
bit_ceil                     of middle, time cost = 73ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 74ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 283ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 149ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 107ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 61ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 62ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 61ms, sum = 37578809449267200
bit_floor                    of small , time cost = 68ms, sum = 35811501600
bit_floor                    of middle, time cost = 71ms, sum = 36653091029800
bit_floor                    of big   , time cost = 67ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 262ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 138ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 99ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 67ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 68ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 67ms, sum = 3041242600
bit_width                    of small , time cost = 58ms, sum = 943961400
bit_width                    of middle, time cost = 58ms, sum = 1992283200
bit_width                    of big   , time cost = 58ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 257ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 144ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 95ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 50ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 36ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 37ms, sum = 314200600
countl_zero                  of small , time cost = 36ms, sum = 2411481800
countl_zero                  of middle, time cost = 37ms, sum = 1363160000
countl_zero                  of big   , time cost = 35ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 67ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 67ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 68ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 35ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 50ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 35ms, sum = 104815800
countr_zero                  of small , time cost = 35ms, sum = 107138500
countr_zero                  of middle, time cost = 36ms, sum = 105190000
countr_zero                  of big   , time cost = 36ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 48ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 49ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 52ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 39ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 39ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 41ms, sum = 1572767300
popcount                     of small , time cost = 42ms, sum = 524234500
popcount                     of middle, time cost = 58ms, sum = 1048579900
popcount                     of big   , time cost = 41ms, sum = 1572767300
gcd                          of small , time cost = 283ms, sum = 57261010
gcd                          of middle, time cost = 576ms, sum = 88032230
gcd                          of big   , time cost = 970ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 282ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 601ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 857ms, sum = 100688520
lcm                          of small , time cost = 330ms, sum = 2006341506350
lcm                          of middle, time cost = 589ms, sum = 22149968141117530
lcm                          of big   , time cost = 851ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 316ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 561ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 813ms, sum = 22526523237941540

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2 -march=native
my_bit_ops::bit_ceil_msvc    of small , time cost = 335ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 236ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 131ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 89ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 77ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 75ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 85ms, sum = 71518895600
bit_ceil                     of middle, time cost = 80ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 77ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 386ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 167ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 118ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 61ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 56ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 61ms, sum = 37578809449267200
bit_floor                    of small , time cost = 57ms, sum = 35811501600
bit_floor                    of middle, time cost = 63ms, sum = 36653091029800
bit_floor                    of big   , time cost = 66ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 333ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 138ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 90ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 53ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 55ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 52ms, sum = 3041242600
bit_width                    of small , time cost = 43ms, sum = 943961400
bit_width                    of middle, time cost = 40ms, sum = 1992283200
bit_width                    of big   , time cost = 40ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 320ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 159ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 110ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 35ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 39ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 40ms, sum = 314200600
countl_zero                  of small , time cost = 31ms, sum = 2411481800
countl_zero                  of middle, time cost = 28ms, sum = 1363160000
countl_zero                  of big   , time cost = 32ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 53ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 52ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 52ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 55ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 55ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 51ms, sum = 104815800
countr_zero                  of small , time cost = 68ms, sum = 107138500
countr_zero                  of middle, time cost = 70ms, sum = 105190000
countr_zero                  of big   , time cost = 82ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 68ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 56ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 75ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 58ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 59ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 60ms, sum = 1572767300
popcount                     of small , time cost = 58ms, sum = 524234500
popcount                     of middle, time cost = 59ms, sum = 1048579900
popcount                     of big   , time cost = 62ms, sum = 1572767300
gcd                          of small , time cost = 197ms, sum = 57261010
gcd                          of middle, time cost = 378ms, sum = 88032230
gcd                          of big   , time cost = 554ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 184ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 356ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 543ms, sum = 100688520
lcm                          of small , time cost = 205ms, sum = 2006341506350
lcm                          of middle, time cost = 393ms, sum = 22149968141117530
lcm                          of big   , time cost = 555ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 201ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 372ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 540ms, sum = 22526523237941540


*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
my_bit_ops::bit_ceil_msvc    of small , time cost = 454ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 280ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 217ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 409ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 249ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 199ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 153ms, sum = 71518895600
bit_ceil                     of middle, time cost = 186ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 234ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 343ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 224ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 195ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 369ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 225ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 171ms, sum = 37578809449267200
bit_floor                    of small , time cost = 158ms, sum = 35811501600
bit_floor                    of middle, time cost = 154ms, sum = 36653091029800
bit_floor                    of big   , time cost = 155ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 306ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 185ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 148ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 314ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 180ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 143ms, sum = 3041242600
bit_width                    of small , time cost = 114ms, sum = 943961400
bit_width                    of middle, time cost = 112ms, sum = 1992283200
bit_width                    of big   , time cost = 110ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 331ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 204ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 143ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 319ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 196ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 142ms, sum = 314200600
countl_zero                  of small , time cost = 115ms, sum = 2411481800
countl_zero                  of middle, time cost = 115ms, sum = 1363160000
countl_zero                  of big   , time cost = 117ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 127ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 151ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 129ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 129ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 130ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 150ms, sum = 104815800
countr_zero                  of small , time cost = 126ms, sum = 107138500
countr_zero                  of middle, time cost = 129ms, sum = 105190000
countr_zero                  of big   , time cost = 141ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 161ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 168ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 169ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 166ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 166ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 164ms, sum = 1572767300
popcount                     of small , time cost = 139ms, sum = 524234500
popcount                     of middle, time cost = 160ms, sum = 1048579900
popcount                     of big   , time cost = 176ms, sum = 1572767300
gcd                          of small , time cost = 209ms, sum = 57261010
gcd                          of middle, time cost = 376ms, sum = 88032230
gcd                          of big   , time cost = 558ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 200ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 378ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 535ms, sum = 100688520
lcm                          of small , time cost = 248ms, sum = 2006341506350
lcm                          of middle, time cost = 422ms, sum = 22149968141117530
lcm                          of big   , time cost = 590ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 218ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 403ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 554ms, sum = 22526523237941540

*/
