/*
本文件在 C++20 标准下编译运行，比较 std::bitset 与模板库里的 Bitset 效率高低
*/
#include <bitset>
#include <chrono>
#include <random>

#include "DS/Bitset.h"
#include "IO/FastIO.h"

using std::bitset;
static constexpr size_t size1 = 32, size2 = 64, size3 = 100, size4 = 1000, size5 = 10000, size6 = 100000;
static constexpr char name[2][10] = {"sparse", "dense "};
bool arr1[2][size1], arr2[2][size2], arr3[2][size3], arr4[2][size4], arr5[2][size5], arr6[2][size6];
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
#define test_bitset_visit(Set, First, Next, End, Size, i, M)                                                                                                      \
    {                                                                                                                                                             \
        Set<size##Size> B;                                                                                                                                        \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                  \
            if (arr##Size[i][j]) B.set(j);                                                                                                                        \
        sum = 0;                                                                                                                                                  \
        timer_start;                                                                                                                                              \
        for (size_t k = 0; k != M; k++)                                                                                                                           \
            for (auto j = B.First(); End; j = B.Next(j)) sum += j;                                                                                                \
        timer_end;                                                                                                                                                \
        cout << to_string_of_len(#Set, 8) << "'s visit " << name[i] << " array of size(" << size##Size << ") = " << duration_get << " ms, sum = " << sum << endl; \
    }
#ifndef _MSC_VER
#define test_all_visit(Size, M)                                                    \
    test_bitset_visit(Bitset64, first_one, next_one, ~j, Size, 0, M);              \
    test_bitset_visit(Bitset64, first_one, next_one, ~j, Size, 1, M);              \
    test_bitset_visit(Bitset32, first_one, next_one, ~j, Size, 0, M);              \
    test_bitset_visit(Bitset32, first_one, next_one, ~j, Size, 1, M);              \
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), Size, 0, M); \
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), Size, 1, M);
#else
#define test_all_visit(Size, M)                                       \
    test_bitset_visit(Bitset64, first_one, next_one, ~j, Size, 0, M); \
    test_bitset_visit(Bitset64, first_one, next_one, ~j, Size, 1, M); \
    test_bitset_visit(Bitset32, first_one, next_one, ~j, Size, 0, M); \
    test_bitset_visit(Bitset32, first_one, next_one, ~j, Size, 1, M);
#endif
#ifndef _MSC_VER
#define test_all_visit2(Size, M)                                                   \
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, Size, 0, M);               \
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, Size, 1, M);               \
    test_bitset_visit(Bitset32, last_one, prev_one, ~j, Size, 0, M);               \
    test_bitset_visit(Bitset32, last_one, prev_one, ~j, Size, 1, M);               \
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), Size, 0, M); \
    test_bitset_visit(bitset, _Find_first, _Find_next, j != B.size(), Size, 1, M);
#else
#define test_all_visit2(Size, M)                                     \
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, Size, 0, M); \
    test_bitset_visit(Bitset64, last_one, prev_one, ~j, Size, 1, M); \
    test_bitset_visit(Bitset32, last_one, prev_one, ~j, Size, 0, M); \
    test_bitset_visit(Bitset32, last_one, prev_one, ~j, Size, 1, M);
#endif
#define test_bitset_op(Set, op, Size, i, M)                                                                                                                                     \
    {                                                                                                                                                                           \
        Set<size##Size> B;                                                                                                                                                      \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                                \
            if (arr##Size[i][j]) B.set(j);                                                                                                                                      \
        timer_start;                                                                                                                                                            \
        if (Size <= 2)                                                                                                                                                          \
            for (size_t j = 0; j != M; j++) {                                                                                                                                   \
                B.op();                                                                                                                                                         \
            }                                                                                                                                                                   \
        else                                                                                                                                                                    \
            for (size_t j = 0; j != M; j++)                                                                                                                                     \
                if (rr() != 123456789) B.op();                                                                                                                                  \
        timer_end;                                                                                                                                                              \
        cout << to_string_of_len(#Set, 8) << "'s " << #op << " " << name[i] << " array of size(" << size##Size << ") = " << duration_get << " ms, sum = " << B.count() << endl; \
    }
#define test_all_op(Size, op, M)              \
    test_bitset_op(Bitset64, op, Size, 0, M); \
    test_bitset_op(Bitset64, op, Size, 1, M); \
    test_bitset_op(Bitset32, op, Size, 0, M); \
    test_bitset_op(Bitset32, op, Size, 1, M); \
    test_bitset_op(bitset, op, Size, 0, M);   \
    test_bitset_op(bitset, op, Size, 1, M);
#define test_bitset_count(Set, Size, i, M)                                                                                                                        \
    {                                                                                                                                                             \
        Set<size##Size> B;                                                                                                                                        \
        for (size_t j = 0; j != size##Size; j++)                                                                                                                  \
            if (arr##Size[i][j]) B.set(j);                                                                                                                        \
        timer_start;                                                                                                                                              \
        sum = 0;                                                                                                                                                  \
        if (Size <= 2)                                                                                                                                            \
            for (size_t j = 0; j != M; j++) sum += B.count();                                                                                                     \
        else                                                                                                                                                      \
            for (size_t j = 0; j != M; j++)                                                                                                                       \
                if (rr() != 123456789) sum += B.count();                                                                                                          \
        timer_end;                                                                                                                                                \
        cout << to_string_of_len(#Set, 8) << "'s count " << name[i] << " array of size(" << size##Size << ") = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_all_count(Size, M)              \
    test_bitset_count(Bitset64, Size, 0, M); \
    test_bitset_count(Bitset64, Size, 1, M); \
    test_bitset_count(Bitset32, Size, 0, M); \
    test_bitset_count(Bitset32, Size, 1, M); \
    test_bitset_count(bitset, Size, 0, M);   \
    test_bitset_count(bitset, Size, 1, M);
template <uint32_t N>
using Bitset64 = OY::Bitset::Table<uint64_t, N>;
template <uint32_t N>
using Bitset32 = OY::Bitset::Table<uint32_t, N>;
int main() {
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < size1; j++) arr1[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size2; j++) arr2[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size3; j++) arr3[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size4; j++) arr4[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size5; j++) arr5[i][j] = i ? (rr() % 10) : !(rr() % 10);
        for (size_t j = 0; j < size6; j++) arr6[i][j] = i ? (rr() % 10) : !(rr() % 10);
    }

    test_all_visit(1, 2000000);
    test_all_visit(2, 1000000);
    test_all_visit(3, 1000000);
    test_all_visit(4, 100000);
    test_all_visit(5, 10000);
    test_all_visit(6, 1000);

    test_all_visit2(1, 2000000);
    test_all_visit2(2, 1000000);
    test_all_visit2(3, 1000000);
    test_all_visit2(4, 100000);
    test_all_visit2(5, 10000);
    test_all_visit2(6, 1000);

    test_all_count(1, 100000000);
    test_all_count(2, 50000000);
    test_all_count(3, 50000000);
    test_all_count(4, 5000000);
    test_all_count(5, 500000);
    test_all_count(6, 50000);

    test_all_op(1, set, 100000000);
    test_all_op(2, set, 50000000);
    test_all_op(3, set, 50000000);
    test_all_op(4, set, 5000000);
    test_all_op(5, set, 500000);
    test_all_op(6, set, 50000);

    test_all_op(1, reset, 100000000);
    test_all_op(2, reset, 50000000);
    test_all_op(3, reset, 50000000);
    test_all_op(4, reset, 5000000);
    test_all_op(5, reset, 500000);
    test_all_op(6, reset, 50000);

    test_all_op(1, flip, 100000000);
    test_all_op(2, flip, 50000000);
    test_all_op(3, flip, 50000000);
    test_all_op(4, flip, 5000000);
    test_all_op(5, flip, 500000);
    test_all_op(6, flip, 50000);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
Bitset64's visit sparse array of size(32) = 14 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 155 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 14 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 153 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 6 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 98 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 8 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 151 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 8 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 161 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 10 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 176 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 17 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 244 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 18 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 258 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 17 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 269 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 32 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 278 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 32 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 275 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 36 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 280 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 30 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 277 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 31 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 285 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 48 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 280 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 36 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 272 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 44 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 276 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 45 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 276 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 9 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 151 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 9 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 147 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 5 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 92 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 6 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 137 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 9 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 175 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 10 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 174 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 12 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 253 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 13 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 255 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 16 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 267 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 31 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 264 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 33 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 276 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 35 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 267 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 31 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 264 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 29 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 264 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 44 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 272 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 32 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 268 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 38 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 268 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 42 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 262 ms, sum = 4495004570000
Bitset64's count sparse array of size(32) = 0 ms, sum = 500000000
Bitset64's count dense  array of size(32) = 0 ms, sum = 3100000000
Bitset32's count sparse array of size(32) = 0 ms, sum = 500000000
Bitset32's count dense  array of size(32) = 0 ms, sum = 3100000000
bitset  's count sparse array of size(32) = 0 ms, sum = 500000000
bitset  's count dense  array of size(32) = 0 ms, sum = 3100000000
Bitset64's count sparse array of size(64) = 0 ms, sum = 300000000
Bitset64's count dense  array of size(64) = 0 ms, sum = 3000000000
Bitset32's count sparse array of size(64) = 0 ms, sum = 300000000
Bitset32's count dense  array of size(64) = 0 ms, sum = 3000000000
bitset  's count sparse array of size(64) = 0 ms, sum = 300000000
bitset  's count dense  array of size(64) = 0 ms, sum = 3000000000
Bitset64's count sparse array of size(100) = 296 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 299 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 300 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 298 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 368 ms, sum = 400000000
bitset  's count dense  array of size(100) = 358 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 30 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 29 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 29 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 29 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 118 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 105 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 2 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 2 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 58 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 60 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 55 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 54 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 47 ms, sum = 32
Bitset64's set dense  array of size(32) = 48 ms, sum = 32
Bitset32's set sparse array of size(32) = 68 ms, sum = 32
Bitset32's set dense  array of size(32) = 68 ms, sum = 32
bitset  's set sparse array of size(32) = 0 ms, sum = 32
bitset  's set dense  array of size(32) = 0 ms, sum = 32
Bitset64's set sparse array of size(64) = 22 ms, sum = 64
Bitset64's set dense  array of size(64) = 23 ms, sum = 64
Bitset32's set sparse array of size(64) = 128 ms, sum = 64
Bitset32's set dense  array of size(64) = 118 ms, sum = 64
bitset  's set sparse array of size(64) = 0 ms, sum = 64
bitset  's set dense  array of size(64) = 0 ms, sum = 64
Bitset64's set sparse array of size(100) = 392 ms, sum = 100
Bitset64's set dense  array of size(100) = 394 ms, sum = 100
Bitset32's set sparse array of size(100) = 403 ms, sum = 100
Bitset32's set dense  array of size(100) = 404 ms, sum = 100
bitset  's set sparse array of size(100) = 295 ms, sum = 100
bitset  's set dense  array of size(100) = 301 ms, sum = 100
Bitset64's set sparse array of size(1000) = 41 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 42 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 44 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 43 ms, sum = 1000
bitset  's set sparse array of size(1000) = 96 ms, sum = 1000
bitset  's set dense  array of size(1000) = 95 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 7 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 7 ms, sum = 10000
bitset  's set sparse array of size(10000) = 43 ms, sum = 10000
bitset  's set dense  array of size(10000) = 44 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 4 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 5 ms, sum = 100000
bitset  's set sparse array of size(100000) = 36 ms, sum = 100000
bitset  's set dense  array of size(100000) = 36 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 232 ms, sum = 0
Bitset64's reset dense  array of size(32) = 235 ms, sum = 0
Bitset32's reset sparse array of size(32) = 237 ms, sum = 0
Bitset32's reset dense  array of size(32) = 256 ms, sum = 0
bitset  's reset sparse array of size(32) = 0 ms, sum = 0
bitset  's reset dense  array of size(32) = 0 ms, sum = 0
Bitset64's reset sparse array of size(64) = 116 ms, sum = 0
Bitset64's reset dense  array of size(64) = 119 ms, sum = 0
Bitset32's reset sparse array of size(64) = 103 ms, sum = 0
Bitset32's reset dense  array of size(64) = 105 ms, sum = 0
bitset  's reset sparse array of size(64) = 12 ms, sum = 0
bitset  's reset dense  array of size(64) = 12 ms, sum = 0
Bitset64's reset sparse array of size(100) = 392 ms, sum = 0
Bitset64's reset dense  array of size(100) = 393 ms, sum = 0
Bitset32's reset sparse array of size(100) = 392 ms, sum = 0
Bitset32's reset dense  array of size(100) = 391 ms, sum = 0
bitset  's reset sparse array of size(100) = 299 ms, sum = 0
bitset  's reset dense  array of size(100) = 301 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 42 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 43 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 43 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 43 ms, sum = 0
bitset  's reset sparse array of size(1000) = 39 ms, sum = 0
bitset  's reset dense  array of size(1000) = 40 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 7 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 7 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 7 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 7 ms, sum = 0
bitset  's reset sparse array of size(10000) = 7 ms, sum = 0
bitset  's reset dense  array of size(10000) = 7 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 6 ms, sum = 0
bitset  's reset sparse array of size(100000) = 5 ms, sum = 0
bitset  's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset64's flip sparse array of size(32) = 122 ms, sum = 5
Bitset64's flip dense  array of size(32) = 122 ms, sum = 31
Bitset32's flip sparse array of size(32) = 124 ms, sum = 5
Bitset32's flip dense  array of size(32) = 125 ms, sum = 31
bitset  's flip sparse array of size(32) = 23 ms, sum = 5
bitset  's flip dense  array of size(32) = 23 ms, sum = 31
Bitset64's flip sparse array of size(64) = 62 ms, sum = 6
Bitset64's flip dense  array of size(64) = 60 ms, sum = 60
Bitset32's flip sparse array of size(64) = 63 ms, sum = 6
Bitset32's flip dense  array of size(64) = 64 ms, sum = 60
bitset  's flip sparse array of size(64) = 62 ms, sum = 6
bitset  's flip dense  array of size(64) = 62 ms, sum = 60
Bitset64's flip sparse array of size(100) = 342 ms, sum = 8
Bitset64's flip dense  array of size(100) = 344 ms, sum = 87
Bitset32's flip sparse array of size(100) = 391 ms, sum = 8
Bitset32's flip dense  array of size(100) = 392 ms, sum = 87
bitset  's flip sparse array of size(100) = 361 ms, sum = 8
bitset  's flip dense  array of size(100) = 356 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 57 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 56 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 110 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 93 ms, sum = 918
bitset  's flip sparse array of size(1000) = 85 ms, sum = 113
bitset  's flip dense  array of size(1000) = 104 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 31 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 31 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 57 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 56 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 55 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 55 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 26 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 25 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 52 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 51 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 51 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 49 ms, sum = 89988

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
Bitset64's visit sparse array of size(32) = 15 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 151 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 15 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 145 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 7 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 95 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 8 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 137 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 10 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 175 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 7 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 186 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 17 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 249 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 23 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 286 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 14 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 272 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 33 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 271 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 35 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 299 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 32 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 278 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 35 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 267 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 36 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 294 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 36 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 291 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 38 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 277 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 47 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 305 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 42 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 278 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 16 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 201 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 16 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 211 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 7 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 86 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 13 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 203 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 207 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 8 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 194 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 19 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 277 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 17 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 296 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 15 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 286 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 36 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 302 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 35 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 305 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 32 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 282 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 38 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 284 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 43 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 297 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 36 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 286 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 39 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 292 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 44 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 286 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 43 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 273 ms, sum = 4495004570000
Bitset64's count sparse array of size(32) = 0 ms, sum = 500000000
Bitset64's count dense  array of size(32) = 0 ms, sum = 3100000000
Bitset32's count sparse array of size(32) = 0 ms, sum = 500000000
Bitset32's count dense  array of size(32) = 0 ms, sum = 3100000000
bitset  's count sparse array of size(32) = 0 ms, sum = 500000000
bitset  's count dense  array of size(32) = 0 ms, sum = 3100000000
Bitset64's count sparse array of size(64) = 0 ms, sum = 300000000
Bitset64's count dense  array of size(64) = 0 ms, sum = 3000000000
Bitset32's count sparse array of size(64) = 0 ms, sum = 300000000
Bitset32's count dense  array of size(64) = 0 ms, sum = 3000000000
bitset  's count sparse array of size(64) = 0 ms, sum = 300000000
bitset  's count dense  array of size(64) = 0 ms, sum = 3000000000
Bitset64's count sparse array of size(100) = 175 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 176 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 172 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 173 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 175 ms, sum = 400000000
bitset  's count dense  array of size(100) = 172 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 17 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 16 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 17 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 17 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 55 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 55 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 1 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 1 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 1 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 71 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 72 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 70 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 70 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 107 ms, sum = 32
Bitset64's set dense  array of size(32) = 104 ms, sum = 32
Bitset32's set sparse array of size(32) = 90 ms, sum = 32
Bitset32's set dense  array of size(32) = 89 ms, sum = 32
bitset  's set sparse array of size(32) = 0 ms, sum = 32
bitset  's set dense  array of size(32) = 0 ms, sum = 32
Bitset64's set sparse array of size(64) = 50 ms, sum = 64
Bitset64's set dense  array of size(64) = 51 ms, sum = 64
Bitset32's set sparse array of size(64) = 138 ms, sum = 64
Bitset32's set dense  array of size(64) = 133 ms, sum = 64
bitset  's set sparse array of size(64) = 0 ms, sum = 64
bitset  's set dense  array of size(64) = 0 ms, sum = 64
Bitset64's set sparse array of size(100) = 292 ms, sum = 100
Bitset64's set dense  array of size(100) = 297 ms, sum = 100
Bitset32's set sparse array of size(100) = 306 ms, sum = 100
Bitset32's set dense  array of size(100) = 304 ms, sum = 100
bitset  's set sparse array of size(100) = 178 ms, sum = 100
bitset  's set dense  array of size(100) = 179 ms, sum = 100
Bitset64's set sparse array of size(1000) = 32 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 32 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 32 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 32 ms, sum = 1000
bitset  's set sparse array of size(1000) = 20 ms, sum = 1000
bitset  's set dense  array of size(1000) = 20 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 6 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 6 ms, sum = 10000
bitset  's set sparse array of size(10000) = 6 ms, sum = 10000
bitset  's set dense  array of size(10000) = 6 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 5 ms, sum = 100000
bitset  's set sparse array of size(100000) = 4 ms, sum = 100000
bitset  's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 227 ms, sum = 0
Bitset64's reset dense  array of size(32) = 227 ms, sum = 0
Bitset32's reset sparse array of size(32) = 252 ms, sum = 0
Bitset32's reset dense  array of size(32) = 248 ms, sum = 0
bitset  's reset sparse array of size(32) = 0 ms, sum = 0
bitset  's reset dense  array of size(32) = 0 ms, sum = 0
Bitset64's reset sparse array of size(64) = 118 ms, sum = 0
Bitset64's reset dense  array of size(64) = 116 ms, sum = 0
Bitset32's reset sparse array of size(64) = 113 ms, sum = 0
Bitset32's reset dense  array of size(64) = 109 ms, sum = 0
bitset  's reset sparse array of size(64) = 0 ms, sum = 0
bitset  's reset dense  array of size(64) = 0 ms, sum = 0
Bitset64's reset sparse array of size(100) = 267 ms, sum = 0
Bitset64's reset dense  array of size(100) = 271 ms, sum = 0
Bitset32's reset sparse array of size(100) = 271 ms, sum = 0
Bitset32's reset dense  array of size(100) = 269 ms, sum = 0
bitset  's reset sparse array of size(100) = 172 ms, sum = 0
bitset  's reset dense  array of size(100) = 172 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 30 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 30 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 30 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 30 ms, sum = 0
bitset  's reset sparse array of size(1000) = 20 ms, sum = 0
bitset  's reset dense  array of size(1000) = 20 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 6 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 6 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 6 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 6 ms, sum = 0
bitset  's reset sparse array of size(10000) = 6 ms, sum = 0
bitset  's reset dense  array of size(10000) = 6 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 5 ms, sum = 0
bitset  's reset sparse array of size(100000) = 5 ms, sum = 0
bitset  's reset dense  array of size(100000) = 4 ms, sum = 0
Bitset64's flip sparse array of size(32) = 1 ms, sum = 5
Bitset64's flip dense  array of size(32) = 1 ms, sum = 31
Bitset32's flip sparse array of size(32) = 129 ms, sum = 5
Bitset32's flip dense  array of size(32) = 127 ms, sum = 31
bitset  's flip sparse array of size(32) = 0 ms, sum = 5
bitset  's flip dense  array of size(32) = 0 ms, sum = 31
Bitset64's flip sparse array of size(64) = 0 ms, sum = 6
Bitset64's flip dense  array of size(64) = 0 ms, sum = 60
Bitset32's flip sparse array of size(64) = 94 ms, sum = 6
Bitset32's flip dense  array of size(64) = 94 ms, sum = 60
bitset  's flip sparse array of size(64) = 0 ms, sum = 6
bitset  's flip dense  array of size(64) = 0 ms, sum = 60
Bitset64's flip sparse array of size(100) = 237 ms, sum = 8
Bitset64's flip dense  array of size(100) = 235 ms, sum = 87
Bitset32's flip sparse array of size(100) = 303 ms, sum = 8
Bitset32's flip dense  array of size(100) = 305 ms, sum = 87
bitset  's flip sparse array of size(100) = 187 ms, sum = 8
bitset  's flip dense  array of size(100) = 188 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 44 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 44 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 102 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 104 ms, sum = 918
bitset  's flip sparse array of size(1000) = 30 ms, sum = 113
bitset  's flip dense  array of size(1000) = 31 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 7 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 7 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 10 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 9 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 8 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 8 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 5 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 4 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 5 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 5 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 5 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 4 ms, sum = 89988

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
Bitset64's visit sparse array of size(32) = 44 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 232 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 24 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 210 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 203 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 13 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 214 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 24 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 312 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 303 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 35 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 302 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 37 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 286 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 41 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 287 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 38 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 288 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 41 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 297 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 45 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 283 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 23 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 225 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 24 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 224 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 16 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 212 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 18 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 222 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 304 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 24 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 310 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 38 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 316 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 39 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 318 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 40 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 318 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 42 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 325 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 41 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 304 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 52 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 317 ms, sum = 4495004570000
Bitset64's count sparse array of size(32) = 117 ms, sum = 500000000
Bitset64's count dense  array of size(32) = 116 ms, sum = 3100000000
Bitset32's count sparse array of size(32) = 117 ms, sum = 500000000
Bitset32's count dense  array of size(32) = 121 ms, sum = 3100000000
bitset  's count sparse array of size(32) = 353 ms, sum = 500000000
bitset  's count dense  array of size(32) = 326 ms, sum = 3100000000
Bitset64's count sparse array of size(64) = 49 ms, sum = 300000000
Bitset64's count dense  array of size(64) = 49 ms, sum = 3000000000
Bitset32's count sparse array of size(64) = 50 ms, sum = 300000000
Bitset32's count dense  array of size(64) = 48 ms, sum = 3000000000
bitset  's count sparse array of size(64) = 252 ms, sum = 300000000
bitset  's count dense  array of size(64) = 252 ms, sum = 3000000000
Bitset64's count sparse array of size(100) = 200 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 202 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 197 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 197 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 427 ms, sum = 400000000
bitset  's count dense  array of size(100) = 419 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 21 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 20 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 20 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 19 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 259 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 256 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 1 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 1 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 1 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 2 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 203 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 210 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 199 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 195 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 188 ms, sum = 32
Bitset64's set dense  array of size(32) = 191 ms, sum = 32
Bitset32's set sparse array of size(32) = 188 ms, sum = 32
Bitset32's set dense  array of size(32) = 191 ms, sum = 32
bitset  's set sparse array of size(32) = 93 ms, sum = 32
bitset  's set dense  array of size(32) = 96 ms, sum = 32
Bitset64's set sparse array of size(64) = 93 ms, sum = 64
Bitset64's set dense  array of size(64) = 97 ms, sum = 64
Bitset32's set sparse array of size(64) = 189 ms, sum = 64
Bitset32's set dense  array of size(64) = 176 ms, sum = 64
bitset  's set sparse array of size(64) = 48 ms, sum = 64
bitset  's set dense  array of size(64) = 58 ms, sum = 64
Bitset64's set sparse array of size(100) = 314 ms, sum = 100
Bitset64's set dense  array of size(100) = 310 ms, sum = 100
Bitset32's set sparse array of size(100) = 333 ms, sum = 100
Bitset32's set dense  array of size(100) = 336 ms, sum = 100
bitset  's set sparse array of size(100) = 193 ms, sum = 100
bitset  's set dense  array of size(100) = 192 ms, sum = 100
Bitset64's set sparse array of size(1000) = 39 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 39 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 39 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 40 ms, sum = 1000
bitset  's set sparse array of size(1000) = 21 ms, sum = 1000
bitset  's set dense  array of size(1000) = 21 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 13 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 11 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 15 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 10 ms, sum = 10000
bitset  's set sparse array of size(10000) = 15 ms, sum = 10000
bitset  's set dense  array of size(10000) = 12 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 8 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 7 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 8 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 8 ms, sum = 100000
bitset  's set sparse array of size(100000) = 8 ms, sum = 100000
bitset  's set dense  array of size(100000) = 8 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 362 ms, sum = 0
Bitset64's reset dense  array of size(32) = 332 ms, sum = 0
Bitset32's reset sparse array of size(32) = 331 ms, sum = 0
Bitset32's reset dense  array of size(32) = 358 ms, sum = 0
bitset  's reset sparse array of size(32) = 95 ms, sum = 0
bitset  's reset dense  array of size(32) = 117 ms, sum = 0
Bitset64's reset sparse array of size(64) = 165 ms, sum = 0
Bitset64's reset dense  array of size(64) = 179 ms, sum = 0
Bitset32's reset sparse array of size(64) = 167 ms, sum = 0
Bitset32's reset dense  array of size(64) = 175 ms, sum = 0
bitset  's reset sparse array of size(64) = 58 ms, sum = 0
bitset  's reset dense  array of size(64) = 49 ms, sum = 0
Bitset64's reset sparse array of size(100) = 334 ms, sum = 0
Bitset64's reset dense  array of size(100) = 336 ms, sum = 0
Bitset32's reset sparse array of size(100) = 345 ms, sum = 0
Bitset32's reset dense  array of size(100) = 322 ms, sum = 0
bitset  's reset sparse array of size(100) = 191 ms, sum = 0
bitset  's reset dense  array of size(100) = 215 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 42 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 44 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 45 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 43 ms, sum = 0
bitset  's reset sparse array of size(1000) = 21 ms, sum = 0
bitset  's reset dense  array of size(1000) = 20 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 13 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 10 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 13 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 11 ms, sum = 0
bitset  's reset sparse array of size(10000) = 18 ms, sum = 0
bitset  's reset dense  array of size(10000) = 10 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 8 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 8 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 8 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 8 ms, sum = 0
bitset  's reset sparse array of size(100000) = 7 ms, sum = 0
bitset  's reset dense  array of size(100000) = 7 ms, sum = 0
Bitset64's flip sparse array of size(32) = 165 ms, sum = 5
Bitset64's flip dense  array of size(32) = 166 ms, sum = 31
Bitset32's flip sparse array of size(32) = 163 ms, sum = 5
Bitset32's flip dense  array of size(32) = 190 ms, sum = 31
bitset  's flip sparse array of size(32) = 96 ms, sum = 5
bitset  's flip dense  array of size(32) = 95 ms, sum = 31
Bitset64's flip sparse array of size(64) = 96 ms, sum = 6
Bitset64's flip dense  array of size(64) = 106 ms, sum = 60
Bitset32's flip sparse array of size(64) = 94 ms, sum = 6
Bitset32's flip dense  array of size(64) = 93 ms, sum = 60
bitset  's flip sparse array of size(64) = 46 ms, sum = 6
bitset  's flip dense  array of size(64) = 60 ms, sum = 60
Bitset64's flip sparse array of size(100) = 251 ms, sum = 8
Bitset64's flip dense  array of size(100) = 250 ms, sum = 87
Bitset32's flip sparse array of size(100) = 293 ms, sum = 8
Bitset32's flip dense  array of size(100) = 295 ms, sum = 87
bitset  's flip sparse array of size(100) = 316 ms, sum = 8
bitset  's flip dense  array of size(100) = 317 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 43 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 43 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 58 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 61 ms, sum = 918
bitset  's flip sparse array of size(1000) = 32 ms, sum = 113
bitset  's flip dense  array of size(1000) = 32 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 15 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 16 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 15 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 14 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 12 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 10 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 11 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 10 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 11 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 11 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 9 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 9 ms, sum = 89988

*/
