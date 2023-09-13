#include <bit>
#include <chrono>
#include <numeric>
#include <random>

#include "IO/FastIO.h"
#include "TEST/mystd.h"

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
g++ -std=c++20 -DOY_LOCAL -O2
my_bit_ops::bit_ceil_msvc    of small , time cost = 331ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 194ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 153ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 109ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 105ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 106ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 108ms, sum = 71518895600
bit_ceil                     of middle, time cost = 105ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 106ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 306ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 184ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 133ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 84ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 82ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 84ms, sum = 37578809449267200
bit_floor                    of small , time cost = 124ms, sum = 35811501600
bit_floor                    of middle, time cost = 120ms, sum = 36653091029800
bit_floor                    of big   , time cost = 119ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 276ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 146ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 101ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 83ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 102ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 99ms, sum = 3041242600
bit_width                    of small , time cost = 79ms, sum = 943961400
bit_width                    of middle, time cost = 82ms, sum = 1992283200
bit_width                    of big   , time cost = 117ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 309ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 170ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 118ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 84ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 79ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 74ms, sum = 314200600
countl_zero                  of small , time cost = 80ms, sum = 2411481800
countl_zero                  of middle, time cost = 80ms, sum = 1363160000
countl_zero                  of big   , time cost = 86ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 98ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 94ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 90ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 75ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 75ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 75ms, sum = 104815800
countr_zero                  of small , time cost = 73ms, sum = 107138500
countr_zero                  of middle, time cost = 74ms, sum = 105190000
countr_zero                  of big   , time cost = 70ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 130ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 128ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 127ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 231ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 228ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 229ms, sum = 1572767300
popcount                     of small , time cost = 228ms, sum = 524234500
popcount                     of middle, time cost = 226ms, sum = 1048579900
popcount                     of big   , time cost = 228ms, sum = 1572767300
gcd                          of small , time cost = 286ms, sum = 57261010
gcd                          of middle, time cost = 573ms, sum = 88032230
gcd                          of big   , time cost = 844ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 293ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 555ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 820ms, sum = 100688520
lcm                          of small , time cost = 299ms, sum = 2006341506350
lcm                          of middle, time cost = 571ms, sum = 22149968141117530
lcm                          of big   , time cost = 834ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 296ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 573ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 832ms, sum = 22526523237941540

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
my_bit_ops::bit_ceil_msvc    of small , time cost = 358ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 187ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 139ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 106ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 103ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 115ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 105ms, sum = 71518895600
bit_ceil                     of middle, time cost = 112ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 104ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 312ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 163ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 124ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 90ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 90ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 91ms, sum = 37578809449267200
bit_floor                    of small , time cost = 117ms, sum = 35811501600
bit_floor                    of middle, time cost = 117ms, sum = 36653091029800
bit_floor                    of big   , time cost = 117ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 301ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 141ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 87ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 68ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 67ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 67ms, sum = 3041242600
bit_width                    of small , time cost = 65ms, sum = 943961400
bit_width                    of middle, time cost = 66ms, sum = 1992283200
bit_width                    of big   , time cost = 64ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 311ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 149ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 104ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 59ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 63ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 59ms, sum = 314200600
countl_zero                  of small , time cost = 60ms, sum = 2411481800
countl_zero                  of middle, time cost = 59ms, sum = 1363160000
countl_zero                  of big   , time cost = 62ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 87ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 88ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 88ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 56ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 66ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 59ms, sum = 104815800
countr_zero                  of small , time cost = 53ms, sum = 107138500
countr_zero                  of middle, time cost = 53ms, sum = 105190000
countr_zero                  of big   , time cost = 51ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 86ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 88ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 85ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 70ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 67ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 66ms, sum = 1572767300
popcount                     of small , time cost = 67ms, sum = 524234500
popcount                     of middle, time cost = 67ms, sum = 1048579900
popcount                     of big   , time cost = 67ms, sum = 1572767300
gcd                          of small , time cost = 199ms, sum = 57261010
gcd                          of middle, time cost = 372ms, sum = 88032230
gcd                          of big   , time cost = 537ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 197ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 373ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 546ms, sum = 100688520
lcm                          of small , time cost = 219ms, sum = 2006341506350
lcm                          of middle, time cost = 389ms, sum = 22149968141117530
lcm                          of big   , time cost = 570ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 267ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 426ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 593ms, sum = 22526523237941540

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
my_bit_ops::bit_ceil_msvc    of small , time cost = 422ms, sum = 71518895600
my_bit_ops::bit_ceil_msvc    of middle, time cost = 256ms, sum = 73306096264000
my_bit_ops::bit_ceil_msvc    of big   , time cost = 193ms, sum = 75157618898534400
my_bit_ops::bit_ceil         of small , time cost = 406ms, sum = 71518895600
my_bit_ops::bit_ceil         of middle, time cost = 243ms, sum = 73306096264000
my_bit_ops::bit_ceil         of big   , time cost = 191ms, sum = 75157618898534400
bit_ceil                     of small , time cost = 188ms, sum = 71518895600
bit_ceil                     of middle, time cost = 163ms, sum = 73306096264000
bit_ceil                     of big   , time cost = 189ms, sum = 75157618898534400
1 1 1
1 1 1
my_bit_ops::bit_floor_msvc   of small , time cost = 353ms, sum = 35811501600
my_bit_ops::bit_floor_msvc   of middle, time cost = 210ms, sum = 36653091029800
my_bit_ops::bit_floor_msvc   of big   , time cost = 174ms, sum = 37578809449267200
my_bit_ops::bit_floor        of small , time cost = 359ms, sum = 35811501600
my_bit_ops::bit_floor        of middle, time cost = 221ms, sum = 36653091029800
my_bit_ops::bit_floor        of big   , time cost = 174ms, sum = 37578809449267200
bit_floor                    of small , time cost = 156ms, sum = 35811501600
bit_floor                    of middle, time cost = 160ms, sum = 36653091029800
bit_floor                    of big   , time cost = 156ms, sum = 37578809449267200
0 0 0
my_bit_ops::bit_width_msvc   of small , time cost = 319ms, sum = 943961400
my_bit_ops::bit_width_msvc   of middle, time cost = 227ms, sum = 1992283200
my_bit_ops::bit_width_msvc   of big   , time cost = 143ms, sum = 3041242600
my_bit_ops::bit_width        of small , time cost = 332ms, sum = 943961400
my_bit_ops::bit_width        of middle, time cost = 206ms, sum = 1992283200
my_bit_ops::bit_width        of big   , time cost = 167ms, sum = 3041242600
bit_width                    of small , time cost = 116ms, sum = 943961400
bit_width                    of middle, time cost = 131ms, sum = 1992283200
bit_width                    of big   , time cost = 117ms, sum = 3041242600
0 0 0
my_bit_ops::countl_zero_msvc of small , time cost = 319ms, sum = 2411481800
my_bit_ops::countl_zero_msvc of middle, time cost = 183ms, sum = 1363160000
my_bit_ops::countl_zero_msvc of big   , time cost = 145ms, sum = 314200600
my_bit_ops::countl_zero      of small , time cost = 324ms, sum = 2411481800
my_bit_ops::countl_zero      of middle, time cost = 181ms, sum = 1363160000
my_bit_ops::countl_zero      of big   , time cost = 140ms, sum = 314200600
countl_zero                  of small , time cost = 117ms, sum = 2411481800
countl_zero                  of middle, time cost = 113ms, sum = 1363160000
countl_zero                  of big   , time cost = 116ms, sum = 314200600
32 32 32
my_bit_ops::countr_zero_msvc of small , time cost = 133ms, sum = 107138500
my_bit_ops::countr_zero_msvc of middle, time cost = 121ms, sum = 105190000
my_bit_ops::countr_zero_msvc of big   , time cost = 131ms, sum = 104815800
my_bit_ops::countr_zero      of small , time cost = 122ms, sum = 107138500
my_bit_ops::countr_zero      of middle, time cost = 132ms, sum = 105190000
my_bit_ops::countr_zero      of big   , time cost = 121ms, sum = 104815800
countr_zero                  of small , time cost = 153ms, sum = 107138500
countr_zero                  of middle, time cost = 129ms, sum = 105190000
countr_zero                  of big   , time cost = 156ms, sum = 104815800
32 32 32
my_bit_ops::popcount_msvc    of small , time cost = 165ms, sum = 524234500
my_bit_ops::popcount_msvc    of middle, time cost = 166ms, sum = 1048579900
my_bit_ops::popcount_msvc    of big   , time cost = 165ms, sum = 1572767300
my_bit_ops::popcount         of small , time cost = 168ms, sum = 524234500
my_bit_ops::popcount         of middle, time cost = 165ms, sum = 1048579900
my_bit_ops::popcount         of big   , time cost = 165ms, sum = 1572767300
popcount                     of small , time cost = 129ms, sum = 524234500
popcount                     of middle, time cost = 164ms, sum = 1048579900
popcount                     of big   , time cost = 130ms, sum = 1572767300
gcd                          of small , time cost = 214ms, sum = 57261010
gcd                          of middle, time cost = 402ms, sum = 88032230
gcd                          of big   , time cost = 565ms, sum = 100688520
my_gcd_lcm::gcd              of small , time cost = 203ms, sum = 57261010
my_gcd_lcm::gcd              of middle, time cost = 371ms, sum = 88032230
my_gcd_lcm::gcd              of big   , time cost = 547ms, sum = 100688520
lcm                          of small , time cost = 241ms, sum = 2006341506350
lcm                          of middle, time cost = 413ms, sum = 22149968141117530
lcm                          of big   , time cost = 603ms, sum = 22526523237941540
my_gcd_lcm::lcm              of small , time cost = 221ms, sum = 2006341506350
my_gcd_lcm::lcm              of middle, time cost = 395ms, sum = 22149968141117530
my_gcd_lcm::lcm              of big   , time cost = 576ms, sum = 22526523237941540

*/
