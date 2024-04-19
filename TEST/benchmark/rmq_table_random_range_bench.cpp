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
    RUN(Bench::RandomRange::run<0, OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>);
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
36        100 %		53(33554432)    	1579      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
46        128 %		71(33554432)    	2115      134 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
3           8 %		106(1048576)    	101089   6402 %		Bench::RandomRange::run<0, OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4503539713631232
11         31 %		170(524288)     	324249  20535 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
13         36 %		171(16777216)   	10192     645 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          25 %		82(33554432)    	2443      155 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
9          25 %		93(33554432)    	2771      175 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
195       542 %		108(524288)     	205993  13046 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
348       967 %		122(1048576)    	116348   7368 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
200       556 %		94(1048576)     	89645    5677 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/
/*
atcoder clang++
build time		query time		time for 1e10 query
27        100 %		56(33554432)    	1668      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
44        163 %		57(33554432)    	1698      102 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
3          11 %		105(1048576)    	100135   6003 %		Bench::RandomRange::run<0, OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4503539713631232
14         52 %		221(524288)     	421524  25271 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
14         52 %		169(16777216)   	10073     604 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          33 %		99(33554432)    	2950      177 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
9          33 %		118(33554432)   	3516      211 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
174       644 %		111(524288)     	211715  12693 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
335      1241 %		127(1048576)    	121116   7261 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
192       711 %		92(1048576)     	87738    5260 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/
/*
luogu g++
build time		query time		time for 1e10 query
35        100 %		49(33554432)    	1460      100 %		Bench::RandomRange::run<0, OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
39        111 %		57(33554432)    	1698      116 %		Bench::RandomRange::run<0, OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	144113270836199424
2           6 %		108(1048576)    	102996   7055 %		Bench::RandomRange::run<0, OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4503539713631232
10         29 %		204(524288)     	389099  26651 %		Bench::RandomRange::run<0, OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, N * 2>, 1 << 9, N, Q>	2251769856815616
14         40 %		147(16777216)   	8761      600 %		Bench::RandomRange::run<0, OY::MaskRMQMaxValueTable<uint32_t, uint64_t, N>, 1 << 14, N, Q>	72056635418099712
9          26 %		90(33554432)    	2682      184 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, true>, 1 << 15, N, Q>	144113270836199424
9          26 %		92(33554432)    	2741      188 %		Bench::RandomRange::run<0, OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	144113270836199424
178       509 %		108(524288)     	205993  14109 %		Bench::RandomRange::run<1, OY::WaveLet::Table<uint32_t>, 1 << 9, N, Q>	2251769856815616
320       914 %		127(1048576)    	121116   8296 %		Bench::RandomRange::run<1, OY::WaveLet::Tree<uint32_t>, 1 << 10, N, Q>	4503539713631232
175       500 %		100(1048576)    	95367    6532 %		Bench::RandomRange::run<1, OY::DIVIDE::Tree<Node, M>, 1 << 10, N, Q>	4503539713631232

*/