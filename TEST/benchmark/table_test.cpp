#include <chrono>
#include <random>

#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
#include "DS/SegTree.h"
#include "DS/SqrtTree.h"
#include "DS/STTable.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

static constexpr size_t D = 20, N = 1 << D, Init_M = 21, Query_M = 11;
uint32_t array[N], rand_q[N][2], single_q[N][2], inner_q[N][2];
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
#define test_table_init(Table, Size, Clear)                                                                  \
    {                                                                                                        \
        for (size_t i = 0; i < Init_M; i++) {                                                                \
            Table<Node, Size> S(array, array + N);                                                           \
            Clear;                                                                                           \
            if (!i) timer_start;                                                                             \
        }                                                                                                    \
        timer_end;                                                                                           \
        cout << to_string_of_len(#Table, 15) << "'s init * " << Init_M - 1 << " = " << duration_get<<" ms" << endl; \
    }
#define test_table_query(Table, Size, Clear, Q)                                                                                                 \
    {                                                                                                                                           \
        Table<Node, Size> S(array, array + N);                                                                                                  \
        Clear;                                                                                                                                  \
        sum = 0;                                                                                                                                \
        timer_start;                                                                                                                            \
        for (size_t i = 0; i < Query_M; i++) {                                                                                                  \
            for (size_t i = 0; i < N; i++) {                                                                                                    \
                auto l = Q[i][0], r = Q[i][1];                                                                                                  \
                sum ^= S.query(l, r);                                                                                                           \
            }                                                                                                                                   \
        }                                                                                                                                       \
        timer_end;                                                                                                                              \
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
    const uint32_t& get() const { return m_val; }
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
    test_table_init(SegTable, N * 2, S.s_use_count = 0);
    test_table_init(SqrtCatTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtSTTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtZkwTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(SqrtSegTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);

    test_table_query_all(OY::ST::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Cat::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Zkw::Tree, N * 2, S.s_use_count = 0);
    test_table_query_all(SegTable, N * 2, S.s_use_count = 0);
    test_table_query_all(SqrtCatTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtSTTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtZkwTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(SqrtSegTable, N * 3, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2
OY::ST::Table  's init * 20 = 281
OY::Cat::Table 's init * 20 = 261
OY::Zkw::Tree  's init * 20 = 19
SegTable       's init * 20 = 86
SqrtCatTable   's init * 20 = 76
SqrtSTTable    's init * 20 = 75
SqrtZkwTable   's init * 20 = 75
SqrtSegTable   's init * 20 = 75
MaskRMQTable   's init * 20 = 160
OY::ST::Table  's query on rand_q * 11 = 185, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 50, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 180, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 169, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 32, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 166, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1336, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 25, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 661, sum = 3315072950
SegTable       's query on rand_q * 11 = 5808, sum = 3081640622
SegTable       's query on single_q * 11 = 2163, sum = 3938828962
SegTable       's query on inner_q * 11 = 3738, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 145, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 52, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 2066, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 156, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 53, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 2076, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 643, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 53, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 2093, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 950, sum = 3081640622
SqrtSegTable   's query on single_q * 11 = 52, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 2082, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 531, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 271, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 464, sum = 3315072950

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2
OY::ST::Table  's init * 20 = 304
OY::Cat::Table 's init * 20 = 469
OY::Zkw::Tree  's init * 20 = 21
SegTable       's init * 20 = 150
SqrtCatTable   's init * 20 = 54
SqrtSTTable    's init * 20 = 52
SqrtZkwTable   's init * 20 = 50
SqrtSegTable   's init * 20 = 55
MaskRMQTable   's init * 20 = 182
OY::ST::Table  's query on rand_q * 11 = 241, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 42, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 206, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 143, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 23, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 134, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1354, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 31, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 647, sum = 3315072950
SegTable       's query on rand_q * 11 = 7023, sum = 3081640622
SegTable       's query on single_q * 11 = 2825, sum = 3938828962
SegTable       's query on inner_q * 11 = 4294, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 159, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 62, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 3347, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 229, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 66, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 3285, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 682, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 69, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 3138, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 1454, sum = 3081640622
SqrtSegTable   's query on single_q * 11 = 68, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 3127, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 380, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 238, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 356, sum = 3315072950

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
OY::ST::Table  's init * 20 = 441 ms
OY::Cat::Table 's init * 20 = 717 ms
OY::Zkw::Tree  's init * 20 = 34 ms
SegTable       's init * 20 = 160 ms
SqrtCatTable   's init * 20 = 92 ms
SqrtSTTable    's init * 20 = 102 ms
SqrtZkwTable   's init * 20 = 125 ms
SqrtSegTable   's init * 20 = 91 ms
MaskRMQTable   's init * 20 = 190 ms
OY::ST::Table  's query on rand_q * 11 = 221 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 54 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 217 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 191 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 31 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 184 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1315 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 31 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 647 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 6504 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 2406 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 4601 ms, sum = 3315072950
SqrtCatTable   's query on rand_q * 11 = 221 ms, sum = 3081640622
SqrtCatTable   's query on single_q * 11 = 85 ms, sum = 3938828962
SqrtCatTable   's query on inner_q * 11 = 723 ms, sum = 3315072950
SqrtSTTable    's query on rand_q * 11 = 230 ms, sum = 3081640622
SqrtSTTable    's query on single_q * 11 = 83 ms, sum = 3938828962
SqrtSTTable    's query on inner_q * 11 = 713 ms, sum = 3315072950
SqrtZkwTable   's query on rand_q * 11 = 728 ms, sum = 3081640622
SqrtZkwTable   's query on single_q * 11 = 93 ms, sum = 3938828962
SqrtZkwTable   's query on inner_q * 11 = 747 ms, sum = 3315072950
SqrtSegTable   's query on rand_q * 11 = 1645 ms, sum = 3081640622
SqrtSegTable   's query on single_q * 11 = 86 ms, sum = 3938828962
SqrtSegTable   's query on inner_q * 11 = 714 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 515 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 298 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 458 ms, sum = 3315072950

*/
