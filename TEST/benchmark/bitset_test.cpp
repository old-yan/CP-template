/*
本文件在 C++20 标准下编译运行，比较 std::bitset 与模板库里的 Bitset 效率高低
*/
#include <bitset>
#include <chrono>
#include <random>

#include "DS/Bitset.h"
#include "IO/FastIO.h"

using std::bitset;
static constexpr size_t size1 = 100, size2 = 100000;
static constexpr char name[2][10] = {"sparse", "dense "};
bool arr1[2][size1], arr2[2][size2];
std::mt19937_64 rr;
std::chrono::high_resolution_clock::time_point t0, t1;
uint64_t sum;
std::string to_string_of_len(const char *s, size_t len) {
    std::string res(s);
    if (res.size() > 5 and res.substr(0, 5) == "std::") res = res.substr(5);
    while (res.size() < len) res += ' ';
    return res;
}
#define timer_start t0 = std::chrono::high_resolution_clock::now()
#define timer_end t1 = std::chrono::high_resolution_clock::now()
#define duration_get std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
#define test_bitset_visit(Set, First, Next, End, Size, i, M)                                                                                                                   \
    {                                                                                                                                                                          \
        Set<size##Size> B;                                                                                                                                                     \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                               \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                     \
        sum = 0;                                                                                                                                                               \
        timer_start;                                                                                                                                                           \
        for (size_t k = 0; k != M; k++)                                                                                                                                        \
            for (auto j = B.First(); End; j = B.Next(j)) sum += j;                                                                                                             \
        timer_end;                                                                                                                                                             \
        cout << to_string_of_len(#Set, 8) << "'s visit " << name[i] << " array of size(" << size##Size << ") *" << M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_bitset_op(Set, op, Size, i, M)                                                                                                                                             \
    {                                                                                                                                                                                   \
        Set<size##Size> B;                                                                                                                                                              \
        size_t sum = 0;                                                                                                                                                                 \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                                        \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                              \
        timer_start;                                                                                                                                                                    \
        for (size_t j = 0, k = rr(); j != M; j++) {                                                                                                                                     \
            B.op();                                                                                                                                                                     \
            if (!k) B.reset(0), B.set(1);                                                                                                                                               \
            sum += B[0] + B[1];                                                                                                                                                         \
        }                                                                                                                                                                               \
        timer_end;                                                                                                                                                                      \
        cout << to_string_of_len(#Set, 8) << "'s " << #op << " " << name[i] << " array of size(" << size##Size << ") * " << M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_bitset_count(Set, Size, i, M)                                                                                                                                      \
    {                                                                                                                                                                           \
        Set<size##Size> B;                                                                                                                                                      \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                                \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                      \
        timer_start;                                                                                                                                                            \
        sum = 0;                                                                                                                                                                \
        for (size_t j = 0; j != M; j++) sum += B.count();                                                                                                                       \
        timer_end;                                                                                                                                                              \
        cout << to_string_of_len(#Set, 8) << "'s count " << name[i] << " array of size(" << size##Size << ") * " << M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_bitset_left_shift(Set, Size, i, M, X)                                                                                                                                            \
    {                                                                                                                                                                                         \
        Set<size##Size> B;                                                                                                                                                                    \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                                              \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                                    \
        timer_start;                                                                                                                                                                          \
        sum = 1;                                                                                                                                                                              \
        for (size_t j = 0; j != M; j++) {                                                                                                                                                     \
            sum += (B << X)[X];                                                                                                                                                               \
            if (sum) B.flip(0);                                                                                                                                                               \
        }                                                                                                                                                                                     \
        timer_end;                                                                                                                                                                            \
        cout << to_string_of_len(#Set, 8) << "'s l_shift " << X << " " << name[i] << " array of size(" << size##Size << ") * " << M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_bitset_right_shift(Set, Size, i, M, X)                                                                                                                                           \
    {                                                                                                                                                                                         \
        Set<size##Size> B;                                                                                                                                                                    \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                                              \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                                    \
        timer_start;                                                                                                                                                                          \
        sum = 1;                                                                                                                                                                              \
        for (size_t j = 0; j != M; j++) {                                                                                                                                                     \
            sum += (B >> X)[0];                                                                                                                                                               \
            if (sum) B.flip(X);                                                                                                                                                               \
        }                                                                                                                                                                                     \
        timer_end;                                                                                                                                                                            \
        cout << to_string_of_len(#Set, 8) << "'s r_shift " << X << " " << name[i] << " array of size(" << size##Size << ") * " << M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
template <uint32_t N>
using Bitset64 = OY::Bitset::Table<N>;
void test_sparse_small() {
    // sparse, small size
    test_bitset_visit(Bitset64, first_one, next_one, ~j, 1, 0, 1000000);
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, 1, 0, 1000000);
#ifndef _MSC_VER
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), 1, 0, 1000000);
#endif
    test_bitset_count(Bitset64, 1, 0, 10000000);
    test_bitset_count(bitset, 1, 0, 10000000);
    test_bitset_op(Bitset64, set, 1, 0, 100000000);
    test_bitset_op(bitset, set, 1, 0, 100000000);
    test_bitset_op(Bitset64, reset, 1, 0, 100000000);
    test_bitset_op(bitset, reset, 1, 0, 100000000);
    test_bitset_op(Bitset64, flip, 1, 0, 100000000);
    test_bitset_op(bitset, flip, 1, 0, 100000000);
    test_bitset_left_shift(Bitset64, 1, 0, 10000000, 1);
    test_bitset_left_shift(bitset, 1, 0, 10000000, 1);
    test_bitset_left_shift(Bitset64, 1, 0, 10000000, 64);
    test_bitset_left_shift(bitset, 1, 0, 10000000, 64);
    test_bitset_left_shift(Bitset64, 1, 0, 10000000, 99);
    test_bitset_left_shift(bitset, 1, 0, 10000000, 99);
    test_bitset_right_shift(Bitset64, 1, 0, 10000000, 1);
    test_bitset_right_shift(bitset, 1, 0, 10000000, 1);
    test_bitset_right_shift(Bitset64, 1, 0, 10000000, 64);
    test_bitset_right_shift(bitset, 1, 0, 10000000, 64);
    test_bitset_right_shift(Bitset64, 1, 0, 10000000, 99);
    test_bitset_right_shift(bitset, 1, 0, 10000000, 99);
}
void test_sparse_big() {
    // sparse, big size
    test_bitset_visit(Bitset64, first_one, next_one, ~j, 2, 0, 1000);
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, 2, 0, 1000);
#ifndef _MSC_VER
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), 2, 0, 1000);
#endif
    test_bitset_count(Bitset64, 2, 0, 10000);
    test_bitset_count(bitset, 2, 0, 10000);
    test_bitset_op(Bitset64, set, 2, 0, 100000);
    test_bitset_op(bitset, set, 2, 0, 100000);
    test_bitset_op(Bitset64, reset, 2, 0, 100000);
    test_bitset_op(bitset, reset, 2, 0, 100000);
    test_bitset_op(Bitset64, flip, 2, 0, 100000);
    test_bitset_op(bitset, flip, 2, 0, 100000);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 1);
    test_bitset_left_shift(bitset, 2, 0, 100000, 1);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 64);
    test_bitset_left_shift(bitset, 2, 0, 100000, 64);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 100);
    test_bitset_left_shift(bitset, 2, 0, 100000, 100);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 200);
    test_bitset_left_shift(bitset, 2, 0, 100000, 200);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 500);
    test_bitset_left_shift(bitset, 2, 0, 100000, 500);
    test_bitset_left_shift(Bitset64, 2, 0, 100000, 10000);
    test_bitset_left_shift(bitset, 2, 0, 100000, 10000);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 1);
    test_bitset_right_shift(bitset, 2, 0, 100000, 1);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 64);
    test_bitset_right_shift(bitset, 2, 0, 100000, 64);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 100);
    test_bitset_right_shift(bitset, 2, 0, 100000, 100);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 200);
    test_bitset_right_shift(bitset, 2, 0, 100000, 200);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 500);
    test_bitset_right_shift(bitset, 2, 0, 100000, 500);
    test_bitset_right_shift(Bitset64, 2, 0, 100000, 10000);
    test_bitset_right_shift(bitset, 2, 0, 100000, 10000);
}
void test_dense_small() {
    // dense, small size
    test_bitset_visit(Bitset64, first_one, next_one, ~j, 1, 1, 100000);
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, 1, 1, 100000);
#ifndef _MSC_VER
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), 1, 1, 100000);
#endif
    test_bitset_count(Bitset64, 1, 1, 10000000);
    test_bitset_count(bitset, 1, 1, 10000000);
    test_bitset_op(Bitset64, set, 1, 1, 100000000);
    test_bitset_op(bitset, set, 1, 1, 100000000);
    test_bitset_op(Bitset64, reset, 1, 1, 100000000);
    test_bitset_op(bitset, reset, 1, 1, 100000000);
    test_bitset_op(Bitset64, flip, 1, 1, 100000000);
    test_bitset_op(bitset, flip, 1, 1, 100000000);
    test_bitset_left_shift(Bitset64, 1, 1, 10000000, 1);
    test_bitset_left_shift(bitset, 1, 1, 10000000, 1);
    test_bitset_left_shift(Bitset64, 1, 1, 10000000, 64);
    test_bitset_left_shift(bitset, 1, 1, 10000000, 64);
    test_bitset_left_shift(Bitset64, 1, 1, 10000000, 99);
    test_bitset_left_shift(bitset, 1, 1, 10000000, 99);
    test_bitset_right_shift(Bitset64, 1, 1, 10000000, 1);
    test_bitset_right_shift(bitset, 1, 1, 10000000, 1);
    test_bitset_right_shift(Bitset64, 1, 1, 10000000, 64);
    test_bitset_right_shift(bitset, 1, 1, 10000000, 64);
    test_bitset_right_shift(Bitset64, 1, 1, 10000000, 99);
    test_bitset_right_shift(bitset, 1, 1, 10000000, 99);
}
void test_dense_big() {
    // dense, big size
    test_bitset_visit(Bitset64, first_one, next_one, ~j, 2, 1, 100);
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, 2, 1, 100);
#ifndef _MSC_VER
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), 2, 1, 100);
#endif
    test_bitset_count(Bitset64, 2, 1, 10000);
    test_bitset_count(bitset, 2, 1, 10000);
    test_bitset_op(Bitset64, set, 2, 1, 100000);
    test_bitset_op(bitset, set, 2, 1, 100000);
    test_bitset_op(Bitset64, reset, 2, 1, 100000);
    test_bitset_op(bitset, reset, 2, 1, 100000);
    test_bitset_op(Bitset64, flip, 2, 1, 100000);
    test_bitset_op(bitset, flip, 2, 1, 100000);

    test_bitset_left_shift(Bitset64, 2, 1, 100000, 1);
    test_bitset_left_shift(bitset, 2, 1, 100000, 1);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 64);
    test_bitset_left_shift(bitset, 2, 1, 100000, 64);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 100);
    test_bitset_left_shift(bitset, 2, 1, 100000, 100);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 200);
    test_bitset_left_shift(bitset, 2, 1, 100000, 200);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 500);
    test_bitset_left_shift(bitset, 2, 1, 100000, 500);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 10000);
    test_bitset_left_shift(bitset, 2, 1, 100000, 10000);
    test_bitset_left_shift(Bitset64, 2, 1, 100000, 90000);
    test_bitset_left_shift(bitset, 2, 1, 100000, 90000);

    test_bitset_right_shift(Bitset64, 2, 1, 100000, 1);
    test_bitset_right_shift(bitset, 2, 1, 100000, 1);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 64);
    test_bitset_right_shift(bitset, 2, 1, 100000, 64);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 100);
    test_bitset_right_shift(bitset, 2, 1, 100000, 100);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 200);
    test_bitset_right_shift(bitset, 2, 1, 100000, 200);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 500);
    test_bitset_right_shift(bitset, 2, 1, 100000, 500);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 10000);
    test_bitset_right_shift(bitset, 2, 1, 100000, 10000);
    test_bitset_right_shift(Bitset64, 2, 1, 100000, 90000);
    test_bitset_right_shift(bitset, 2, 1, 100000, 90000);
}
int main() {
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < size1; j++) arr1[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size2; j++) arr2[i][j] = i ? (rr() % 10) : !(rr() % 10);
    }

    test_sparse_small();
    test_sparse_big();
    test_dense_small();
    test_dense_big();
}
/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
Bitset64's visit sparse array of size(100) *1000000 = 11 ms, sum = 564000000
Bitset64's visit sparse array of size(100) *1000000 = 15 ms, sum = 564000000
bitset  's visit sparse array of size(100) *1000000 = 28 ms, sum = 564000000
Bitset64's count sparse array of size(100) * 10000000 = 0 ms, sum = 120000000
bitset  's count sparse array of size(100) * 10000000 = 15 ms, sum = 120000000
Bitset64's set sparse array of size(100) * 100000000 = 74 ms, sum = 200000000
bitset  's set sparse array of size(100) * 100000000 = 0 ms, sum = 200000000
Bitset64's reset sparse array of size(100) * 100000000 = 60 ms, sum = 0
bitset  's reset sparse array of size(100) * 100000000 = 48 ms, sum = 0
Bitset64's flip sparse array of size(100) * 100000000 = 114 ms, sum = 100000000
bitset  's flip sparse array of size(100) * 100000000 = 71 ms, sum = 100000000
Bitset64's l_shift 1 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's l_shift 1 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's l_shift 64 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's l_shift 64 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's l_shift 99 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's l_shift 99 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's r_shift 1 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's r_shift 1 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's r_shift 64 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's r_shift 64 sparse array of size(100) * 10000000 = 12 ms, sum = 5000001
Bitset64's r_shift 99 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's r_shift 99 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's visit sparse array of size(100000) *1000 = 38 ms, sum = 501519936000
Bitset64's visit sparse array of size(100000) *1000 = 36 ms, sum = 501519936000
bitset  's visit sparse array of size(100000) *1000 = 46 ms, sum = 501519936000
Bitset64's count sparse array of size(100000) * 10000 = 5 ms, sum = 100610000
bitset  's count sparse array of size(100000) * 10000 = 15 ms, sum = 100610000
Bitset64's set sparse array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set sparse array of size(100000) * 100000 = 10 ms, sum = 200000
Bitset64's reset sparse array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset sparse array of size(100000) * 100000 = 10 ms, sum = 0
Bitset64's flip sparse array of size(100000) * 100000 = 51 ms, sum = 100000
bitset  's flip sparse array of size(100000) * 100000 = 99 ms, sum = 100000
Bitset64's l_shift 1 sparse array of size(100000) * 100000 = 25 ms, sum = 50001
bitset  's l_shift 1 sparse array of size(100000) * 100000 = 198 ms, sum = 50001
Bitset64's l_shift 64 sparse array of size(100000) * 100000 = 43 ms, sum = 50001
bitset  's l_shift 64 sparse array of size(100000) * 100000 = 45 ms, sum = 50001
Bitset64's l_shift 100 sparse array of size(100000) * 100000 = 23 ms, sum = 50001
bitset  's l_shift 100 sparse array of size(100000) * 100000 = 23 ms, sum = 50001
Bitset64's l_shift 200 sparse array of size(100000) * 100000 = 26 ms, sum = 50001
bitset  's l_shift 200 sparse array of size(100000) * 100000 = 198 ms, sum = 50001
Bitset64's l_shift 500 sparse array of size(100000) * 100000 = 133 ms, sum = 50001
bitset  's l_shift 500 sparse array of size(100000) * 100000 = 243 ms, sum = 50001
Bitset64's l_shift 10000 sparse array of size(100000) * 100000 = 101 ms, sum = 50001
bitset  's l_shift 10000 sparse array of size(100000) * 100000 = 195 ms, sum = 50001
Bitset64's r_shift 1 sparse array of size(100000) * 100000 = 129 ms, sum = 50001
bitset  's r_shift 1 sparse array of size(100000) * 100000 = 226 ms, sum = 50001
Bitset64's r_shift 64 sparse array of size(100000) * 100000 = 42 ms, sum = 50001
bitset  's r_shift 64 sparse array of size(100000) * 100000 = 41 ms, sum = 50001
Bitset64's r_shift 100 sparse array of size(100000) * 100000 = 121 ms, sum = 50001
bitset  's r_shift 100 sparse array of size(100000) * 100000 = 219 ms, sum = 50001
Bitset64's r_shift 200 sparse array of size(100000) * 100000 = 122 ms, sum = 50001
bitset  's r_shift 200 sparse array of size(100000) * 100000 = 224 ms, sum = 50001
Bitset64's r_shift 500 sparse array of size(100000) * 100000 = 123 ms, sum = 50001
bitset  's r_shift 500 sparse array of size(100000) * 100000 = 219 ms, sum = 50001
Bitset64's r_shift 10000 sparse array of size(100000) * 100000 = 115 ms, sum = 50001
bitset  's r_shift 10000 sparse array of size(100000) * 100000 = 159 ms, sum = 50001
Bitset64's visit dense  array of size(100) *100000 = 17 ms, sum = 442900000
Bitset64's visit dense  array of size(100) *100000 = 25 ms, sum = 442900000
bitset  's visit dense  array of size(100) *100000 = 27 ms, sum = 442900000
Bitset64's count dense  array of size(100) * 10000000 = 0 ms, sum = 910000000
bitset  's count dense  array of size(100) * 10000000 = 15 ms, sum = 910000000
Bitset64's set dense  array of size(100) * 100000000 = 71 ms, sum = 200000000
bitset  's set dense  array of size(100) * 100000000 = 0 ms, sum = 200000000
Bitset64's reset dense  array of size(100) * 100000000 = 68 ms, sum = 0
bitset  's reset dense  array of size(100) * 100000000 = 48 ms, sum = 0
Bitset64's flip dense  array of size(100) * 100000000 = 109 ms, sum = 100000000
bitset  's flip dense  array of size(100) * 100000000 = 74 ms, sum = 100000000
Bitset64's l_shift 1 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's l_shift 1 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's l_shift 64 dense  array of size(100) * 10000000 = 12 ms, sum = 5000001
bitset  's l_shift 64 dense  array of size(100) * 10000000 = 12 ms, sum = 5000001
Bitset64's l_shift 99 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's l_shift 99 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's r_shift 1 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's r_shift 1 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's r_shift 64 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
bitset  's r_shift 64 dense  array of size(100) * 10000000 = 12 ms, sum = 5000001
Bitset64's r_shift 99 dense  array of size(100) * 10000000 = 12 ms, sum = 5000001
bitset  's r_shift 99 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's visit dense  array of size(100000) *100 = 31 ms, sum = 450217681100
Bitset64's visit dense  array of size(100000) *100 = 27 ms, sum = 450217681100
bitset  's visit dense  array of size(100000) *100 = 28 ms, sum = 450217681100
Bitset64's count dense  array of size(100000) * 10000 = 5 ms, sum = 900750000
bitset  's count dense  array of size(100000) * 10000 = 16 ms, sum = 900750000
Bitset64's set dense  array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set dense  array of size(100000) * 100000 = 10 ms, sum = 200000
Bitset64's reset dense  array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset dense  array of size(100000) * 100000 = 10 ms, sum = 0
Bitset64's flip dense  array of size(100000) * 100000 = 51 ms, sum = 100000
bitset  's flip dense  array of size(100000) * 100000 = 100 ms, sum = 100000
Bitset64's l_shift 1 dense  array of size(100000) * 100000 = 24 ms, sum = 50001
bitset  's l_shift 1 dense  array of size(100000) * 100000 = 202 ms, sum = 50001
Bitset64's l_shift 64 dense  array of size(100000) * 100000 = 44 ms, sum = 50001
bitset  's l_shift 64 dense  array of size(100000) * 100000 = 44 ms, sum = 50001
Bitset64's l_shift 100 dense  array of size(100000) * 100000 = 25 ms, sum = 50001
bitset  's l_shift 100 dense  array of size(100000) * 100000 = 32 ms, sum = 50001
Bitset64's l_shift 200 dense  array of size(100000) * 100000 = 24 ms, sum = 50001
bitset  's l_shift 200 dense  array of size(100000) * 100000 = 205 ms, sum = 50001
Bitset64's l_shift 500 dense  array of size(100000) * 100000 = 133 ms, sum = 50001
bitset  's l_shift 500 dense  array of size(100000) * 100000 = 246 ms, sum = 50001
Bitset64's l_shift 10000 dense  array of size(100000) * 100000 = 103 ms, sum = 50001
bitset  's l_shift 10000 dense  array of size(100000) * 100000 = 177 ms, sum = 50001
Bitset64's l_shift 90000 dense  array of size(100000) * 100000 = 32 ms, sum = 50001
bitset  's l_shift 90000 dense  array of size(100000) * 100000 = 41 ms, sum = 50001
Bitset64's r_shift 1 dense  array of size(100000) * 100000 = 123 ms, sum = 50001
bitset  's r_shift 1 dense  array of size(100000) * 100000 = 223 ms, sum = 50001
Bitset64's r_shift 64 dense  array of size(100000) * 100000 = 42 ms, sum = 50001
bitset  's r_shift 64 dense  array of size(100000) * 100000 = 42 ms, sum = 50001
Bitset64's r_shift 100 dense  array of size(100000) * 100000 = 126 ms, sum = 50001
bitset  's r_shift 100 dense  array of size(100000) * 100000 = 225 ms, sum = 50001
Bitset64's r_shift 200 dense  array of size(100000) * 100000 = 120 ms, sum = 50001
bitset  's r_shift 200 dense  array of size(100000) * 100000 = 222 ms, sum = 50001
Bitset64's r_shift 500 dense  array of size(100000) * 100000 = 124 ms, sum = 50001
bitset  's r_shift 500 dense  array of size(100000) * 100000 = 218 ms, sum = 50001
Bitset64's r_shift 10000 dense  array of size(100000) * 100000 = 115 ms, sum = 50001
bitset  's r_shift 10000 dense  array of size(100000) * 100000 = 161 ms, sum = 50001
Bitset64's r_shift 90000 dense  array of size(100000) * 100000 = 34 ms, sum = 50001
bitset  's r_shift 90000 dense  array of size(100000) * 100000 = 39 ms, sum = 50001

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
Bitset64's visit sparse array of size(100) *1000000 = 23 ms, sum = 564000000
Bitset64's visit sparse array of size(100) *1000000 = 30 ms, sum = 564000000
bitset  's visit sparse array of size(100) *1000000 = 24 ms, sum = 564000000
Bitset64's count sparse array of size(100) * 10000000 = 0 ms, sum = 120000000
bitset  's count sparse array of size(100) * 10000000 = 0 ms, sum = 120000000
Bitset64's set sparse array of size(100) * 100000000 = 279 ms, sum = 200000000
bitset  's set sparse array of size(100) * 100000000 = 46 ms, sum = 200000000
Bitset64's reset sparse array of size(100) * 100000000 = 266 ms, sum = 0
bitset  's reset sparse array of size(100) * 100000000 = 36 ms, sum = 0
Bitset64's flip sparse array of size(100) * 100000000 = 166 ms, sum = 100000000
bitset  's flip sparse array of size(100) * 100000000 = 78 ms, sum = 100000000
Bitset64's l_shift 1 sparse array of size(100) * 10000000 = 47 ms, sum = 5000001
bitset  's l_shift 1 sparse array of size(100) * 10000000 = 8 ms, sum = 5000001
Bitset64's l_shift 64 sparse array of size(100) * 10000000 = 45 ms, sum = 5000001
bitset  's l_shift 64 sparse array of size(100) * 10000000 = 37 ms, sum = 5000001
Bitset64's l_shift 99 sparse array of size(100) * 10000000 = 45 ms, sum = 5000001
bitset  's l_shift 99 sparse array of size(100) * 10000000 = 8 ms, sum = 5000001
Bitset64's r_shift 1 sparse array of size(100) * 10000000 = 54 ms, sum = 5000001
bitset  's r_shift 1 sparse array of size(100) * 10000000 = 12 ms, sum = 5000001
Bitset64's r_shift 64 sparse array of size(100) * 10000000 = 49 ms, sum = 5000001
bitset  's r_shift 64 sparse array of size(100) * 10000000 = 40 ms, sum = 5000001
Bitset64's r_shift 99 sparse array of size(100) * 10000000 = 49 ms, sum = 5000001
bitset  's r_shift 99 sparse array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's visit sparse array of size(100000) *1000 = 37 ms, sum = 501519936000
Bitset64's visit sparse array of size(100000) *1000 = 40 ms, sum = 501519936000
bitset  's visit sparse array of size(100000) *1000 = 43 ms, sum = 501519936000
Bitset64's count sparse array of size(100000) * 10000 = 4 ms, sum = 100610000
bitset  's count sparse array of size(100000) * 10000 = 14 ms, sum = 100610000
Bitset64's set sparse array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set sparse array of size(100000) * 100000 = 9 ms, sum = 200000
Bitset64's reset sparse array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset sparse array of size(100000) * 100000 = 9 ms, sum = 0
Bitset64's flip sparse array of size(100000) * 100000 = 15 ms, sum = 100000
bitset  's flip sparse array of size(100000) * 100000 = 15 ms, sum = 100000
Bitset64's l_shift 1 sparse array of size(100000) * 100000 = 71 ms, sum = 50001
bitset  's l_shift 1 sparse array of size(100000) * 100000 = 99 ms, sum = 50001
Bitset64's l_shift 64 sparse array of size(100000) * 100000 = 49 ms, sum = 50001
bitset  's l_shift 64 sparse array of size(100000) * 100000 = 34 ms, sum = 50001
Bitset64's l_shift 100 sparse array of size(100000) * 100000 = 79 ms, sum = 50001
bitset  's l_shift 100 sparse array of size(100000) * 100000 = 57 ms, sum = 50001
Bitset64's l_shift 200 sparse array of size(100000) * 100000 = 82 ms, sum = 50001
bitset  's l_shift 200 sparse array of size(100000) * 100000 = 105 ms, sum = 50001
Bitset64's l_shift 500 sparse array of size(100000) * 100000 = 80 ms, sum = 50001
bitset  's l_shift 500 sparse array of size(100000) * 100000 = 56 ms, sum = 50001
Bitset64's l_shift 10000 sparse array of size(100000) * 100000 = 51 ms, sum = 50001
bitset  's l_shift 10000 sparse array of size(100000) * 100000 = 60 ms, sum = 50001
Bitset64's r_shift 1 sparse array of size(100000) * 100000 = 39 ms, sum = 50001
bitset  's r_shift 1 sparse array of size(100000) * 100000 = 40 ms, sum = 50001
Bitset64's r_shift 64 sparse array of size(100000) * 100000 = 34 ms, sum = 50001
bitset  's r_shift 64 sparse array of size(100000) * 100000 = 36 ms, sum = 50001
Bitset64's r_shift 100 sparse array of size(100000) * 100000 = 36 ms, sum = 50001
bitset  's r_shift 100 sparse array of size(100000) * 100000 = 39 ms, sum = 50001
Bitset64's r_shift 200 sparse array of size(100000) * 100000 = 38 ms, sum = 50001
bitset  's r_shift 200 sparse array of size(100000) * 100000 = 38 ms, sum = 50001
Bitset64's r_shift 500 sparse array of size(100000) * 100000 = 36 ms, sum = 50001
bitset  's r_shift 500 sparse array of size(100000) * 100000 = 40 ms, sum = 50001
Bitset64's r_shift 10000 sparse array of size(100000) * 100000 = 34 ms, sum = 50001
bitset  's r_shift 10000 sparse array of size(100000) * 100000 = 35 ms, sum = 50001
Bitset64's visit dense  array of size(100) *100000 = 29 ms, sum = 442900000
Bitset64's visit dense  array of size(100) *100000 = 29 ms, sum = 442900000
bitset  's visit dense  array of size(100) *100000 = 30 ms, sum = 442900000
Bitset64's count dense  array of size(100) * 10000000 = 0 ms, sum = 910000000
bitset  's count dense  array of size(100) * 10000000 = 0 ms, sum = 910000000
Bitset64's set dense  array of size(100) * 100000000 = 266 ms, sum = 200000000
bitset  's set dense  array of size(100) * 100000000 = 48 ms, sum = 200000000
Bitset64's reset dense  array of size(100) * 100000000 = 288 ms, sum = 0
bitset  's reset dense  array of size(100) * 100000000 = 32 ms, sum = 0
Bitset64's flip dense  array of size(100) * 100000000 = 172 ms, sum = 100000000
bitset  's flip dense  array of size(100) * 100000000 = 79 ms, sum = 100000000
Bitset64's l_shift 1 dense  array of size(100) * 10000000 = 45 ms, sum = 5000001
bitset  's l_shift 1 dense  array of size(100) * 10000000 = 8 ms, sum = 5000001
Bitset64's l_shift 64 dense  array of size(100) * 10000000 = 45 ms, sum = 5000001
bitset  's l_shift 64 dense  array of size(100) * 10000000 = 38 ms, sum = 5000001
Bitset64's l_shift 99 dense  array of size(100) * 10000000 = 46 ms, sum = 5000001
bitset  's l_shift 99 dense  array of size(100) * 10000000 = 9 ms, sum = 5000001
Bitset64's r_shift 1 dense  array of size(100) * 10000000 = 52 ms, sum = 5000001
bitset  's r_shift 1 dense  array of size(100) * 10000000 = 11 ms, sum = 5000001
Bitset64's r_shift 64 dense  array of size(100) * 10000000 = 50 ms, sum = 5000001
bitset  's r_shift 64 dense  array of size(100) * 10000000 = 39 ms, sum = 5000001
Bitset64's r_shift 99 dense  array of size(100) * 10000000 = 48 ms, sum = 5000001
bitset  's r_shift 99 dense  array of size(100) * 10000000 = 10 ms, sum = 5000001
Bitset64's visit dense  array of size(100000) *100 = 28 ms, sum = 450217681100
Bitset64's visit dense  array of size(100000) *100 = 29 ms, sum = 450217681100
bitset  's visit dense  array of size(100000) *100 = 27 ms, sum = 450217681100
Bitset64's count dense  array of size(100000) * 10000 = 4 ms, sum = 900750000
bitset  's count dense  array of size(100000) * 10000 = 14 ms, sum = 900750000
Bitset64's set dense  array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set dense  array of size(100000) * 100000 = 10 ms, sum = 200000
Bitset64's reset dense  array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset dense  array of size(100000) * 100000 = 9 ms, sum = 0
Bitset64's flip dense  array of size(100000) * 100000 = 15 ms, sum = 100000
bitset  's flip dense  array of size(100000) * 100000 = 16 ms, sum = 100000
Bitset64's l_shift 1 dense  array of size(100000) * 100000 = 72 ms, sum = 50001
bitset  's l_shift 1 dense  array of size(100000) * 100000 = 99 ms, sum = 50001
Bitset64's l_shift 64 dense  array of size(100000) * 100000 = 49 ms, sum = 50001
bitset  's l_shift 64 dense  array of size(100000) * 100000 = 35 ms, sum = 50001
Bitset64's l_shift 100 dense  array of size(100000) * 100000 = 79 ms, sum = 50001
bitset  's l_shift 100 dense  array of size(100000) * 100000 = 55 ms, sum = 50001
Bitset64's l_shift 200 dense  array of size(100000) * 100000 = 78 ms, sum = 50001
bitset  's l_shift 200 dense  array of size(100000) * 100000 = 108 ms, sum = 50001
Bitset64's l_shift 500 dense  array of size(100000) * 100000 = 82 ms, sum = 50001
bitset  's l_shift 500 dense  array of size(100000) * 100000 = 58 ms, sum = 50001
Bitset64's l_shift 10000 dense  array of size(100000) * 100000 = 53 ms, sum = 50001
bitset  's l_shift 10000 dense  array of size(100000) * 100000 = 61 ms, sum = 50001
Bitset64's l_shift 90000 dense  array of size(100000) * 100000 = 22 ms, sum = 50001
bitset  's l_shift 90000 dense  array of size(100000) * 100000 = 23 ms, sum = 50001
Bitset64's r_shift 1 dense  array of size(100000) * 100000 = 39 ms, sum = 50001
bitset  's r_shift 1 dense  array of size(100000) * 100000 = 41 ms, sum = 50001
Bitset64's r_shift 64 dense  array of size(100000) * 100000 = 34 ms, sum = 50001
bitset  's r_shift 64 dense  array of size(100000) * 100000 = 35 ms, sum = 50001
Bitset64's r_shift 100 dense  array of size(100000) * 100000 = 37 ms, sum = 50001
bitset  's r_shift 100 dense  array of size(100000) * 100000 = 39 ms, sum = 50001
Bitset64's r_shift 200 dense  array of size(100000) * 100000 = 36 ms, sum = 50001
bitset  's r_shift 200 dense  array of size(100000) * 100000 = 38 ms, sum = 50001
Bitset64's r_shift 500 dense  array of size(100000) * 100000 = 36 ms, sum = 50001
bitset  's r_shift 500 dense  array of size(100000) * 100000 = 42 ms, sum = 50001
Bitset64's r_shift 10000 dense  array of size(100000) * 100000 = 35 ms, sum = 50001
bitset  's r_shift 10000 dense  array of size(100000) * 100000 = 36 ms, sum = 50001
Bitset64's r_shift 90000 dense  array of size(100000) * 100000 = 21 ms, sum = 50001
bitset  's r_shift 90000 dense  array of size(100000) * 100000 = 20 ms, sum = 50001

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
Bitset64's visit sparse array of size(100) *1000000 = 54 ms, sum = 564000000
Bitset64's visit sparse array of size(100) *1000000 = 51 ms, sum = 564000000
Bitset64's count sparse array of size(100) * 10000000 = 0 ms, sum = 120000000
bitset  's count sparse array of size(100) * 10000000 = 81 ms, sum = 120000000
Bitset64's set sparse array of size(100) * 100000000 = 269 ms, sum = 200000000
bitset  's set sparse array of size(100) * 100000000 = 97 ms, sum = 200000000
Bitset64's reset sparse array of size(100) * 100000000 = 271 ms, sum = 0
bitset  's reset sparse array of size(100) * 100000000 = 63 ms, sum = 0
Bitset64's flip sparse array of size(100) * 100000000 = 212 ms, sum = 100000000
bitset  's flip sparse array of size(100) * 100000000 = 486 ms, sum = 100000000
Bitset64's l_shift 1 sparse array of size(100) * 10000000 = 95 ms, sum = 5000001
bitset  's l_shift 1 sparse array of size(100) * 10000000 = 5 ms, sum = 5000001
Bitset64's l_shift 64 sparse array of size(100) * 10000000 = 132 ms, sum = 5000001
bitset  's l_shift 64 sparse array of size(100) * 10000000 = 4 ms, sum = 5000001
Bitset64's l_shift 99 sparse array of size(100) * 10000000 = 89 ms, sum = 5000001
bitset  's l_shift 99 sparse array of size(100) * 10000000 = 4 ms, sum = 5000001
Bitset64's r_shift 1 sparse array of size(100) * 10000000 = 89 ms, sum = 5000001
bitset  's r_shift 1 sparse array of size(100) * 10000000 = 51 ms, sum = 5000001
Bitset64's r_shift 64 sparse array of size(100) * 10000000 = 101 ms, sum = 5000001
bitset  's r_shift 64 sparse array of size(100) * 10000000 = 108 ms, sum = 5000001
Bitset64's r_shift 99 sparse array of size(100) * 10000000 = 99 ms, sum = 5000001
bitset  's r_shift 99 sparse array of size(100) * 10000000 = 118 ms, sum = 5000001
Bitset64's visit sparse array of size(100000) *1000 = 59 ms, sum = 501519936000
Bitset64's visit sparse array of size(100000) *1000 = 80 ms, sum = 501519936000
Bitset64's count sparse array of size(100000) * 10000 = 0 ms, sum = 100610000
bitset  's count sparse array of size(100000) * 10000 = 41 ms, sum = 100610000
Bitset64's set sparse array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set sparse array of size(100000) * 100000 = 9 ms, sum = 200000
Bitset64's reset sparse array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset sparse array of size(100000) * 100000 = 9 ms, sum = 0
Bitset64's flip sparse array of size(100000) * 100000 = 24 ms, sum = 100000
bitset  's flip sparse array of size(100000) * 100000 = 19 ms, sum = 100000
Bitset64's l_shift 1 sparse array of size(100000) * 100000 = 314 ms, sum = 50001
bitset  's l_shift 1 sparse array of size(100000) * 100000 = 167 ms, sum = 50001
Bitset64's l_shift 64 sparse array of size(100000) * 100000 = 95 ms, sum = 50001
bitset  's l_shift 64 sparse array of size(100000) * 100000 = 113 ms, sum = 50001
Bitset64's l_shift 100 sparse array of size(100000) * 100000 = 307 ms, sum = 50001
bitset  's l_shift 100 sparse array of size(100000) * 100000 = 230 ms, sum = 50001
Bitset64's l_shift 200 sparse array of size(100000) * 100000 = 305 ms, sum = 50001
bitset  's l_shift 200 sparse array of size(100000) * 100000 = 213 ms, sum = 50001
Bitset64's l_shift 500 sparse array of size(100000) * 100000 = 305 ms, sum = 50001
bitset  's l_shift 500 sparse array of size(100000) * 100000 = 230 ms, sum = 50001
Bitset64's l_shift 10000 sparse array of size(100000) * 100000 = 274 ms, sum = 50001
bitset  's l_shift 10000 sparse array of size(100000) * 100000 = 217 ms, sum = 50001
Bitset64's r_shift 1 sparse array of size(100000) * 100000 = 197 ms, sum = 50001
bitset  's r_shift 1 sparse array of size(100000) * 100000 = 72 ms, sum = 50001
Bitset64's r_shift 64 sparse array of size(100000) * 100000 = 85 ms, sum = 50001
bitset  's r_shift 64 sparse array of size(100000) * 100000 = 90 ms, sum = 50001
Bitset64's r_shift 100 sparse array of size(100000) * 100000 = 198 ms, sum = 50001
bitset  's r_shift 100 sparse array of size(100000) * 100000 = 131 ms, sum = 50001
Bitset64's r_shift 200 sparse array of size(100000) * 100000 = 200 ms, sum = 50001
bitset  's r_shift 200 sparse array of size(100000) * 100000 = 129 ms, sum = 50001
Bitset64's r_shift 500 sparse array of size(100000) * 100000 = 195 ms, sum = 50001
bitset  's r_shift 500 sparse array of size(100000) * 100000 = 130 ms, sum = 50001
Bitset64's r_shift 10000 sparse array of size(100000) * 100000 = 181 ms, sum = 50001
bitset  's r_shift 10000 sparse array of size(100000) * 100000 = 132 ms, sum = 50001
Bitset64's visit dense  array of size(100) *100000 = 47 ms, sum = 442900000
Bitset64's visit dense  array of size(100) *100000 = 43 ms, sum = 442900000
Bitset64's count dense  array of size(100) * 10000000 = 0 ms, sum = 910000000
bitset  's count dense  array of size(100) * 10000000 = 81 ms, sum = 910000000
Bitset64's set dense  array of size(100) * 100000000 = 285 ms, sum = 200000000
bitset  's set dense  array of size(100) * 100000000 = 94 ms, sum = 200000000
Bitset64's reset dense  array of size(100) * 100000000 = 262 ms, sum = 0
bitset  's reset dense  array of size(100) * 100000000 = 56 ms, sum = 0
Bitset64's flip dense  array of size(100) * 100000000 = 195 ms, sum = 100000000
bitset  's flip dense  array of size(100) * 100000000 = 499 ms, sum = 100000000
Bitset64's l_shift 1 dense  array of size(100) * 10000000 = 96 ms, sum = 5000001
bitset  's l_shift 1 dense  array of size(100) * 10000000 = 5 ms, sum = 5000001
Bitset64's l_shift 64 dense  array of size(100) * 10000000 = 81 ms, sum = 5000001
bitset  's l_shift 64 dense  array of size(100) * 10000000 = 3 ms, sum = 5000001
Bitset64's l_shift 99 dense  array of size(100) * 10000000 = 93 ms, sum = 5000001
bitset  's l_shift 99 dense  array of size(100) * 10000000 = 4 ms, sum = 5000001
Bitset64's r_shift 1 dense  array of size(100) * 10000000 = 90 ms, sum = 5000001
bitset  's r_shift 1 dense  array of size(100) * 10000000 = 50 ms, sum = 5000001
Bitset64's r_shift 64 dense  array of size(100) * 10000000 = 99 ms, sum = 5000001
bitset  's r_shift 64 dense  array of size(100) * 10000000 = 108 ms, sum = 5000001
Bitset64's r_shift 99 dense  array of size(100) * 10000000 = 99 ms, sum = 5000001
bitset  's r_shift 99 dense  array of size(100) * 10000000 = 126 ms, sum = 5000001
Bitset64's visit dense  array of size(100000) *100 = 47 ms, sum = 450217681100
Bitset64's visit dense  array of size(100000) *100 = 46 ms, sum = 450217681100
Bitset64's count dense  array of size(100000) * 10000 = 0 ms, sum = 900750000
bitset  's count dense  array of size(100000) * 10000 = 41 ms, sum = 900750000
Bitset64's set dense  array of size(100000) * 100000 = 10 ms, sum = 200000
bitset  's set dense  array of size(100000) * 100000 = 9 ms, sum = 200000
Bitset64's reset dense  array of size(100000) * 100000 = 10 ms, sum = 0
bitset  's reset dense  array of size(100000) * 100000 = 9 ms, sum = 0
Bitset64's flip dense  array of size(100000) * 100000 = 23 ms, sum = 100000
bitset  's flip dense  array of size(100000) * 100000 = 19 ms, sum = 100000
Bitset64's l_shift 1 dense  array of size(100000) * 100000 = 305 ms, sum = 50001
bitset  's l_shift 1 dense  array of size(100000) * 100000 = 162 ms, sum = 50001
Bitset64's l_shift 64 dense  array of size(100000) * 100000 = 97 ms, sum = 50001
bitset  's l_shift 64 dense  array of size(100000) * 100000 = 111 ms, sum = 50001
Bitset64's l_shift 100 dense  array of size(100000) * 100000 = 306 ms, sum = 50001
bitset  's l_shift 100 dense  array of size(100000) * 100000 = 228 ms, sum = 50001
Bitset64's l_shift 200 dense  array of size(100000) * 100000 = 303 ms, sum = 50001
bitset  's l_shift 200 dense  array of size(100000) * 100000 = 214 ms, sum = 50001
Bitset64's l_shift 500 dense  array of size(100000) * 100000 = 309 ms, sum = 50001
bitset  's l_shift 500 dense  array of size(100000) * 100000 = 229 ms, sum = 50001
Bitset64's l_shift 10000 dense  array of size(100000) * 100000 = 277 ms, sum = 50001
bitset  's l_shift 10000 dense  array of size(100000) * 100000 = 217 ms, sum = 50001
Bitset64's l_shift 90000 dense  array of size(100000) * 100000 = 48 ms, sum = 50001
bitset  's l_shift 90000 dense  array of size(100000) * 100000 = 217 ms, sum = 50001
Bitset64's r_shift 1 dense  array of size(100000) * 100000 = 199 ms, sum = 50001
bitset  's r_shift 1 dense  array of size(100000) * 100000 = 77 ms, sum = 50001
Bitset64's r_shift 64 dense  array of size(100000) * 100000 = 83 ms, sum = 50001
bitset  's r_shift 64 dense  array of size(100000) * 100000 = 95 ms, sum = 50001
Bitset64's r_shift 100 dense  array of size(100000) * 100000 = 199 ms, sum = 50001
bitset  's r_shift 100 dense  array of size(100000) * 100000 = 133 ms, sum = 50001
Bitset64's r_shift 200 dense  array of size(100000) * 100000 = 196 ms, sum = 50001
bitset  's r_shift 200 dense  array of size(100000) * 100000 = 132 ms, sum = 50001
Bitset64's r_shift 500 dense  array of size(100000) * 100000 = 198 ms, sum = 50001
bitset  's r_shift 500 dense  array of size(100000) * 100000 = 132 ms, sum = 50001
Bitset64's r_shift 10000 dense  array of size(100000) * 100000 = 182 ms, sum = 50001
bitset  's r_shift 10000 dense  array of size(100000) * 100000 = 134 ms, sum = 50001
Bitset64's r_shift 90000 dense  array of size(100000) * 100000 = 42 ms, sum = 50001
bitset  's r_shift 90000 dense  array of size(100000) * 100000 = 127 ms, sum = 50001

*/