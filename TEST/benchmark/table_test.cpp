#include <chrono>
#include <random>

#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
#include "DS/STTable.h"
#include "DS/SegTree.h"
#include "DS/SqrtTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

static constexpr size_t D = 20, N = 1 << D, Init_M = 21, Query_M = 11;
uint32_t array[N], rand_q[N][2], single_q[N][2], inner_q[N][2];
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
#define test_table_init(Table, Size, Clear)                                                                           \
    {                                                                                                                 \
        for (size_t i = 0; i < Init_M; i++) {                                                                         \
            Table<Node, Size> S(array, array + N);                                                                    \
            Clear;                                                                                                    \
            if (!i) timer_start;                                                                                      \
        }                                                                                                             \
        timer_end;                                                                                                    \
        cout << to_string_of_len(#Table, 15) << "'s init * " << Init_M - 1 << " = " << duration_get << " ms" << endl; \
    }
#define test_table_query(Table, Size, Clear, Q)                                                                                                    \
    {                                                                                                                                              \
        Table<Node, Size> S(array, array + N);                                                                                                     \
        Clear;                                                                                                                                     \
        sum = 0;                                                                                                                                   \
        timer_start;                                                                                                                               \
        for (size_t i = 0; i < Query_M; i++) {                                                                                                     \
            for (size_t i = 0; i < N; i++) {                                                                                                       \
                auto l = Q[i][0], r = Q[i][1];                                                                                                     \
                sum ^= S.query(l, r);                                                                                                              \
            }                                                                                                                                      \
        }                                                                                                                                          \
        timer_end;                                                                                                                                 \
        cout << to_string_of_len(#Table, 15) << "'s query on " << #Q << " * " << Query_M << " = " << duration_get << " ms, sum = " << sum << endl; \
    }
#define test_table_query_all(Table, Size, Clear)    \
    test_table_query(Table, Size, Clear, rand_q);   \
    test_table_query(Table, Size, Clear, single_q); \
    test_table_query(Table, Size, Clear, inner_q);

struct Node {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    static uint32_t op(uint32_t x, uint32_t y) { return x > y ? x : y; }
    static bool comp(uint32_t x, uint32_t y) { return x < y; }
    uint32_t m_val;
    const uint32_t &get() const { return m_val; }
    void set(uint32_t val) { m_val = val; }
};
template <typename Node, uint32_t MAX_NODE>
using SegTable = OY::Seg::Tree<Node, OY::Seg::NoInit, true, uint32_t, MAX_NODE>;
template <typename Node, size_t MAX_NODE>
using SqrtSTTable = OY::Sqrt::Table<Node, OY::PrefixTable, OY::ST::Table, MAX_NODE>;
template <typename Node, size_t MAX_NODE>
using SqrtCatTable = OY::Sqrt::Table<Node, OY::PrefixTable, OY::Cat::Table, MAX_NODE>;
template <typename Node, size_t MAX_NODE>
using SqrtZkwTable = OY::Sqrt::Table<Node, OY::PrefixTable, OY::Zkw::Tree, MAX_NODE>;
template <typename Node, size_t MAX_NODE>
using SqrtSegTable = OY::Sqrt::Table<Node, OY::PrefixTable, SegTable, MAX_NODE>;
template <typename Node, uint32_t MAX_NODE>
using MaskRMQTable = OY::MaskRMQ::ValueTable<Node, uint64_t, OY::ST::Table, MAX_NODE>;
int main() {
    for (size_t i = 0; i < N; i++) array[i] = rr();
    for (size_t i = 0; i < N; i++) {
        rand_q[i][0] = rr() % N, rand_q[i][1] = rr() % N;
        if (rand_q[i][0] > rand_q[i][1]) std::swap(rand_q[i][0], rand_q[i][1]);
    }
    for (size_t i = 0; i < N; i++) {
        single_q[i][0] = single_q[i][1] = rr() % N;
    }
    for (size_t i = 0; i < N; i++) {
        size_t j = rr() % (N / 1024);
        inner_q[i][0] = j * 1024 + rr() % 1024, inner_q[i][1] = j * 1024 + rr() % 1024;
        if (inner_q[i][0] > inner_q[i][1]) std::swap(inner_q[i][0], inner_q[i][1]);
    }

    test_table_init(OY::ST::Table, N * D, S.s_use_count = 0);
    test_table_init(OY::Cat::Table, N * D, S.s_use_count = 0);
    test_table_init(OY::Zkw::Tree, N * 2, S.s_use_count = 0);
    test_table_init(SegTable, N * 2, S.s_use_count = 1);
    test_table_init(SqrtCatTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtSTTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtZkwTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtSegTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);

    test_table_query_all(OY::ST::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Cat::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Zkw::Tree, N * 2, S.s_use_count = 0);
    test_table_query_all(SegTable, N * 2, S.s_use_count = 1);
    test_table_query_all(SqrtCatTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtSTTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtZkwTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtSegTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 282 ms
OY::Cat::Table 's init * 20 = 275 ms
OY::Zkw::Tree  's init * 20 = 18 ms
SegTable       's init * 20 = 91 ms
SqrtCatTable   's init * 20 = 74 ms
SqrtSTTable    's init * 20 = 72 ms
SqrtZkwTable   's init * 20 = 74 ms
SqrtSegTable   's init * 20 = 73 ms
MaskRMQTable   's init * 20 = 153 ms
OY::ST::Table  's query on rand_q * 11 = 163 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 84 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 152 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 148 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 26 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 143 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1232 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 24 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 603 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 4671 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 1781 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 2854 ms, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 153 ms, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 44 ms, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 2086 ms, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 144 ms, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 64 ms, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 1997 ms, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 599 ms, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 46 ms, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 1980 ms, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 110 ms, sum = 3072061456
SqrtSegTable   's query on single_q * 11 = 40 ms, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 1991 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 315 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 159 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 293 ms, sum = 3315072950

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 159 ms
OY::Cat::Table 's init * 20 = 340 ms
OY::Zkw::Tree  's init * 20 = 17 ms
SegTable       's init * 20 = 112 ms
SqrtCatTable   's init * 20 = 50 ms
SqrtSTTable    's init * 20 = 50 ms
SqrtZkwTable   's init * 20 = 52 ms
SqrtSegTable   's init * 20 = 49 ms
MaskRMQTable   's init * 20 = 153 ms
OY::ST::Table  's query on rand_q * 11 = 112 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 38 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 125 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 136 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 20 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 114 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1208 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 21 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 586 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 5310 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 1728 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 3396 ms, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 127 ms, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 67 ms, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 3007 ms, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 124 ms, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 50 ms, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 2973 ms, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 611 ms, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 63 ms, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 2955 ms, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 140 ms, sum = 3072061456
SqrtSegTable   's query on single_q * 11 = 55 ms, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 2915 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 308 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 146 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 282 ms, sum = 3315072950

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
OY::ST::Table  's init * 20 = 403 ms
OY::Cat::Table 's init * 20 = 647 ms
OY::Zkw::Tree  's init * 20 = 30 ms
SegTable       's init * 20 = 116 ms
SqrtCatTable   's init * 20 = 83 ms
SqrtSTTable    's init * 20 = 83 ms
SqrtZkwTable   's init * 20 = 84 ms
SqrtSegTable   's init * 20 = 86 ms
MaskRMQTable   's init * 20 = 181 ms
OY::ST::Table  's query on rand_q * 11 = 182 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 43 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 177 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 165 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 33 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 188 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1409 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 23 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 643 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 5773 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 1846 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 3557 ms, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 163 ms, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 66 ms, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 610 ms, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 173 ms, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 71 ms, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 600 ms, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 643 ms, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 78 ms, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 610 ms, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 176 ms, sum = 3072061456
SqrtSegTable   's query on single_q * 11 = 68 ms, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 593 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 464 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 251 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 388 ms, sum = 3315072950


*/
