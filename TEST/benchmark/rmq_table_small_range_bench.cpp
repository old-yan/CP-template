/*
本文件比较各种数据结构维护静态区间最值，面对特定小区间查询的效率比较
“小区间” 意味着查询的左端点和右端点在 SqrtTree(RandomData=True) 的一个块之内
可以看到此时， RandomData=True 的 SqrtTree 比 RandomData=False 的 SqrtTree 要慢了

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
#include "DS/STTable.h"
#include "DS/SqrtTree.h"
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
    namespace SmallRange {
        template <typename Table, size_t Repeat, size_t BuildSize, size_t QuerySize>
        Result bench(std::string table_name) {
            uint32_t buffer1[BuildSize], buffer2[QuerySize][2];
            for (size_t j = 0; j != BuildSize; j++) buffer1[j] = get_rng<Table>()();
            size_t block = 1 << (std::bit_width(BuildSize) - 1) / 2;
            for (size_t j = 0; j != QuerySize; j++) {
                size_t x = get_rng<Table>()() % (BuildSize / block);
                buffer2[j][0] = x * block + get_rng<Table>()() % block, buffer2[j][1] = x * block + get_rng<Table>()() % block;
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
                    sum += S.query(left, right);
                }
                value += sum;
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            return {table_name, value, Repeat * QuerySize, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count(), std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()};
        }
        template <typename Table, size_t Repeat, size_t BuildSize, size_t QuerySize>
        void run(std::string table_name) {
            auto res = bench<Table, Repeat, BuildSize, QuerySize>(table_name);
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
    RUN(Bench::SmallRange::run<OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>);
    RUN(Bench::SmallRange::run<OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>);
    RUN(Bench::SmallRange::run<OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>);
    RUN(Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, true>, 1 << 10, N, Q>);
    RUN(Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>);
}
/*
atcoder g++
build time		query time		time for 1e10 query
36        100 %		47(33554432)    	1400      100 %		Bench::SmallRange::run<OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
47        131 %		62(33554432)    	1847      132 %		Bench::SmallRange::run<OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
3           8 %		43(1048576)     	41007    2929 %		Bench::SmallRange::run<OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4456610382960640
9          25 %		111(1048576)    	105857   7561 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, true>, 1 << 10, N, Q>	4456610382960640
9          25 %		94(33554432)    	2801      200 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	142611532254740480

*/
/*
atcoder clang++
build time		query time		time for 1e10 query
29        100 %		44(33554432)    	1311      100 %		Bench::SmallRange::run<OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
44        152 %		50(33554432)    	1490      114 %		Bench::SmallRange::run<OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
3          10 %		28(1048576)     	26702    2037 %		Bench::SmallRange::run<OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4456610382960640
10         34 %		50(1048576)     	47683    3637 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, true>, 1 << 10, N, Q>	4456610382960640
9          31 %		118(33554432)   	3516      268 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	142611532254740480

*/
/*
luogu g++
build time		query time		time for 1e10 query
34        100 %		49(33554432)    	1460      100 %		Bench::SmallRange::run<OY::STMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
39        115 %		57(33554432)    	1698      116 %		Bench::SmallRange::run<OY::CatMaxTable<uint32_t, M>, 1 << 15, N, Q>	142611532254740480
3           9 %		42(1048576)     	40054    2743 %		Bench::SmallRange::run<OY::Zkw::Tree<Node, N * 2>, 1 << 10, N, Q>	4456610382960640
8          24 %		111(1048576)    	105857   7250 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, true>, 1 << 10, N, Q>	4456610382960640
9          26 %		103(33554432)   	3069      210 %		Bench::SmallRange::run<OY::SqrtMaxTable<uint32_t, N, false>, 1 << 15, N, Q>	142611532254740480

*/