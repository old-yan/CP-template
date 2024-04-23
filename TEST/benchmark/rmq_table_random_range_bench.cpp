/*
本文件比较一些数据结构维护静态区间最值，面对特殊区间查询的效率比较

在 atcoder C++ 23 (gcc 12.2) 语言条件下测试
cmd:        g++-12 -std=gnu++2b -O2 -DONLINE_JUDGE -DATCODER -Wall -Wextra -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647 -I/opt/ac-library -I/opt/boost/gcc/include -L/opt/boost/gcc/lib-lgmpxx -lgmp -I/usr/include/eigen3
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz

在 atcoder C++ 23 (clang 16.0.6) 语言条件下测试
cmd:        clang++ -std=c++2b -Wall -Wextra -O2 -DONLINE_JUDGE -DATCODER -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647 -I/opt/boost/clang/include -L/opt/boost/clang/lib -I/opt/ac-library -I/usr/include/eigen3 -fuse-ld=lld
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz

在 luogu C++ 20 (gcc 13.2.0) 语言条件下测试
cmd:        g++ -x c++ -std=C++2a -fPIC -DONLINE_JUDGE -Wall -fno-asm -lm -march=native
CPU:        Intel(R) Xeon(R) Platinum 8369HC CPU @ 3.30GHz
*/
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "DS/CatTree.h"
#include "DS/DivideTree.h"
#include "DS/MaskRMQ.h"
#include "DS/STTable.h"
#include "DS/SegTree.h"
#include "DS/SqrtTree.h"
#include "DS/WaveLet.h"
#include "DS/ZkwTree.h"

using std::cout;

namespace Bench {
    struct Result {
        std::string table_name;
        size_t value;
        size_t repeat;
        long long build_time;
        long long query_time;
        long long time_for_1e10_query() const {
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
        show_width(std::to_string(res.time_for_1e10_query()), 8);
        show_ratio(ans.time_for_1e10_query(), res.time_for_1e10_query());
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
    cout << "build time\t\tquery time\t\ttime for 1e10 query\n";
    RUN(Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::ZKW::Tree<Node>, 1 << 10, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>);
    RUN(Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>);
    RUN(Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>);
    RUN(Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>);
    RUN(Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>);
}
/*
atcoder g++
build time		query time		time for 1e10 query
36        100 %		54(33554432)    	1609      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
46        128 %		69(33554432)    	2056      128 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
4          11 %		124(1048576)    	118255   7350 %		Bench::RandomRange::run<0, OY::ZKW::Tree<Node>, 1 << 10, N, Q>	4503539713631232
11         31 %		178(524288)     	339508  21101 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
13         36 %		169(16777216)   	10073     626 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          25 %		83(33554432)    	2473      154 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
6          17 %		93(33554432)    	2771      172 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
195       542 %		109(524288)     	207901  12921 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
345       958 %		126(1048576)    	120162   7468 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
195       542 %		95(1048576)     	90599    5631 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/
/*
atcoder clang++
build time		query time		time for 1e10 query
27        100 %		55(33554432)    	1639      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
43        159 %		58(33554432)    	1728      105 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
3          11 %		116(1048576)    	110626   6750 %		Bench::RandomRange::run<0, OY::ZKW::Tree<Node>, 1 << 10, N, Q>	4503539713631232
13         48 %		199(524288)     	379562  23158 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
14         52 %		167(16777216)   	9953      607 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          33 %		99(33554432)    	2950      180 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
5          19 %		116(33554432)   	3457      211 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
172       637 %		112(524288)     	213623  13034 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
332      1230 %		129(1048576)    	123023   7506 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
190       704 %		93(1048576)     	88691    5411 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/
/*
luogu g++
build time		query time		time for 1e10 query
34        100 %		50(33554432)    	1490      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
40        118 %		57(33554432)    	1698      114 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
4          12 %		108(1048576)    	102996   6912 %		Bench::RandomRange::run<0, OY::ZKW::Tree<Node>, 1 << 10, N, Q>	4503539713631232
10         29 %		205(524288)     	391006  26242 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
13         38 %		144(16777216)   	8583      576 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          26 %		89(33554432)    	2652      178 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
10         29 %		91(33554432)    	2712      182 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
176       518 %		111(524288)     	211715  14209 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
322       947 %		139(1048576)    	132560   8897 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
181       532 %		100(1048576)    	95367    6400 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/