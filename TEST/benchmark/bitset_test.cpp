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
g++ -std=c++20 -DOY_LOCAL -O2
Bitset64's visit sparse array of size(32) = 15 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 154 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 15 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 143 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 8 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 114 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 8 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 141 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 10 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 170 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 11 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 208 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 17 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 258 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 23 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 275 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 21 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 308 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 34 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 282 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 34 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 290 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 40 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 316 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 33 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 285 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 32 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 280 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 50 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 311 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 38 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 283 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 44 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 291 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 50 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 314 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 22 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 157 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 22 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 156 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 8 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 113 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 150 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 18 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 210 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 11 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 208 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 24 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 305 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 23 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 298 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 21 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 305 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 37 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 320 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 37 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 312 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 40 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 314 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 34 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 315 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 35 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 310 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 50 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 310 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 38 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 312 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 43 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 310 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 50 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 311 ms, sum = 4495004570000
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
Bitset64's count sparse array of size(100) = 306 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 305 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 308 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 308 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 788 ms, sum = 400000000
bitset  's count dense  array of size(100) = 733 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 30 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 31 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 30 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 31 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 408 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 399 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 3 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 2 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 347 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 356 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 348 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 330 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 49 ms, sum = 32
Bitset64's set dense  array of size(32) = 50 ms, sum = 32
Bitset32's set sparse array of size(32) = 66 ms, sum = 32
Bitset32's set dense  array of size(32) = 68 ms, sum = 32
bitset  's set sparse array of size(32) = 0 ms, sum = 32
bitset  's set dense  array of size(32) = 0 ms, sum = 32
Bitset64's set sparse array of size(64) = 25 ms, sum = 64
Bitset64's set dense  array of size(64) = 24 ms, sum = 64
Bitset32's set sparse array of size(64) = 149 ms, sum = 64
Bitset32's set dense  array of size(64) = 150 ms, sum = 64
bitset  's set sparse array of size(64) = 0 ms, sum = 64
bitset  's set dense  array of size(64) = 0 ms, sum = 64
Bitset64's set sparse array of size(100) = 422 ms, sum = 100
Bitset64's set dense  array of size(100) = 416 ms, sum = 100
Bitset32's set sparse array of size(100) = 430 ms, sum = 100
Bitset32's set dense  array of size(100) = 428 ms, sum = 100
bitset  's set sparse array of size(100) = 300 ms, sum = 100
bitset  's set dense  array of size(100) = 300 ms, sum = 100
Bitset64's set sparse array of size(1000) = 45 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 45 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 45 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 47 ms, sum = 1000
bitset  's set sparse array of size(1000) = 102 ms, sum = 1000
bitset  's set dense  array of size(1000) = 114 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 7 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 7 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 7 ms, sum = 10000
bitset  's set sparse array of size(10000) = 44 ms, sum = 10000
bitset  's set dense  array of size(10000) = 44 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 5 ms, sum = 100000
bitset  's set sparse array of size(100000) = 38 ms, sum = 100000
bitset  's set dense  array of size(100000) = 37 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 242 ms, sum = 0
Bitset64's reset dense  array of size(32) = 241 ms, sum = 0
Bitset32's reset sparse array of size(32) = 263 ms, sum = 0
Bitset32's reset dense  array of size(32) = 239 ms, sum = 0
bitset  's reset sparse array of size(32) = 0 ms, sum = 0
bitset  's reset dense  array of size(32) = 0 ms, sum = 0
Bitset64's reset sparse array of size(64) = 121 ms, sum = 0
Bitset64's reset dense  array of size(64) = 109 ms, sum = 0
Bitset32's reset sparse array of size(64) = 107 ms, sum = 0
Bitset32's reset dense  array of size(64) = 119 ms, sum = 0
bitset  's reset sparse array of size(64) = 11 ms, sum = 0
bitset  's reset dense  array of size(64) = 14 ms, sum = 0
Bitset64's reset sparse array of size(100) = 392 ms, sum = 0
Bitset64's reset dense  array of size(100) = 389 ms, sum = 0
Bitset32's reset sparse array of size(100) = 388 ms, sum = 0
Bitset32's reset dense  array of size(100) = 393 ms, sum = 0
bitset  's reset sparse array of size(100) = 306 ms, sum = 0
bitset  's reset dense  array of size(100) = 306 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 44 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 44 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 44 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 47 ms, sum = 0
bitset  's reset sparse array of size(1000) = 66 ms, sum = 0
bitset  's reset dense  array of size(1000) = 68 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 8 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 7 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 7 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 7 ms, sum = 0
bitset  's reset sparse array of size(10000) = 12 ms, sum = 0
bitset  's reset dense  array of size(10000) = 12 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 5 ms, sum = 0
bitset  's reset sparse array of size(100000) = 5 ms, sum = 0
bitset  's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset64's flip sparse array of size(32) = 130 ms, sum = 5
Bitset64's flip dense  array of size(32) = 128 ms, sum = 31
Bitset32's flip sparse array of size(32) = 128 ms, sum = 5
Bitset32's flip dense  array of size(32) = 128 ms, sum = 31
bitset  's flip sparse array of size(32) = 23 ms, sum = 5
bitset  's flip dense  array of size(32) = 23 ms, sum = 31
Bitset64's flip sparse array of size(64) = 64 ms, sum = 6
Bitset64's flip dense  array of size(64) = 66 ms, sum = 60
Bitset32's flip sparse array of size(64) = 67 ms, sum = 6
Bitset32's flip dense  array of size(64) = 66 ms, sum = 60
bitset  's flip sparse array of size(64) = 12 ms, sum = 6
bitset  's flip dense  array of size(64) = 11 ms, sum = 60
Bitset64's flip sparse array of size(100) = 366 ms, sum = 8
Bitset64's flip dense  array of size(100) = 369 ms, sum = 87
Bitset32's flip sparse array of size(100) = 415 ms, sum = 8
Bitset32's flip dense  array of size(100) = 421 ms, sum = 87
bitset  's flip sparse array of size(100) = 372 ms, sum = 8
bitset  's flip dense  array of size(100) = 374 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 58 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 59 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 84 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 121 ms, sum = 918
bitset  's flip sparse array of size(1000) = 80 ms, sum = 113
bitset  's flip dense  array of size(1000) = 83 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 31 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 30 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 55 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 57 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 56 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 58 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 26 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 25 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 50 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 50 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 51 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 52 ms, sum = 89988

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
Bitset64's visit sparse array of size(32) = 17 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 171 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 22 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 152 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 9 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 105 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 10 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 147 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 176 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 10 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 195 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 271 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 26 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 285 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 19 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 294 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 40 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 315 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 37 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 314 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 38 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 314 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 42 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 302 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 41 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 313 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 43 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 320 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 45 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 317 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 54 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 305 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 51 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 296 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 19 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 208 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 19 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 209 ms, sum = 990000000
bitset  's visit sparse array of size(32) = 8 ms, sum = 138000000
bitset  's visit dense  array of size(32) = 102 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 15 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 200 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 15 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 202 ms, sum = 1896000000
bitset  's visit sparse array of size(64) = 11 ms, sum = 207000000
bitset  's visit dense  array of size(64) = 196 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 19 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 304 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 19 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 295 ms, sum = 4381000000
bitset  's visit sparse array of size(100) = 19 ms, sum = 385000000
bitset  's visit dense  array of size(100) = 287 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 36 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 312 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 35 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 301 ms, sum = 45812100000
bitset  's visit sparse array of size(1000) = 36 ms, sum = 5910200000
bitset  's visit dense  array of size(1000) = 299 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 38 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 300 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 37 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 299 ms, sum = 450012500000
bitset  's visit sparse array of size(10000) = 39 ms, sum = 52298530000
bitset  's visit dense  array of size(10000) = 292 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 40 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 296 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 44 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 311 ms, sum = 4495004570000
bitset  's visit sparse array of size(100000) = 47 ms, sum = 500365681000
bitset  's visit dense  array of size(100000) = 306 ms, sum = 4495004570000
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
Bitset64's count sparse array of size(100) = 156 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 156 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 154 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 151 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 149 ms, sum = 400000000
bitset  's count dense  array of size(100) = 151 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 14 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 16 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 15 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 15 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 118 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 117 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 1 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 1 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 1 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 1 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 106 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 114 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 107 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 106 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 140 ms, sum = 32
Bitset64's set dense  array of size(32) = 141 ms, sum = 32
Bitset32's set sparse array of size(32) = 114 ms, sum = 32
Bitset32's set dense  array of size(32) = 120 ms, sum = 32
bitset  's set sparse array of size(32) = 0 ms, sum = 32
bitset  's set dense  array of size(32) = 0 ms, sum = 32
Bitset64's set sparse array of size(64) = 68 ms, sum = 64
Bitset64's set dense  array of size(64) = 68 ms, sum = 64
Bitset32's set sparse array of size(64) = 144 ms, sum = 64
Bitset32's set dense  array of size(64) = 144 ms, sum = 64
bitset  's set sparse array of size(64) = 0 ms, sum = 64
bitset  's set dense  array of size(64) = 0 ms, sum = 64
Bitset64's set sparse array of size(100) = 258 ms, sum = 100
Bitset64's set dense  array of size(100) = 272 ms, sum = 100
Bitset32's set sparse array of size(100) = 257 ms, sum = 100
Bitset32's set dense  array of size(100) = 262 ms, sum = 100
bitset  's set sparse array of size(100) = 154 ms, sum = 100
bitset  's set dense  array of size(100) = 163 ms, sum = 100
Bitset64's set sparse array of size(1000) = 28 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 29 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 30 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 29 ms, sum = 1000
bitset  's set sparse array of size(1000) = 18 ms, sum = 1000
bitset  's set dense  array of size(1000) = 16 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 6 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 6 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 6 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 6 ms, sum = 10000
bitset  's set sparse array of size(10000) = 5 ms, sum = 10000
bitset  's set dense  array of size(10000) = 5 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 5 ms, sum = 100000
bitset  's set sparse array of size(100000) = 5 ms, sum = 100000
bitset  's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 228 ms, sum = 0
Bitset64's reset dense  array of size(32) = 256 ms, sum = 0
Bitset32's reset sparse array of size(32) = 263 ms, sum = 0
Bitset32's reset dense  array of size(32) = 263 ms, sum = 0
bitset  's reset sparse array of size(32) = 0 ms, sum = 0
bitset  's reset dense  array of size(32) = 0 ms, sum = 0
Bitset64's reset sparse array of size(64) = 127 ms, sum = 0
Bitset64's reset dense  array of size(64) = 127 ms, sum = 0
Bitset32's reset sparse array of size(64) = 120 ms, sum = 0
Bitset32's reset dense  array of size(64) = 106 ms, sum = 0
bitset  's reset sparse array of size(64) = 0 ms, sum = 0
bitset  's reset dense  array of size(64) = 0 ms, sum = 0
Bitset64's reset sparse array of size(100) = 221 ms, sum = 0
Bitset64's reset dense  array of size(100) = 224 ms, sum = 0
Bitset32's reset sparse array of size(100) = 228 ms, sum = 0
Bitset32's reset dense  array of size(100) = 229 ms, sum = 0
bitset  's reset sparse array of size(100) = 144 ms, sum = 0
bitset  's reset dense  array of size(100) = 141 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 27 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 27 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 27 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 27 ms, sum = 0
bitset  's reset sparse array of size(1000) = 15 ms, sum = 0
bitset  's reset dense  array of size(1000) = 15 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 5 ms, sum = 0
bitset  's reset sparse array of size(10000) = 5 ms, sum = 0
bitset  's reset dense  array of size(10000) = 6 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 5 ms, sum = 0
bitset  's reset sparse array of size(100000) = 5 ms, sum = 0
bitset  's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset64's flip sparse array of size(32) = 2 ms, sum = 5
Bitset64's flip dense  array of size(32) = 3 ms, sum = 31
Bitset32's flip sparse array of size(32) = 157 ms, sum = 5
Bitset32's flip dense  array of size(32) = 159 ms, sum = 31
bitset  's flip sparse array of size(32) = 0 ms, sum = 5
bitset  's flip dense  array of size(32) = 0 ms, sum = 31
Bitset64's flip sparse array of size(64) = 1 ms, sum = 6
Bitset64's flip dense  array of size(64) = 1 ms, sum = 60
Bitset32's flip sparse array of size(64) = 113 ms, sum = 6
Bitset32's flip dense  array of size(64) = 115 ms, sum = 60
bitset  's flip sparse array of size(64) = 0 ms, sum = 6
bitset  's flip dense  array of size(64) = 0 ms, sum = 60
Bitset64's flip sparse array of size(100) = 189 ms, sum = 8
Bitset64's flip dense  array of size(100) = 197 ms, sum = 87
Bitset32's flip sparse array of size(100) = 266 ms, sum = 8
Bitset32's flip dense  array of size(100) = 259 ms, sum = 87
bitset  's flip sparse array of size(100) = 145 ms, sum = 8
bitset  's flip dense  array of size(100) = 149 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 29 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 28 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 41 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 42 ms, sum = 918
bitset  's flip sparse array of size(1000) = 29 ms, sum = 113
bitset  's flip dense  array of size(1000) = 30 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 12 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 11 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 11 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 11 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 11 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 11 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 10 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 10 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 10 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 11 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 10 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 9 ms, sum = 89988

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
Bitset64's visit sparse array of size(32) = 30 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 228 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 23 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 226 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 210 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 14 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 206 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 309 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 291 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 36 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 302 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 36 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 294 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 43 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 302 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 37 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 293 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 41 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 294 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 46 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 289 ms, sum = 4495004570000
Bitset64's visit sparse array of size(32) = 23 ms, sum = 138000000
Bitset64's visit dense  array of size(32) = 230 ms, sum = 990000000
Bitset32's visit sparse array of size(32) = 24 ms, sum = 138000000
Bitset32's visit dense  array of size(32) = 224 ms, sum = 990000000
Bitset64's visit sparse array of size(64) = 17 ms, sum = 207000000
Bitset64's visit dense  array of size(64) = 211 ms, sum = 1896000000
Bitset32's visit sparse array of size(64) = 19 ms, sum = 207000000
Bitset32's visit dense  array of size(64) = 222 ms, sum = 1896000000
Bitset64's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset64's visit dense  array of size(100) = 310 ms, sum = 4381000000
Bitset32's visit sparse array of size(100) = 22 ms, sum = 385000000
Bitset32's visit dense  array of size(100) = 313 ms, sum = 4381000000
Bitset64's visit sparse array of size(1000) = 40 ms, sum = 5910200000
Bitset64's visit dense  array of size(1000) = 333 ms, sum = 45812100000
Bitset32's visit sparse array of size(1000) = 37 ms, sum = 5910200000
Bitset32's visit dense  array of size(1000) = 330 ms, sum = 45812100000
Bitset64's visit sparse array of size(10000) = 42 ms, sum = 52298530000
Bitset64's visit dense  array of size(10000) = 314 ms, sum = 450012500000
Bitset32's visit sparse array of size(10000) = 40 ms, sum = 52298530000
Bitset32's visit dense  array of size(10000) = 324 ms, sum = 450012500000
Bitset64's visit sparse array of size(100000) = 43 ms, sum = 500365681000
Bitset64's visit dense  array of size(100000) = 316 ms, sum = 4495004570000
Bitset32's visit sparse array of size(100000) = 50 ms, sum = 500365681000
Bitset32's visit dense  array of size(100000) = 321 ms, sum = 4495004570000
Bitset64's count sparse array of size(32) = 119 ms, sum = 500000000
Bitset64's count dense  array of size(32) = 96 ms, sum = 3100000000
Bitset32's count sparse array of size(32) = 95 ms, sum = 500000000
Bitset32's count dense  array of size(32) = 96 ms, sum = 3100000000
bitset  's count sparse array of size(32) = 317 ms, sum = 500000000
bitset  's count dense  array of size(32) = 292 ms, sum = 3100000000
Bitset64's count sparse array of size(64) = 48 ms, sum = 300000000
Bitset64's count dense  array of size(64) = 60 ms, sum = 3000000000
Bitset32's count sparse array of size(64) = 61 ms, sum = 300000000
Bitset32's count dense  array of size(64) = 47 ms, sum = 3000000000
bitset  's count sparse array of size(64) = 218 ms, sum = 300000000
bitset  's count dense  array of size(64) = 217 ms, sum = 3000000000
Bitset64's count sparse array of size(100) = 199 ms, sum = 400000000
Bitset64's count dense  array of size(100) = 197 ms, sum = 4350000000
Bitset32's count sparse array of size(100) = 199 ms, sum = 400000000
Bitset32's count dense  array of size(100) = 208 ms, sum = 4350000000
bitset  's count sparse array of size(100) = 432 ms, sum = 400000000
bitset  's count dense  array of size(100) = 465 ms, sum = 4350000000
Bitset64's count sparse array of size(1000) = 20 ms, sum = 565000000
Bitset64's count dense  array of size(1000) = 20 ms, sum = 4590000000
Bitset32's count sparse array of size(1000) = 19 ms, sum = 565000000
Bitset32's count dense  array of size(1000) = 23 ms, sum = 4590000000
bitset  's count sparse array of size(1000) = 276 ms, sum = 565000000
bitset  's count dense  array of size(1000) = 283 ms, sum = 4590000000
Bitset64's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset64's count dense  array of size(10000) = 2 ms, sum = 4502500000
Bitset32's count sparse array of size(10000) = 2 ms, sum = 520500000
Bitset32's count dense  array of size(10000) = 1 ms, sum = 4502500000
bitset  's count sparse array of size(10000) = 209 ms, sum = 520500000
bitset  's count dense  array of size(10000) = 206 ms, sum = 4502500000
Bitset64's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset64's count dense  array of size(100000) = 0 ms, sum = 4499400000
Bitset32's count sparse array of size(100000) = 0 ms, sum = 500100000
Bitset32's count dense  array of size(100000) = 0 ms, sum = 4499400000
bitset  's count sparse array of size(100000) = 202 ms, sum = 500100000
bitset  's count dense  array of size(100000) = 203 ms, sum = 4499400000
Bitset64's set sparse array of size(32) = 195 ms, sum = 32
Bitset64's set dense  array of size(32) = 195 ms, sum = 32
Bitset32's set sparse array of size(32) = 201 ms, sum = 32
Bitset32's set dense  array of size(32) = 196 ms, sum = 32
bitset  's set sparse array of size(32) = 120 ms, sum = 32
bitset  's set dense  array of size(32) = 95 ms, sum = 32
Bitset64's set sparse array of size(64) = 95 ms, sum = 64
Bitset64's set dense  array of size(64) = 98 ms, sum = 64
Bitset32's set sparse array of size(64) = 218 ms, sum = 64
Bitset32's set dense  array of size(64) = 207 ms, sum = 64
bitset  's set sparse array of size(64) = 49 ms, sum = 64
bitset  's set dense  array of size(64) = 48 ms, sum = 64
Bitset64's set sparse array of size(100) = 368 ms, sum = 100
Bitset64's set dense  array of size(100) = 358 ms, sum = 100
Bitset32's set sparse array of size(100) = 350 ms, sum = 100
Bitset32's set dense  array of size(100) = 378 ms, sum = 100
bitset  's set sparse array of size(100) = 228 ms, sum = 100
bitset  's set dense  array of size(100) = 236 ms, sum = 100
Bitset64's set sparse array of size(1000) = 43 ms, sum = 1000
Bitset64's set dense  array of size(1000) = 44 ms, sum = 1000
Bitset32's set sparse array of size(1000) = 43 ms, sum = 1000
Bitset32's set dense  array of size(1000) = 43 ms, sum = 1000
bitset  's set sparse array of size(1000) = 23 ms, sum = 1000
bitset  's set dense  array of size(1000) = 24 ms, sum = 1000
Bitset64's set sparse array of size(10000) = 10 ms, sum = 10000
Bitset64's set dense  array of size(10000) = 12 ms, sum = 10000
Bitset32's set sparse array of size(10000) = 10 ms, sum = 10000
Bitset32's set dense  array of size(10000) = 13 ms, sum = 10000
bitset  's set sparse array of size(10000) = 13 ms, sum = 10000
bitset  's set dense  array of size(10000) = 14 ms, sum = 10000
Bitset64's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset64's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset32's set sparse array of size(100000) = 5 ms, sum = 100000
Bitset32's set dense  array of size(100000) = 5 ms, sum = 100000
bitset  's set sparse array of size(100000) = 5 ms, sum = 100000
bitset  's set dense  array of size(100000) = 5 ms, sum = 100000
Bitset64's reset sparse array of size(32) = 360 ms, sum = 0
Bitset64's reset dense  array of size(32) = 361 ms, sum = 0
Bitset32's reset sparse array of size(32) = 387 ms, sum = 0
Bitset32's reset dense  array of size(32) = 363 ms, sum = 0
bitset  's reset sparse array of size(32) = 96 ms, sum = 0
bitset  's reset dense  array of size(32) = 120 ms, sum = 0
Bitset64's reset sparse array of size(64) = 181 ms, sum = 0
Bitset64's reset dense  array of size(64) = 181 ms, sum = 0
Bitset32's reset sparse array of size(64) = 182 ms, sum = 0
Bitset32's reset dense  array of size(64) = 198 ms, sum = 0
bitset  's reset sparse array of size(64) = 60 ms, sum = 0
bitset  's reset dense  array of size(64) = 48 ms, sum = 0
Bitset64's reset sparse array of size(100) = 343 ms, sum = 0
Bitset64's reset dense  array of size(100) = 365 ms, sum = 0
Bitset32's reset sparse array of size(100) = 350 ms, sum = 0
Bitset32's reset dense  array of size(100) = 335 ms, sum = 0
bitset  's reset sparse array of size(100) = 203 ms, sum = 0
bitset  's reset dense  array of size(100) = 202 ms, sum = 0
Bitset64's reset sparse array of size(1000) = 44 ms, sum = 0
Bitset64's reset dense  array of size(1000) = 44 ms, sum = 0
Bitset32's reset sparse array of size(1000) = 44 ms, sum = 0
Bitset32's reset dense  array of size(1000) = 43 ms, sum = 0
bitset  's reset sparse array of size(1000) = 21 ms, sum = 0
bitset  's reset dense  array of size(1000) = 20 ms, sum = 0
Bitset64's reset sparse array of size(10000) = 11 ms, sum = 0
Bitset64's reset dense  array of size(10000) = 17 ms, sum = 0
Bitset32's reset sparse array of size(10000) = 11 ms, sum = 0
Bitset32's reset dense  array of size(10000) = 14 ms, sum = 0
bitset  's reset sparse array of size(10000) = 10 ms, sum = 0
bitset  's reset dense  array of size(10000) = 15 ms, sum = 0
Bitset64's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset64's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset32's reset sparse array of size(100000) = 5 ms, sum = 0
Bitset32's reset dense  array of size(100000) = 5 ms, sum = 0
bitset  's reset sparse array of size(100000) = 5 ms, sum = 0
bitset  's reset dense  array of size(100000) = 5 ms, sum = 0
Bitset64's flip sparse array of size(32) = 169 ms, sum = 5
Bitset64's flip dense  array of size(32) = 194 ms, sum = 31
Bitset32's flip sparse array of size(32) = 190 ms, sum = 5
Bitset32's flip dense  array of size(32) = 147 ms, sum = 31
bitset  's flip sparse array of size(32) = 120 ms, sum = 5
bitset  's flip dense  array of size(32) = 96 ms, sum = 31
Bitset64's flip sparse array of size(64) = 73 ms, sum = 6
Bitset64's flip dense  array of size(64) = 96 ms, sum = 60
Bitset32's flip sparse array of size(64) = 87 ms, sum = 6
Bitset32's flip dense  array of size(64) = 85 ms, sum = 60
bitset  's flip sparse array of size(64) = 48 ms, sum = 6
bitset  's flip dense  array of size(64) = 47 ms, sum = 60
Bitset64's flip sparse array of size(100) = 264 ms, sum = 8
Bitset64's flip dense  array of size(100) = 261 ms, sum = 87
Bitset32's flip sparse array of size(100) = 307 ms, sum = 8
Bitset32's flip dense  array of size(100) = 296 ms, sum = 87
bitset  's flip sparse array of size(100) = 315 ms, sum = 8
bitset  's flip dense  array of size(100) = 319 ms, sum = 87
Bitset64's flip sparse array of size(1000) = 44 ms, sum = 113
Bitset64's flip dense  array of size(1000) = 44 ms, sum = 918
Bitset32's flip sparse array of size(1000) = 59 ms, sum = 113
Bitset32's flip dense  array of size(1000) = 58 ms, sum = 918
bitset  's flip sparse array of size(1000) = 32 ms, sum = 113
bitset  's flip dense  array of size(1000) = 31 ms, sum = 918
Bitset64's flip sparse array of size(10000) = 13 ms, sum = 1041
Bitset64's flip dense  array of size(10000) = 13 ms, sum = 9005
Bitset32's flip sparse array of size(10000) = 15 ms, sum = 1041
Bitset32's flip dense  array of size(10000) = 16 ms, sum = 9005
bitset  's flip sparse array of size(10000) = 11 ms, sum = 1041
bitset  's flip dense  array of size(10000) = 11 ms, sum = 9005
Bitset64's flip sparse array of size(100000) = 10 ms, sum = 10002
Bitset64's flip dense  array of size(100000) = 11 ms, sum = 89988
Bitset32's flip sparse array of size(100000) = 10 ms, sum = 10002
Bitset32's flip dense  array of size(100000) = 10 ms, sum = 89988
bitset  's flip sparse array of size(100000) = 9 ms, sum = 10002
bitset  's flip dense  array of size(100000) = 10 ms, sum = 89988

*/
