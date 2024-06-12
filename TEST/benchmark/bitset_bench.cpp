/*
本文件比较 std::bitset 与模板库里的 StaticBitset DynamicBitset 效率高低

在 atcoder C++ 23 (gcc 12.2) 语言条件下测试
cmd:        g++-12 -std=gnu++2b -O2 -DONLINE_JUDGE -DATCODER -Wall -Wextra -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647 -I/opt/ac-library -I/opt/boost/gcc/include -L/opt/boost/gcc/lib-lgmpxx -lgmp -I/usr/include/eigen3
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz

在 atcoder C++ 23 (clang 16.0.6) 语言条件下测试
cmd:        clang++ -std=c++2b -Wall -Wextra -O2 -DONLINE_JUDGE -DATCODER -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647 -I/opt/boost/clang/include -L/opt/boost/clang/lib -I/opt/ac-library -I/usr/include/eigen3 -fuse-ld=lld
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
*/
#include <bitset>
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <random>

#include "DS/DynamicBitset.h"
#include "DS/StaticBitset.h"

using std::bitset;
using std::cout;
template <uint32_t N>
using StaticBitset = OY::StaticBitset<N>;
using DynamicBitset = OY::DynamicBitset;

namespace Bench {
    struct Result {
        std::string type_name;
        std::string bench_name;
        size_t value;
        size_t repeat;
        long long time_cost;
    };
    template <typename... Args>
    void handle(Result res1, Args... args) {
        std::array<Result, sizeof...(Args) + 1> res{res1, args...};
        for (int i = 1; i < res.size(); i++)
            if (res1.value != res[i].value) {
                cout << "wrong value!" << res1.bench_name << '\n';
                cout << "res[0].value = " << res1.value << '\n';
                cout << "res[" << i << "].value = " << res[i].value << '\n';
            }
        cout << int(res1.time_cost) << "\t\t";
        for (int i = 1; i < res.size(); i++) {
            cout << int(res[i].time_cost) << "\t";
            int x = round(100.0 * res[i].time_cost / res1.time_cost);
            if (x < 1000) cout << ' ';
            if (x < 100) cout << ' ';
            if (x < 10) cout << ' ';
            cout << x;
            cout << " %\t\t";
        }
        cout << "\t\t" << res1.bench_name << '\n';
    }
    template <size_t Size>
    void test_for_types(auto call) {
        handle(call(std::bitset<Size>{}), call(StaticBitset<Size>{}), call(DynamicBitset{Size}));
    }
    template <typename T>
    struct is_stl : std::false_type {};
    template <auto Size>
    struct is_stl<std::bitset<Size>> : std::true_type {};
    template <size_t Size>
    std::string type_name(const bitset<Size> &x) {
        return "bitset";
    }
    template <uint32_t Size>
    std::string type_name(const StaticBitset<Size> &x) {
        return "StaticBitset";
    }
    std::string type_name(const DynamicBitset &x) {
        return "DynamicBitset";
    }
    template <size_t Size>
    std::mt19937 &get_rng(const bitset<Size> &x) {
        static std::mt19937 s_rng;
        return s_rng;
    }
    template <uint32_t Size>
    std::mt19937 &get_rng(const StaticBitset<Size> &x) {
        static std::mt19937 s_rng;
        return s_rng;
    }
    std::mt19937 &get_rng(const DynamicBitset &x) {
        static std::map<uint32_t, std::mt19937> s_rng;
        return s_rng[x.size()];
    }
    namespace VisitOne {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value)
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                else
                    for (auto i = B.first_one(); ~i; i = B.next_one(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace ReverseVisitOne {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value)
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                else
                    for (auto i = B.last_one(); ~i; i = B.prev_one(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace TraverseOne {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value)
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                else
                    B.enumerate_one([&](auto i) { value += i; });
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace VisitZero {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value) {
                    B.flip();
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                    B.flip();
                } else
                    for (auto i = B.first_zero(); ~i; i = B.next_zero(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace ReverseVisitZero {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value) {
                    B.flip();
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                    B.flip();
                } else
                    for (auto i = B.last_zero(); ~i; i = B.prev_zero(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace TraverseZero {
        template <size_t OneCount, uint32_t Repeat>
        Result bench(auto &&B, std::string name) {
            for (uint32_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value) {
                    B.flip();
                    for (auto i = B._Find_first(); i != B.size(); i = B._Find_next(i)) value = value + i;
                    B.flip();
                } else
                    B.enumerate_zero([&](auto i) { value += i; });
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat>(x, bench_name); });
        }
    }
    namespace General {
        template <size_t OneCount, size_t Repeat, typename Callback>
        Result bench(auto &&B, std::string name, Callback call) {
            for (size_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            volatile size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) call(value, B);
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size, typename Callback>
        void run(std::string bench_name, Callback call) {
            test_for_types<Size>([bench_name, call](auto &&x) { return bench<OneCount, Repeat>(x, bench_name, call); });
        }
    }
    namespace Count {
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            General::run<OneCount, Repeat, Size>(bench_name, [](auto &value, auto &&B) { value = value + B.count(); });
        }
    }
    namespace Set {
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            General::run<OneCount, Repeat, Size>(bench_name, [](auto &value, auto &&B) {
                B.set();
                value = value + B[0] + B[1];
            });
        }
    }
    namespace Reset {
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            General::run<OneCount, Repeat, Size>(bench_name, [](auto &value, auto &&B) {
                B.reset();
                value = value + B[0] + B[1];
            });
        }
    }
    namespace Flip {
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            General::run<OneCount, Repeat, Size>(bench_name, [](auto &value, auto &&B) {
                B.flip();
                value = value + B[0] + B[1];
            });
        }
    }
    namespace LeftShift {
        template <size_t OneCount, size_t Repeat, size_t X>
        Result bench(auto &&B, std::string name) {
            for (size_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            volatile size_t value = 1;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value)
                    value = value + (B << X)._Find_first();
                else
                    value = value + (B << X).first_one();
                if (value) B.flip(X);
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t X, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat, X>(x, bench_name); });
        }
    }
    namespace RightShift {
        template <size_t OneCount, size_t Repeat, size_t X>
        Result bench(auto &&B, std::string name) {
            for (size_t i = 0; i != B.size(); i++)
                if (get_rng(B)() % uint32_t(B.size()) < OneCount) B.set(i);
            volatile size_t value = 1;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                if constexpr (is_stl<std::decay_t<decltype(B)>>::value)
                    value = value + (B >> X)._Find_first();
                else
                    value = value + (B >> X).first_one();
                if (value) B.flip(X);
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t X, size_t Size>
        void run(std::string bench_name) {
            test_for_types<Size>([bench_name](auto &&x) { return bench<OneCount, Repeat, X>(x, bench_name); });
        }
    }
}

#define RUN(...) __VA_ARGS__(#__VA_ARGS__);
int main() {
    cout << "bitset\t\tStaticBitset\t\tDynamicBitset\n";
    RUN(Bench::VisitOne::run<10, 1000000, 64>);
    RUN(Bench::VisitOne::run<50, 200000, 64>);
    RUN(Bench::VisitOne::run<1000, 5000, 100000>);
    RUN(Bench::VisitOne::run<99000, 50, 100000>);

    RUN(Bench::ReverseVisitOne::run<10, 1000000, 64>);
    RUN(Bench::ReverseVisitOne::run<50, 200000, 64>);
    RUN(Bench::ReverseVisitOne::run<1000, 5000, 100000>);
    RUN(Bench::ReverseVisitOne::run<99000, 50, 100000>);

    RUN(Bench::TraverseOne::run<10, 1000000, 64>);
    RUN(Bench::TraverseOne::run<50, 200000, 64>);
    RUN(Bench::TraverseOne::run<1000, 10000, 100000>);
    RUN(Bench::TraverseOne::run<99000, 100, 100000>);

    RUN(Bench::VisitZero::run<10, 200000, 64>);
    RUN(Bench::VisitZero::run<50, 1000000, 64>);
    RUN(Bench::VisitZero::run<1000, 50, 100000>);
    RUN(Bench::VisitZero::run<99000, 5000, 100000>);

    RUN(Bench::ReverseVisitZero::run<10, 200000, 64>);
    RUN(Bench::ReverseVisitZero::run<50, 1000000, 64>);
    RUN(Bench::ReverseVisitZero::run<1000, 50, 100000>);
    RUN(Bench::ReverseVisitZero::run<99000, 5000, 100000>);

    RUN(Bench::TraverseZero::run<10, 200000, 64>);
    RUN(Bench::TraverseZero::run<50, 1000000, 64>);
    RUN(Bench::TraverseZero::run<1000, 100, 100000>);
    RUN(Bench::TraverseZero::run<99000, 10000, 100000>);

    RUN(Bench::Count::run<10, 10000000, 64>);
    RUN(Bench::Count::run<50, 10000000, 64>);
    RUN(Bench::Count::run<1000, 100000, 100000>);
    RUN(Bench::Count::run<99000, 100000, 100000>);

    RUN(Bench::Set::run<10, 10000000, 64>);
    RUN(Bench::Set::run<50, 10000000, 64>);
    RUN(Bench::Set::run<1000, 100000, 100000>);
    RUN(Bench::Set::run<99000, 100000, 100000>);

    RUN(Bench::Reset::run<10, 10000000, 64>);
    RUN(Bench::Reset::run<50, 10000000, 64>);
    RUN(Bench::Reset::run<1000, 200000, 100000>);
    RUN(Bench::Reset::run<99000, 200000, 100000>);

    RUN(Bench::Flip::run<10, 10000000, 64>);
    RUN(Bench::Flip::run<50, 10000000, 64>);
    RUN(Bench::Flip::run<1000, 100000, 100000>);
    RUN(Bench::Flip::run<99000, 100000, 100000>);

    RUN(Bench::LeftShift::run<10, 5000000, 1, 64>);
    RUN(Bench::LeftShift::run<50, 5000000, 1, 64>);

    RUN(Bench::LeftShift::run<1000, 100000, 1, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 1, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 32, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 32, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 1000, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 1000, 100000>);

    RUN(Bench::LeftShift::run<1000, 50000, 64000, 100000>);
    RUN(Bench::LeftShift::run<99000, 50000, 64000, 100000>);

    RUN(Bench::RightShift::run<10, 5000000, 1, 64>);
    RUN(Bench::RightShift::run<50, 5000000, 1, 64>);

    RUN(Bench::RightShift::run<1000, 100000, 1, 100000>);
    RUN(Bench::RightShift::run<99000, 100000, 1, 100000>);

    RUN(Bench::RightShift::run<1000, 100000, 32, 100000>);
    RUN(Bench::RightShift::run<99000, 100000, 32, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 1000, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 1000, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 64000, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 64000, 100000>);
}
/*
atcoder g++
bitset		StaticBitset		DynamicBitset
8		8	 100 %		18	 225 %				Bench::VisitOne::run<10, 1000000, 64>
16		17	 106 %		33	 206 %				Bench::VisitOne::run<50, 200000, 64>
20		21	 105 %		20	 100 %				Bench::VisitOne::run<1000, 5000, 100000>
18		18	 100 %		18	 100 %				Bench::VisitOne::run<99000, 50, 100000>
10		8	  80 %		19	 190 %				Bench::ReverseVisitOne::run<10, 1000000, 64>
15		12	  80 %		31	 207 %				Bench::ReverseVisitOne::run<50, 200000, 64>
20		20	 100 %		20	 100 %				Bench::ReverseVisitOne::run<1000, 5000, 100000>
18		17	  94 %		18	 100 %				Bench::ReverseVisitOne::run<99000, 50, 100000>
10		6	  60 %		7	  70 %				Bench::TraverseOne::run<10, 1000000, 64>
15		11	  73 %		12	  80 %				Bench::TraverseOne::run<50, 200000, 64>
41		15	  37 %		12	  29 %				Bench::TraverseOne::run<1000, 10000, 100000>
37		15	  41 %		14	  38 %				Bench::TraverseOne::run<99000, 100, 100000>
20		19	  95 %		47	 235 %				Bench::VisitZero::run<10, 200000, 64>
15		15	 100 %		45	 300 %				Bench::VisitZero::run<50, 1000000, 64>
18		18	 100 %		20	 111 %				Bench::VisitZero::run<1000, 50, 100000>
25		23	  92 %		20	  80 %				Bench::VisitZero::run<99000, 5000, 100000>
19		22	 116 %		43	 226 %				Bench::ReverseVisitZero::run<10, 200000, 64>
18		22	 122 %		46	 256 %				Bench::ReverseVisitZero::run<50, 1000000, 64>
18		19	 106 %		19	 106 %				Bench::ReverseVisitZero::run<1000, 50, 100000>
26		23	  88 %		22	  85 %				Bench::ReverseVisitZero::run<99000, 5000, 100000>
17		12	  71 %		13	  76 %				Bench::TraverseZero::run<10, 200000, 64>
21		14	  67 %		18	  86 %				Bench::TraverseZero::run<50, 1000000, 64>
37		15	  41 %		15	  41 %				Bench::TraverseZero::run<1000, 100, 100000>
54		15	  28 %		18	  33 %				Bench::TraverseZero::run<99000, 10000, 100000>
21		21	 100 %		21	 100 %				Bench::Count::run<10, 10000000, 64>
21		21	 100 %		18	  86 %				Bench::Count::run<50, 10000000, 64>
80		80	 100 %		81	 101 %				Bench::Count::run<1000, 100000, 100000>
80		80	 100 %		81	 101 %				Bench::Count::run<99000, 100000, 100000>
21		21	 100 %		94	 448 %				Bench::Set::run<10, 10000000, 64>
21		21	 100 %		94	 448 %				Bench::Set::run<50, 10000000, 64>
48		6	  13 %		6	  13 %				Bench::Set::run<1000, 100000, 100000>
48		6	  13 %		6	  13 %				Bench::Set::run<99000, 100000, 100000>
14		14	 100 %		71	 507 %				Bench::Reset::run<10, 10000000, 64>
14		14	 100 %		71	 507 %				Bench::Reset::run<50, 10000000, 64>
12		12	 100 %		12	 100 %				Bench::Reset::run<1000, 200000, 100000>
12		12	 100 %		12	 100 %				Bench::Reset::run<99000, 200000, 100000>
22		21	  95 %		36	 164 %				Bench::Flip::run<10, 10000000, 64>
21		21	 100 %		36	 171 %				Bench::Flip::run<50, 10000000, 64>
71		71	 100 %		71	 100 %				Bench::Flip::run<1000, 100000, 100000>
90		90	 100 %		71	  79 %				Bench::Flip::run<99000, 100000, 100000>
10		10	 100 %		81	 810 %				Bench::LeftShift::run<10, 5000000, 1, 64>
10		10	 100 %		77	 770 %				Bench::LeftShift::run<50, 5000000, 1, 64>
82		75	  91 %		110	 134 %				Bench::LeftShift::run<1000, 100000, 1, 100000>
81		74	  91 %		123	 152 %				Bench::LeftShift::run<99000, 100000, 1, 100000>
83		75	  90 %		96	 116 %				Bench::LeftShift::run<1000, 100000, 32, 100000>
82		74	  90 %		95	 116 %				Bench::LeftShift::run<99000, 100000, 32, 100000>
116		97	  84 %		118	 102 %				Bench::LeftShift::run<1000, 100000, 1000, 100000>
116		108	  93 %		118	 102 %				Bench::LeftShift::run<99000, 100000, 1000, 100000>
25		29	 116 %		44	 176 %				Bench::LeftShift::run<1000, 50000, 64000, 100000>
25		35	 140 %		44	 176 %				Bench::LeftShift::run<99000, 50000, 64000, 100000>
5		10	 200 %		76	1520 %				Bench::RightShift::run<10, 5000000, 1, 64>
4		4	 100 %		77	1925 %				Bench::RightShift::run<50, 5000000, 1, 64>
59		53	  90 %		109	 185 %				Bench::RightShift::run<1000, 100000, 1, 100000>
59		54	  92 %		109	 185 %				Bench::RightShift::run<99000, 100000, 1, 100000>
59		54	  92 %		109	 185 %				Bench::RightShift::run<1000, 100000, 32, 100000>
59		54	  92 %		109	 185 %				Bench::RightShift::run<99000, 100000, 32, 100000>
209		193	  92 %		218	 104 %				Bench::RightShift::run<1000, 200000, 1000, 100000>
208		192	  92 %		217	 104 %				Bench::RightShift::run<99000, 200000, 1000, 100000>
41		28	  68 %		53	 129 %				Bench::RightShift::run<1000, 200000, 64000, 100000>
41		28	  68 %		53	 129 %				Bench::RightShift::run<99000, 200000, 64000, 100000>

*/
/*
atcoder clang++
bitset		StaticBitset		DynamicBitset
9		5	  56 %		19	 211 %				Bench::VisitOne::run<10, 1000000, 64>
16		11	  69 %		35	 219 %				Bench::VisitOne::run<50, 200000, 64>
19		21	 111 %		21	 111 %				Bench::VisitOne::run<1000, 5000, 100000>
18		19	 106 %		19	 106 %				Bench::VisitOne::run<99000, 50, 100000>
10		28	 280 %		28	 280 %				Bench::ReverseVisitOne::run<10, 1000000, 64>
15		36	 240 %		36	 240 %				Bench::ReverseVisitOne::run<50, 200000, 64>
19		22	 116 %		22	 116 %				Bench::ReverseVisitOne::run<1000, 5000, 100000>
19		19	 100 %		19	 100 %				Bench::ReverseVisitOne::run<99000, 50, 100000>
10		6	  60 %		7	  70 %				Bench::TraverseOne::run<10, 1000000, 64>
15		11	  73 %		12	  80 %				Bench::TraverseOne::run<50, 200000, 64>
38		14	  37 %		13	  34 %				Bench::TraverseOne::run<1000, 10000, 100000>
38		14	  37 %		14	  37 %				Bench::TraverseOne::run<99000, 100, 100000>
18		13	  72 %		32	 178 %				Bench::VisitZero::run<10, 200000, 64>
15		9	  60 %		38	 253 %				Bench::VisitZero::run<50, 1000000, 64>
19		21	 111 %		21	 111 %				Bench::VisitZero::run<1000, 50, 100000>
18		21	 117 %		21	 117 %				Bench::VisitZero::run<99000, 5000, 100000>
19		45	 237 %		46	 242 %				Bench::ReverseVisitZero::run<10, 200000, 64>
18		53	 294 %		52	 289 %				Bench::ReverseVisitZero::run<50, 1000000, 64>
18		19	 106 %		19	 106 %				Bench::ReverseVisitZero::run<1000, 50, 100000>
19		22	 116 %		22	 116 %				Bench::ReverseVisitZero::run<99000, 5000, 100000>
17		12	  71 %		13	  76 %				Bench::TraverseZero::run<10, 200000, 64>
21		14	  67 %		15	  71 %				Bench::TraverseZero::run<50, 1000000, 64>
39		15	  38 %		15	  38 %				Bench::TraverseZero::run<1000, 100, 100000>
40		17	  43 %		17	  43 %				Bench::TraverseZero::run<99000, 10000, 100000>
21		21	 100 %		22	 105 %				Bench::Count::run<10, 10000000, 64>
21		21	 100 %		22	 105 %				Bench::Count::run<50, 10000000, 64>
11		22	 200 %		22	 200 %				Bench::Count::run<1000, 100000, 100000>
11		22	 200 %		22	 200 %				Bench::Count::run<99000, 100000, 100000>
21		21	 100 %		91	 433 %				Bench::Set::run<10, 10000000, 64>
21		21	 100 %		91	 433 %				Bench::Set::run<50, 10000000, 64>
6		6	 100 %		6	 100 %				Bench::Set::run<1000, 100000, 100000>
6		6	 100 %		6	 100 %				Bench::Set::run<99000, 100000, 100000>
14		14	 100 %		71	 507 %				Bench::Reset::run<10, 10000000, 64>
14		14	 100 %		71	 507 %				Bench::Reset::run<50, 10000000, 64>
12		12	 100 %		12	 100 %				Bench::Reset::run<1000, 200000, 100000>
12		12	 100 %		12	 100 %				Bench::Reset::run<99000, 200000, 100000>
22		22	 100 %		42	 191 %				Bench::Flip::run<10, 10000000, 64>
21		22	 105 %		35	 167 %				Bench::Flip::run<50, 10000000, 64>
18		18	 100 %		9	  50 %				Bench::Flip::run<1000, 100000, 100000>
18		18	 100 %		9	  50 %				Bench::Flip::run<99000, 100000, 100000>
9		10	 111 %		77	 856 %				Bench::LeftShift::run<10, 5000000, 1, 64>
9		9	 100 %		76	 844 %				Bench::LeftShift::run<50, 5000000, 1, 64>
71		53	  75 %		53	  75 %				Bench::LeftShift::run<1000, 100000, 1, 100000>
69		52	  75 %		53	  77 %				Bench::LeftShift::run<99000, 100000, 1, 100000>
71		53	  75 %		38	  54 %				Bench::LeftShift::run<1000, 100000, 32, 100000>
69		52	  75 %		38	  55 %				Bench::LeftShift::run<99000, 100000, 32, 100000>
57		68	 119 %		43	  75 %				Bench::LeftShift::run<1000, 100000, 1000, 100000>
57		68	 119 %		43	  75 %				Bench::LeftShift::run<99000, 100000, 1000, 100000>
13		12	  92 %		45	 346 %				Bench::LeftShift::run<1000, 50000, 64000, 100000>
13		11	  85 %		45	 346 %				Bench::LeftShift::run<99000, 50000, 64000, 100000>
9		9	 100 %		73	 811 %				Bench::RightShift::run<10, 5000000, 1, 64>
9		9	 100 %		76	 844 %				Bench::RightShift::run<50, 5000000, 1, 64>
44		23	  52 %		36	  82 %				Bench::RightShift::run<1000, 100000, 1, 100000>
44		23	  52 %		36	  82 %				Bench::RightShift::run<99000, 100000, 1, 100000>
43		18	  42 %		31	  72 %				Bench::RightShift::run<1000, 100000, 32, 100000>
43		19	  44 %		31	  72 %				Bench::RightShift::run<99000, 100000, 32, 100000>
92		58	  63 %		90	  98 %				Bench::RightShift::run<1000, 200000, 1000, 100000>
92		58	  63 %		83	  90 %				Bench::RightShift::run<99000, 200000, 1000, 100000>
14		28	 200 %		53	 379 %				Bench::RightShift::run<1000, 200000, 64000, 100000>
14		28	 200 %		53	 379 %				Bench::RightShift::run<99000, 200000, 64000, 100000>

*/