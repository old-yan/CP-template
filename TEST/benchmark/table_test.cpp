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
    static uint32_t op(uint32_t x, uint32_t y) { return x > y ? x : y; }
    static bool comp(uint32_t x, uint32_t y) { return x < y; }
    uint32_t m_val;
    const uint32_t &get() const { return m_val; }
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
    test_table_init(OY::Sqrt::Table, N, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_init(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);

    test_table_query_all(OY::ST::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Cat::Table, N * D, S.s_use_count = 0);
    test_table_query_all(OY::Zkw::Tree, N * 2, S.s_use_count = 0);
    test_table_query_all(SegTable, N * 2, S.s_use_count = 1);
    test_table_query_all(OY::Sqrt::Table, N, S.m_inter_table.s_use_count = S.m_inner_table[0].s_use_count = 0);
    test_table_query_all(MaskRMQTable, N, S.m_table.s_use_count = S.m_table.m_inter_table.s_use_count = 0);
}

/*
gcc version 11.2.0
g++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 284 ms
OY::Cat::Table 's init * 20 = 252 ms
OY::Zkw::Tree  's init * 20 = 23 ms
SegTable       's init * 20 = 85 ms
OY::Sqrt::Table's init * 20 = 76 ms
MaskRMQTable   's init * 20 = 155 ms
OY::ST::Table  's query on rand_q * 11 = 159 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 44 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 165 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 147 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 34 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 145 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1281 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 27 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 627 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 5956 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 2163 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 4055 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 225 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 119 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 3044 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 422 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 221 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 379 ms, sum = 3315072950

*/
/*
clang version 12.0.1
clang++ -std=c++20 -DOY_LOCAL -O2 -march=native
OY::ST::Table  's init * 20 = 155 ms
OY::Cat::Table 's init * 20 = 325 ms
OY::Zkw::Tree  's init * 20 = 18 ms
SegTable       's init * 20 = 117 ms
OY::Sqrt::Table's init * 20 = 58 ms
MaskRMQTable   's init * 20 = 200 ms
OY::ST::Table  's query on rand_q * 11 = 114 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 40 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 112 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 149 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 20 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 123 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1214 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 21 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 599 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 5885 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 1956 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 3691 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 219 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 118 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 4128 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 323 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 150 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 284 ms, sum = 3315072950

*/
/*
Visual Studio 2019 (v142)
std:c++20 x64 Release
OY::ST::Table  's init * 20 = 422 ms
OY::Cat::Table 's init * 20 = 652 ms
OY::Zkw::Tree  's init * 20 = 34 ms
SegTable       's init * 20 = 126 ms
OY::Sqrt::Table's init * 20 = 114 ms
MaskRMQTable   's init * 20 = 207 ms
OY::ST::Table  's query on rand_q * 11 = 224 ms, sum = 3081640622
OY::ST::Table  's query on single_q * 11 = 45 ms, sum = 3938828962
OY::ST::Table  's query on inner_q * 11 = 212 ms, sum = 3315072950
OY::Cat::Table 's query on rand_q * 11 = 172 ms, sum = 3081640622
OY::Cat::Table 's query on single_q * 11 = 39 ms, sum = 3938828962
OY::Cat::Table 's query on inner_q * 11 = 213 ms, sum = 3315072950
OY::Zkw::Tree  's query on rand_q * 11 = 1265 ms, sum = 3081640622
OY::Zkw::Tree  's query on single_q * 11 = 22 ms, sum = 3938828962
OY::Zkw::Tree  's query on inner_q * 11 = 578 ms, sum = 3315072950
SegTable       's query on rand_q * 11 = 5475 ms, sum = 3081640622
SegTable       's query on single_q * 11 = 1854 ms, sum = 3938828962
SegTable       's query on inner_q * 11 = 3647 ms, sum = 3315072950
OY::Sqrt::Table's query on rand_q * 11 = 332 ms, sum = 3081640622
OY::Sqrt::Table's query on single_q * 11 = 188 ms, sum = 3938828962
OY::Sqrt::Table's query on inner_q * 11 = 2685 ms, sum = 3315072950
MaskRMQTable   's query on rand_q * 11 = 513 ms, sum = 3081640622
MaskRMQTable   's query on single_q * 11 = 298 ms, sum = 3938828962
MaskRMQTable   's query on inner_q * 11 = 433 ms, sum = 3315072950

*/
