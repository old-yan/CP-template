/*
本文件在 C++20 标准下编译运行，比较各种树/表在初始化、查询时的效率高低
*/
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
std::string to_string_of_len(const char* s, size_t len) {
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
    static uint32_t op(uint32_t x, uint32_t y) { return x > y ? x : y; }
    static bool comp(uint32_t x, uint32_t y) { return x < y; }
    uint32_t m_val;
    const uint32_t& get() const { return m_val; }
    void set(uint32_t val) { m_val = val; }
};
template <typename Node, uint32_t MAX_NODE>
using SegTable = OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, MAX_NODE>;
template <typename Node, uint32_t MAX_NODE>
using MaskRMQTable = OY::MaskRMQ::ValueTable<Node, uint64_t, MAX_NODE>;
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
    test_table_init(OY::Sqrt::Table, N, S.m_inter_table.s_use_count = 0);
    test_table_init(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);

    test_table_query_all(OY::ST::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Cat::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Zkw::Tree, N * 2, S.s_use_count = 0);
    test_table_query_all(SegTable, N * 2, S.s_use_count = 1);
    test_table_query_all(OY::Sqrt::Table, N, S.m_inter_table.s_use_count = 0);
    test_table_query_all(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 315 ms
OY::Cat::Table 's init * 20 = 280 ms
OY::Zkw::Tree  's init * 20 = 24 ms
SegTable       's init * 20 = 136 ms
OY::Sqrt::Table's init * 20 = 163 ms
MaskRMQTable   's init * 20 = 194 ms
OY::ST::Table  's query on rand_q * 11 = 151 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 74 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 199 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 186 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 28 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 157 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1425 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 25 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 696 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 6621 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 2848 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 4741 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 183 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 33 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 196 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 460 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 224 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 370 ms, sum = 3315072950

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 184 ms
OY::Cat::Table 's init * 20 = 404 ms
OY::Zkw::Tree  's init * 20 = 20 ms
SegTable       's init * 20 = 139 ms
OY::Sqrt::Table's init * 20 = 143 ms
MaskRMQTable   's init * 20 = 181 ms
OY::ST::Table  's query on rand_q * 11 = 130 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 52 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 156 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 152 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 24 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 137 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1502 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 28 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 723 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 6729 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 2386 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 4562 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 162 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 30 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 159 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 439 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 217 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 358 ms, sum = 3315072950

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
OY::ST::Table  's init * 20 = 626 ms
OY::Cat::Table 's init * 20 = 750 ms
OY::Zkw::Tree  's init * 20 = 38 ms
SegTable       's init * 20 = 160 ms
OY::Sqrt::Table's init * 20 = 171 ms
MaskRMQTable   's init * 20 = 215 ms
OY::ST::Table  's query on rand_q * 11 = 227 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 71 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 216 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 201 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 32 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 168 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1532 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 32 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 761 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 7271 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 2791 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 5125 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 291 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 59 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 260 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 687 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 397 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 620 ms, sum = 3315072950

*/
