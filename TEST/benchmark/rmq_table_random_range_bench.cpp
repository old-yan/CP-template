/*
本文件比较一些数据结构维护静态区间最值，面对特殊区间查询的效率比较

在 atcoder C++ 23 (gcc 12.2) 语言条件下测试
cmd:        g++-12 -std=gnu++2b -O2 -DONLINE_JUDGE -DATCODER -Wall -Wextra -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647 -I/opt/ac-library -I/opt/boost/gcc/include -L/opt/boost/gcc/lib-lgmpxx -lgmp -I/usr/include/eigen3
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz

在 atcoder C++ 23 (clang 16.0.6) 语言条件下测试
cmd:        clang++ -std=c++2b -Wall -Wextra -O2 -DONLINE_JUDGE -DATCODER -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647 -I/opt/boost/clang/include -L/opt/boost/clang/lib -I/opt/ac-library -I/usr/include/eigen3 -fuse-ld=lld
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
*/
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "DS/CatTree.h"
#include "DS/CompressedTree.h"
#include "DS/MaskRMQ.h"
#include "DS/MonoAVL.h"
#include "DS/MonoZkwTree.h"
#include "DS/SparseTable.h"
#include "DS/SqrtTree.h"

#include "DS/StaticBufferWrapWithCollect.h"

using std::cout;

namespace Bench {
    struct Result {
        std::string table_name;
        size_t value;
        size_t repeat;
        long long build_time;
        long long query_time;
        long long time_for_1e9_query() const {
            return query_time * 1000000000 / repeat;
        }
    };
    void handle(Result res) {
        static Result ans{res};
        if (res.value * ans.repeat != ans.value * res.repeat) {
            cout << "wrong ans!\n";
            cout << res.table_name << '\n';
            cout << "ans.value = " << ans.value << '\n';
            cout << "ans.repeat = " << ans.repeat << '\n';
            cout << "res.value = " << res.value << '\n';
            cout << "res.repeat = " << res.repeat << '\n';
        }
        auto show_width = [](std::string x, int w) {
            cout << x << std::string(w - x.size(), ' ');
        };
        auto show_ratio = [](long long t1, long long t2) {
            if (t1) {
                int x = round(100.0 * t2 / t1);
                if (x < 10000) cout << ' ';
                if (x < 1000) cout << ' ';
                if (x < 100) cout << ' ';
                if (x < 10) cout << ' ';
                cout << x;
                cout << " %";
            } else if (!t2)
                cout << "  100 %";
            else
                cout << "    inf";
        };
        show_width(std::to_string(res.build_time), 8);
        show_ratio(ans.build_time, res.build_time);
        cout << "\t\t";
        show_width(std::to_string(res.query_time) + "(" + std::to_string(res.repeat) + ")", 16);
        cout << "\t";
        show_width(std::to_string(res.time_for_1e9_query()), 8);
        show_ratio(ans.time_for_1e9_query(), res.time_for_1e9_query());
        cout << "\t\t" << res.table_name << '\t' << res.value << '\n';
    }
    template <typename Table>
    std::mt19937_64 &get_rng() {
        static std::mt19937_64 s_rng;
        return s_rng;
    }
    namespace RandomRange {
        template <size_t Id, typename Table, size_t Repeat, size_t BuildSize, size_t QuerySize>
        Result bench(std::string table_name) {
            uint32_t buffer1[BuildSize], buffer2[QuerySize][2];
            for (size_t j = 0; j != BuildSize; j++) buffer1[j] = get_rng<Table>()();
            for (size_t j = 0; j != QuerySize; j++) {
                buffer2[j][0] = get_rng<Table>()() % BuildSize, buffer2[j][1] = get_rng<Table>()() % BuildSize;
                if (buffer2[j][0] > buffer2[j][1]) std::swap(buffer2[j][0], buffer2[j][1]);
            }
            auto t0 = std::chrono::high_resolution_clock::now();
            Table S(buffer1, buffer1 + BuildSize);
            auto t1 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                for (size_t j = 0; j != QuerySize; j++) {
                    size_t left = buffer2[j][0], right = buffer2[j][1];
                    if constexpr (Id == 0)
                        sum += S.query(left, right);
                    else if constexpr (Id == 1)
                        sum += S.maximum(left, right);
                }
                value += sum;
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            return {table_name, value, Repeat * QuerySize, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count(), std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()};
        }
        template <size_t Id, typename Table, size_t Repeat, size_t BuildSize, size_t QuerySize>
        void run(std::string table_name) {
            auto res = bench<Id, Table, Repeat, BuildSize, QuerySize>(table_name);
            handle(res);
        }
    }
}

#define RUN(...) __VA_ARGS__(#__VA_ARGS__)
struct Node {
    using value_type = uint32_t;
    static uint32_t op(uint32_t x, uint32_t y) { return x > y ? x : y; }
    static bool comp(uint32_t x, uint32_t y) { return x < y; }
    uint32_t m_val;
    const uint32_t &get() const { return m_val; }
    void set(uint32_t val) { m_val = val; }
};
static constexpr size_t N = 1 << 20, M = N * 20, Q = 1 << 10;
int main() {
    cout << "build time\t\tquery time\t\ttime for 1e9 query\n";
    using Monoid = OY::MONOZKW::BaseMonoid<uint32_t, 0, decltype([](auto x, auto y) { return x > y ? x : y; })>;
    RUN(Bench::RandomRange::run<0, OY::ST::Table<Monoid, 21>, 1 << 16, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::CAT::Table<Monoid, 21>, 1 << 16, N, Q>);
    using SqrtMaxTable_random = OY::SQRT::Table<Monoid, OY::SQRT::RandomController<>, 12>;
    RUN(Bench::RandomRange::run<0, SqrtMaxTable_random, 1 << 16, N, Q>);
    using SqrtMaxTable_nonrandom = OY::SQRT::Table<Monoid, OY::SQRT::NonRandomController<>, 16>;
    RUN(Bench::RandomRange::run<0, SqrtMaxTable_nonrandom, 1 << 16, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t>, 1 << 15, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::MONOZKW::Tree<Monoid>, 1 << 13, N, Q>);
    using StaticCompressedMaxTree = OY::CPTREE::Tree<Monoid, uint32_t, OY::StaticBufferWrapWithCollect<N * 2>::type>;
    RUN(Bench::RandomRange::run<0, StaticCompressedMaxTree, 1 << 10, N, Q>);
    struct AVLMaxTree {
        using table_type = OY::MonoMaxAVL<uint32_t, 0, false, N>;
        table_type m_avl;
        AVLMaxTree(uint32_t *first, uint32_t *last) : m_avl(table_type::from_sorted(first, last)) {}
        uint32_t query(uint32_t left, uint32_t right) const { return m_avl.query(left, right); }
    };
    RUN(Bench::RandomRange::run<0, AVLMaxTree, 1 << 9, N, Q>);
}
/*
atcoder g++
build time		query time		time for 1e9 query
43        100 %		139(67108864)   	2071      100 %		Bench::RandomRange::run<0, OY::ST::Table<Monoid, 21>, 1 << 16, N, Q>	288226541672398848
52        121 %		129(67108864)   	1922       93 %		Bench::RandomRange::run<0, OY::CAT::Table<Monoid, 21>, 1 << 16, N, Q>	288226541672398848
8          19 %		153(67108864)   	2279      110 %		Bench::RandomRange::run<0, SqrtMaxTable_random, 1 << 16, N, Q>	288226541672398848
9          21 %		178(67108864)   	2652      128 %		Bench::RandomRange::run<0, SqrtMaxTable_nonrandom, 1 << 16, N, Q>	288226541672398848
15         35 %		345(33554432)   	10281     496 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t>, 1 << 15, N, Q>	144113270836199424
4           9 %		241(8388608)    	28729    1387 %		Bench::RandomRange::run<0, OY::MONOZKW::Tree<Monoid>, 1 << 13, N, Q>	36028317709049856
15         35 %		594(1048576)    	566482  27353 %		Bench::RandomRange::run<0, StaticCompressedMaxTree, 1 << 10, N, Q>	4503539713631232
14         33 %		575(524288)     	1096725 52956 %		Bench::RandomRange::run<0, AVLMaxTree, 1 << 9, N, Q>	2251769856815616

*/
/*
atcoder clang++
build time		query time		time for 1e9 query
38        100 %		118(67108864)   	1758      100 %		Bench::RandomRange::run<0, OY::ST::Table<Monoid, 21>, 1 << 16, N, Q>	288226541672398848
51        134 %		136(67108864)   	2026      115 %		Bench::RandomRange::run<0, OY::CAT::Table<Monoid, 21>, 1 << 16, N, Q>	288226541672398848
9          24 %		183(67108864)   	2726      155 %		Bench::RandomRange::run<0, SqrtMaxTable_random, 1 << 16, N, Q>	288226541672398848
9          24 %		229(67108864)   	3412      194 %		Bench::RandomRange::run<0, SqrtMaxTable_nonrandom, 1 << 16, N, Q>	288226541672398848
15         39 %		360(33554432)   	10728     610 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t>, 1 << 15, N, Q>	144113270836199424
3           8 %		247(8388608)    	29444    1675 %		Bench::RandomRange::run<0, OY::MONOZKW::Tree<Monoid>, 1 << 13, N, Q>	36028317709049856
16         42 %		699(1048576)    	666618  37919 %		Bench::RandomRange::run<0, StaticCompressedMaxTree, 1 << 10, N, Q>	4503539713631232
14         37 %		659(524288)     	1256942 71498 %		Bench::RandomRange::run<0, AVLMaxTree, 1 << 9, N, Q>	2251769856815616

*/
