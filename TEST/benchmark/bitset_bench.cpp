/*
本文件比较 std::bitset 与模板库里的 Bitset 效率高低

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
#include <bitset>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "DS/Bitset.h"

using std::bitset;
using std::cout;
template <uint32_t N>
using Bitset64 = OY::Bitset::Table<N>;
enum SHOW_CONTENT {
    SHOW_TIME = 0,
    SHOW_RATIO = 1,
    SHOW_ALL = 2
};
// 本参数决定展示的内容
static constexpr SHOW_CONTENT show_content = SHOW_ALL;

namespace Bench {
    struct Result {
        std::string type_name;
        std::string bench_name;
        size_t value;
        size_t repeat;
        long long time_cost;
    };
    void handle(Result &res1, Result &res2) {
        if (res1.value != res2.value) {
            cout << "wrong value!" << res1.bench_name << '\n';
            cout << "res1.value = " << res1.value << '\n';
            cout << "res2.value = " << res2.value << '\n';
        }
        if constexpr (show_content == SHOW_TIME)
            cout << int(res1.time_cost) << "\t\t" << int(res2.time_cost) << "\t\t" << res1.bench_name << '\n';
        else if constexpr (show_content == SHOW_RATIO) {
            if (res1.time_cost) {
                int x = round(100.0 * res2.time_cost / res1.time_cost);
                if (x < 1000) cout << ' ';
                if (x < 100) cout << ' ';
                if (x < 10) cout << ' ';
                cout << x;
                cout << " %";
            } else if (!res2.time_cost)
                cout << " 100 %";
            else
                cout << "   inf";
            cout << "\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        } else if constexpr (show_content == SHOW_ALL) {
            cout << int(res1.time_cost) << "\t\t" << int(res2.time_cost) << "\t\t";
            if (res1.time_cost) {
                int x = round(100.0 * res2.time_cost / res1.time_cost);
                if (x < 1000) cout << ' ';
                if (x < 100) cout << ' ';
                if (x < 10) cout << ' ';
                cout << x;
                cout << " %";
            } else if (!res2.time_cost)
                cout << " 100 %";
            else
                cout << "   inf";
            cout << "\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        }
    }
    template <size_t Size>
    uint32_t find_first(const bitset<Size> &x) {
        return x._Find_first();
    }
    template <uint32_t Size>
    uint32_t find_first(const Bitset64<Size> &x) {
        return x.first_one();
    }
    template <size_t Size>
    std::string type_name(const bitset<Size> &x) {
        return "bitset";
    }
    template <uint32_t Size>
    std::string type_name(const Bitset64<Size> &x) {
        return "Bitset64";
    }
    template <size_t Size>
    std::mt19937 &get_rng(const bitset<Size> &x) {
        static std::mt19937 s_rng;
        return s_rng;
    }
    template <uint32_t Size>
    std::mt19937 &get_rng(const Bitset64<Size> &x) {
        static std::mt19937 s_rng;
        return s_rng;
    }
    namespace VisitOne {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B.first_one(); ~i; i = B.next_one(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace ReverseVisitOne {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B.last_one(); ~i; i = B.prev_one(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace TraverseOne {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                B.do_for_each_one([&](auto i) { value = value + i; });
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace VisitZero {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                B.flip();
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
                B.flip();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B.first_zero(); ~i; i = B.next_zero(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace ReverseVisitZero {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                B.flip();
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
                B.flip();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                for (auto i = B.last_zero(); ~i; i = B.prev_zero(i)) value = value + i;
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace TraverseZero {
        template <size_t OneCount, size_t Repeat, size_t Size>
        Result bench(bitset<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                B.flip();
                for (auto i = B._Find_first(); i != Size; i = B._Find_next(i)) value = value + i;
                B.flip();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, uint32_t Repeat, uint32_t Size>
        Result bench(Bitset64<Size> &&B, std::string name) {
            for (uint32_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++)
                B.do_for_each_zero([&](auto i) { value = value + i; });
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace General {
        template <size_t OneCount, size_t Repeat, typename SizeType, SizeType Size, template <SizeType> typename table, typename Callback>
        Result bench(table<Size> &&B, std::string name, Callback call) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            volatile size_t value = 0;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) call(value, B);
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t Size, typename Callback>
        void run(std::string bench_name, Callback call) {
            auto res1 = bench<OneCount, Repeat>(bitset<Size>{}, bench_name, call);
            auto res2 = bench<OneCount, Repeat>(Bitset64<Size>{}, bench_name, call);
            handle(res1, res2);
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
        template <size_t OneCount, size_t Repeat, size_t X, typename SizeType, SizeType Size, template <SizeType> typename table>
        Result bench(table<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            volatile size_t value = 1;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                value = value + find_first<Size>(B << X);
                if (value) B.flip(X);
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t X, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat, X>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat, X>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
    namespace RightShift {
        template <size_t OneCount, size_t Repeat, size_t X, typename SizeType, SizeType Size, template <SizeType> typename table>
        Result bench(table<Size> &&B, std::string name) {
            for (size_t i = 0; i != Size; i++)
                if (get_rng(B)() % Size < OneCount) B.set(i);
            volatile size_t value = 1;
            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t j = 0; j != Repeat; j++) {
                value = value + find_first<Size>(B >> X);
                if (value) B.flip(X);
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Size>(B), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <size_t OneCount, size_t Repeat, size_t X, size_t Size>
        void run(std::string bench_name) {
            auto res1 = bench<OneCount, Repeat, X>(bitset<Size>{}, bench_name);
            auto res2 = bench<OneCount, Repeat, X>(Bitset64<Size>{}, bench_name);
            handle(res1, res2);
        }
    }
}

#define RUN(...) __VA_ARGS__(#__VA_ARGS__);
int main() {
    if constexpr (show_content == SHOW_TIME)
        cout << "bitset\t\tBitset64\n";
    else if constexpr (show_content == SHOW_RATIO)
        cout << "Bitset64 / bitset\n";
    else if constexpr (show_content == SHOW_ALL)
        cout << "bitset\t\tBitset64\t\tratio\n";
    RUN(Bench::VisitOne::run<10, 1000000, 64>);
    RUN(Bench::VisitOne::run<50, 200000, 64>);
    RUN(Bench::VisitOne::run<1000, 10000, 100000>);
    RUN(Bench::VisitOne::run<99000, 100, 100000>);

    RUN(Bench::VisitZero::run<10, 200000, 64>);
    RUN(Bench::VisitZero::run<50, 1000000, 64>);
    RUN(Bench::VisitZero::run<1000, 100, 100000>);
    RUN(Bench::VisitZero::run<99000, 10000, 100000>);

    RUN(Bench::ReverseVisitOne::run<10, 1000000, 64>);
    RUN(Bench::ReverseVisitOne::run<50, 200000, 64>);
    RUN(Bench::ReverseVisitOne::run<1000, 10000, 100000>);
    RUN(Bench::ReverseVisitOne::run<99000, 100, 100000>);

    RUN(Bench::ReverseVisitZero::run<10, 200000, 64>);
    RUN(Bench::ReverseVisitZero::run<50, 1000000, 64>);
    RUN(Bench::ReverseVisitZero::run<1000, 100, 100000>);
    RUN(Bench::ReverseVisitZero::run<99000, 10000, 100000>);

    RUN(Bench::TraverseOne::run<10, 1000000, 64>);
    RUN(Bench::TraverseOne::run<50, 1000000, 64>);
    RUN(Bench::TraverseOne::run<1000, 10000, 100000>);
    RUN(Bench::TraverseOne::run<99000, 100, 100000>);

    RUN(Bench::TraverseZero::run<10, 1000000, 64>);
    RUN(Bench::TraverseZero::run<50, 1000000, 64>);
    RUN(Bench::TraverseZero::run<1000, 100, 100000>);
    RUN(Bench::TraverseZero::run<99000, 10000, 100000>);

    RUN(Bench::Count::run<10, 10000000, 64>);
    RUN(Bench::Count::run<50, 10000000, 64>);
    RUN(Bench::Count::run<1000, 10000, 100000>);
    RUN(Bench::Count::run<99000, 10000, 100000>);

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

    RUN(Bench::LeftShift::run<10, 10000000, 1, 64>);
    RUN(Bench::LeftShift::run<50, 10000000, 1, 64>);

    RUN(Bench::LeftShift::run<10, 10000000, 32, 64>);
    RUN(Bench::LeftShift::run<50, 10000000, 32, 64>);

    RUN(Bench::LeftShift::run<1000, 100000, 1, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 1, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 32, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 32, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 64, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 64, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 1000, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 1000, 100000>);

    RUN(Bench::LeftShift::run<1000, 100000, 96000, 100000>);
    RUN(Bench::LeftShift::run<99000, 100000, 96000, 100000>);

    RUN(Bench::RightShift::run<10, 10000000, 1, 64>);
    RUN(Bench::RightShift::run<50, 10000000, 1, 64>);

    RUN(Bench::RightShift::run<10, 10000000, 32, 64>);
    RUN(Bench::RightShift::run<50, 10000000, 32, 64>);

    RUN(Bench::RightShift::run<1000, 100000, 1, 100000>);
    RUN(Bench::RightShift::run<99000, 100000, 1, 100000>);

    RUN(Bench::RightShift::run<1000, 100000, 32, 100000>);
    RUN(Bench::RightShift::run<99000, 100000, 32, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 64, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 64, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 100, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 100, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 1000, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 1000, 100000>);

    RUN(Bench::RightShift::run<1000, 200000, 96000, 100000>);
    RUN(Bench::RightShift::run<99000, 200000, 96000, 100000>);
}
/*
atcoder g++
bitset		Bitset64		ratio
8		9		 113 %		Bench::VisitOne::run<10, 1000000, 64>		too fast(8)
16		17		 106 %		Bench::VisitOne::run<50, 200000, 64>
41		40		  98 %		Bench::VisitOne::run<1000, 10000, 100000>
37		36		  97 %		Bench::VisitOne::run<99000, 100, 100000>
17		23		 135 %		Bench::VisitZero::run<10, 200000, 64>
16		25		 156 %		Bench::VisitZero::run<50, 1000000, 64>
37		38		 103 %		Bench::VisitZero::run<1000, 100, 100000>
54		43		  80 %		Bench::VisitZero::run<99000, 10000, 100000>
10		7		  70 %		Bench::ReverseVisitOne::run<10, 1000000, 64>
15		12		  80 %		Bench::ReverseVisitOne::run<50, 200000, 64>
41		40		  98 %		Bench::ReverseVisitOne::run<1000, 10000, 100000>
37		35		  95 %		Bench::ReverseVisitOne::run<99000, 100, 100000>
18		21		 117 %		Bench::ReverseVisitZero::run<10, 200000, 64>
15		18		 120 %		Bench::ReverseVisitZero::run<50, 1000000, 64>
37		38		 103 %		Bench::ReverseVisitZero::run<1000, 100, 100000>
50		39		  78 %		Bench::ReverseVisitZero::run<99000, 10000, 100000>
3		2		  67 %		Bench::TraverseOne::run<10, 1000000, 64>		too fast(3)
75		60		  80 %		Bench::TraverseOne::run<50, 1000000, 64>
38		17		  45 %		Bench::TraverseOne::run<1000, 10000, 100000>
37		14		  38 %		Bench::TraverseOne::run<99000, 100, 100000>
86		69		  80 %		Bench::TraverseZero::run<10, 1000000, 64>
21		16		  76 %		Bench::TraverseZero::run<50, 1000000, 64>
37		15		  41 %		Bench::TraverseZero::run<1000, 100, 100000>
54		18		  33 %		Bench::TraverseZero::run<99000, 10000, 100000>
21		21		 100 %		Bench::Count::run<10, 10000000, 64>
21		21		 100 %		Bench::Count::run<50, 10000000, 64>
8		8		 100 %		Bench::Count::run<1000, 10000, 100000>		too fast(8)
9		8		  89 %		Bench::Count::run<99000, 10000, 100000>		too fast(9)
17		21		 124 %		Bench::Set::run<10, 10000000, 64>
21		20		  95 %		Bench::Set::run<50, 10000000, 64>
90		7		   8 %		Bench::Set::run<1000, 100000, 100000>
48		7		  15 %		Bench::Set::run<99000, 100000, 100000>
14		14		 100 %		Bench::Reset::run<10, 10000000, 64>
14		15		 107 %		Bench::Reset::run<50, 10000000, 64>
12		14		 117 %		Bench::Reset::run<1000, 200000, 100000>
12		14		 117 %		Bench::Reset::run<99000, 200000, 100000>
21		21		 100 %		Bench::Flip::run<10, 10000000, 64>
21		21		 100 %		Bench::Flip::run<50, 10000000, 64>
90		72		  80 %		Bench::Flip::run<1000, 100000, 100000>
71		72		 101 %		Bench::Flip::run<99000, 100000, 100000>
20		11		  55 %		Bench::LeftShift::run<10, 10000000, 1, 64>
20		13		  65 %		Bench::LeftShift::run<50, 10000000, 1, 64>
8		20		 250 %		Bench::LeftShift::run<10, 10000000, 32, 64>		too fast(8)
8		20		 250 %		Bench::LeftShift::run<50, 10000000, 32, 64>		too fast(8)
82		99		 121 %		Bench::LeftShift::run<1000, 100000, 1, 100000>
81		98		 121 %		Bench::LeftShift::run<99000, 100000, 1, 100000>
82		84		 102 %		Bench::LeftShift::run<1000, 100000, 32, 100000>
81		84		 104 %		Bench::LeftShift::run<99000, 100000, 32, 100000>
24		19		  79 %		Bench::LeftShift::run<1000, 100000, 64, 100000>
24		19		  79 %		Bench::LeftShift::run<99000, 100000, 64, 100000>
115		124		 108 %		Bench::LeftShift::run<1000, 100000, 1000, 100000>
115		123		 107 %		Bench::LeftShift::run<99000, 100000, 1000, 100000>
63		103		 163 %		Bench::LeftShift::run<1000, 100000, 96000, 100000>
63		85		 135 %		Bench::LeftShift::run<99000, 100000, 96000, 100000>
20		16		  80 %		Bench::RightShift::run<10, 10000000, 1, 64>
20		18		  90 %		Bench::RightShift::run<50, 10000000, 1, 64>
10		20		 200 %		Bench::RightShift::run<10, 10000000, 32, 64>
8		14		 175 %		Bench::RightShift::run<50, 10000000, 32, 64>		too fast(8)
60		97		 162 %		Bench::RightShift::run<1000, 100000, 1, 100000>
61		97		 159 %		Bench::RightShift::run<99000, 100000, 1, 100000>
60		113		 188 %		Bench::RightShift::run<1000, 100000, 32, 100000>
60		113		 188 %		Bench::RightShift::run<99000, 100000, 32, 100000>
737		724		  98 %		Bench::RightShift::run<1000, 200000, 64, 100000>
737		724		  98 %		Bench::RightShift::run<99000, 200000, 64, 100000>
208		195		  94 %		Bench::RightShift::run<1000, 200000, 100, 100000>
207		227		 110 %		Bench::RightShift::run<99000, 200000, 100, 100000>
205		193		  94 %		Bench::RightShift::run<1000, 200000, 1000, 100000>
206		192		  93 %		Bench::RightShift::run<99000, 200000, 1000, 100000>
39		28		  72 %		Bench::RightShift::run<1000, 200000, 96000, 100000>
40		28		  70 %		Bench::RightShift::run<99000, 200000, 96000, 100000>

*/
/*
atcoder clang++
bitset		Bitset64		ratio
8		9		 113 %		Bench::VisitOne::run<10, 1000000, 64>		too fast(8)
16		16		 100 %		Bench::VisitOne::run<50, 200000, 64>
38		42		 111 %		Bench::VisitOne::run<1000, 10000, 100000>
37		38		 103 %		Bench::VisitOne::run<99000, 100, 100000>
17		19		 112 %		Bench::VisitZero::run<10, 200000, 64>
16		18		 113 %		Bench::VisitZero::run<50, 1000000, 64>
38		43		 113 %		Bench::VisitZero::run<1000, 100, 100000>
40		47		 118 %		Bench::VisitZero::run<99000, 10000, 100000>
10		28		 280 %		Bench::ReverseVisitOne::run<10, 1000000, 64>
15		36		 240 %		Bench::ReverseVisitOne::run<50, 200000, 64>
39		44		 113 %		Bench::ReverseVisitOne::run<1000, 10000, 100000>
39		39		 100 %		Bench::ReverseVisitOne::run<99000, 100, 100000>
18		42		 233 %		Bench::ReverseVisitZero::run<10, 200000, 64>
15		41		 273 %		Bench::ReverseVisitZero::run<50, 1000000, 64>
39		39		 100 %		Bench::ReverseVisitZero::run<1000, 100, 100000>
37		42		 114 %		Bench::ReverseVisitZero::run<99000, 10000, 100000>
2		3		 150 %		Bench::TraverseOne::run<10, 1000000, 64>		too fast(2)
75		59		  79 %		Bench::TraverseOne::run<50, 1000000, 64>
36		11		  31 %		Bench::TraverseOne::run<1000, 10000, 100000>
37		14		  38 %		Bench::TraverseOne::run<99000, 100, 100000>
86		68		  79 %		Bench::TraverseZero::run<10, 1000000, 64>
21		15		  71 %		Bench::TraverseZero::run<50, 1000000, 64>
39		15		  38 %		Bench::TraverseZero::run<1000, 100, 100000>
40		13		  33 %		Bench::TraverseZero::run<99000, 10000, 100000>
21		21		 100 %		Bench::Count::run<10, 10000000, 64>
21		21		 100 %		Bench::Count::run<50, 10000000, 64>
1		2		 200 %		Bench::Count::run<1000, 10000, 100000>		too fast(1)
1		2		 200 %		Bench::Count::run<99000, 10000, 100000>		too fast(1)
21		12		  57 %		Bench::Set::run<10, 10000000, 64>
21		11		  52 %		Bench::Set::run<50, 10000000, 64>
6		7		 117 %		Bench::Set::run<1000, 100000, 100000>		too fast(6)
6		7		 117 %		Bench::Set::run<99000, 100000, 100000>		too fast(6)
14		14		 100 %		Bench::Reset::run<10, 10000000, 64>
14		14		 100 %		Bench::Reset::run<50, 10000000, 64>
12		14		 117 %		Bench::Reset::run<1000, 200000, 100000>
12		14		 117 %		Bench::Reset::run<99000, 200000, 100000>
22		35		 159 %		Bench::Flip::run<10, 10000000, 64>
22		42		 191 %		Bench::Flip::run<50, 10000000, 64>
18		18		 100 %		Bench::Flip::run<1000, 100000, 100000>
18		18		 100 %		Bench::Flip::run<99000, 100000, 100000>
19		150		 789 %		Bench::LeftShift::run<10, 10000000, 1, 64>
19		150		 789 %		Bench::LeftShift::run<50, 10000000, 1, 64>
18		149		 828 %		Bench::LeftShift::run<10, 10000000, 32, 64>
18		149		 828 %		Bench::LeftShift::run<50, 10000000, 32, 64>
70		42		  60 %		Bench::LeftShift::run<1000, 100000, 1, 100000>
69		41		  59 %		Bench::LeftShift::run<99000, 100000, 1, 100000>
71		27		  38 %		Bench::LeftShift::run<1000, 100000, 32, 100000>
69		27		  39 %		Bench::LeftShift::run<99000, 100000, 32, 100000>
24		26		 108 %		Bench::LeftShift::run<1000, 100000, 64, 100000>
24		26		 108 %		Bench::LeftShift::run<99000, 100000, 64, 100000>
58		31		  53 %		Bench::LeftShift::run<1000, 100000, 1000, 100000>
57		31		  54 %		Bench::LeftShift::run<99000, 100000, 1000, 100000>
36		77		 214 %		Bench::LeftShift::run<1000, 100000, 96000, 100000>
36		103		 286 %		Bench::LeftShift::run<99000, 100000, 96000, 100000>
18		153		 850 %		Bench::RightShift::run<10, 10000000, 1, 64>
18		153		 850 %		Bench::RightShift::run<50, 10000000, 1, 64>
18		152		 844 %		Bench::RightShift::run<10, 10000000, 32, 64>
18		152		 844 %		Bench::RightShift::run<50, 10000000, 32, 64>
44		27		  61 %		Bench::RightShift::run<1000, 100000, 1, 100000>
44		27		  61 %		Bench::RightShift::run<99000, 100000, 1, 100000>
42		26		  62 %		Bench::RightShift::run<1000, 100000, 32, 100000>
42		26		  62 %		Bench::RightShift::run<99000, 100000, 32, 100000>
737		724		  98 %		Bench::RightShift::run<1000, 200000, 64, 100000>
737		724		  98 %		Bench::RightShift::run<99000, 200000, 64, 100000>
88		54		  61 %		Bench::RightShift::run<1000, 200000, 100, 100000>
88		54		  61 %		Bench::RightShift::run<99000, 200000, 100, 100000>
93		61		  66 %		Bench::RightShift::run<1000, 200000, 1000, 100000>
93		60		  65 %		Bench::RightShift::run<99000, 200000, 1000, 100000>
13		28		 215 %		Bench::RightShift::run<1000, 200000, 96000, 100000>
13		28		 215 %		Bench::RightShift::run<99000, 200000, 96000, 100000>

*/
/*
luogu g++
bitset		Bitset64		ratio
8		8		 100 %		Bench::VisitOne::run<10, 1000000, 64>		too fast(8)
17		20		 118 %		Bench::VisitOne::run<50, 200000, 64>
38		40		 105 %		Bench::VisitOne::run<1000, 10000, 100000>
33		33		 100 %		Bench::VisitOne::run<99000, 100, 100000>
18		25		 139 %		Bench::VisitZero::run<10, 200000, 64>
14		20		 143 %		Bench::VisitZero::run<50, 1000000, 64>
33		34		 103 %		Bench::VisitZero::run<1000, 100, 100000>
46		40		  87 %		Bench::VisitZero::run<99000, 10000, 100000>
8		10		 125 %		Bench::ReverseVisitOne::run<10, 1000000, 64>		too fast(8)
17		19		 112 %		Bench::ReverseVisitOne::run<50, 200000, 64>
38		37		  97 %		Bench::ReverseVisitOne::run<1000, 10000, 100000>
33		32		  97 %		Bench::ReverseVisitOne::run<99000, 100, 100000>
18		29		 161 %		Bench::ReverseVisitZero::run<10, 200000, 64>
15		26		 173 %		Bench::ReverseVisitZero::run<50, 1000000, 64>
33		35		 106 %		Bench::ReverseVisitZero::run<1000, 100, 100000>
47		41		  87 %		Bench::ReverseVisitZero::run<99000, 10000, 100000>
12		9		  75 %		Bench::TraverseOne::run<10, 1000000, 64>
89		83		  93 %		Bench::TraverseOne::run<50, 1000000, 64>
38		13		  34 %		Bench::TraverseOne::run<1000, 10000, 100000>
33		15		  45 %		Bench::TraverseOne::run<99000, 100, 100000>
85		79		  93 %		Bench::TraverseZero::run<10, 1000000, 64>
19		15		  79 %		Bench::TraverseZero::run<50, 1000000, 64>
34		14		  41 %		Bench::TraverseZero::run<1000, 100, 100000>
42		14		  33 %		Bench::TraverseZero::run<99000, 10000, 100000>
14		14		 100 %		Bench::Count::run<10, 10000000, 64>
14		14		 100 %		Bench::Count::run<50, 10000000, 64>
6		6		 100 %		Bench::Count::run<1000, 10000, 100000>		too fast(6)
6		6		 100 %		Bench::Count::run<99000, 10000, 100000>		too fast(6)
14		14		 100 %		Bench::Set::run<10, 10000000, 64>
14		14		 100 %		Bench::Set::run<50, 10000000, 64>
6		7		 117 %		Bench::Set::run<1000, 100000, 100000>		too fast(6)
6		7		 117 %		Bench::Set::run<99000, 100000, 100000>		too fast(6)
11		11		 100 %		Bench::Reset::run<10, 10000000, 64>
11		11		 100 %		Bench::Reset::run<50, 10000000, 64>
12		14		 117 %		Bench::Reset::run<1000, 200000, 100000>
12		14		 117 %		Bench::Reset::run<99000, 200000, 100000>
15		13		  87 %		Bench::Flip::run<10, 10000000, 64>
15		14		  93 %		Bench::Flip::run<50, 10000000, 64>
42		42		 100 %		Bench::Flip::run<1000, 100000, 100000>
42		42		 100 %		Bench::Flip::run<99000, 100000, 100000>
15		16		 107 %		Bench::LeftShift::run<10, 10000000, 1, 64>
15		16		 107 %		Bench::LeftShift::run<50, 10000000, 1, 64>
15		16		 107 %		Bench::LeftShift::run<10, 10000000, 32, 64>
15		16		 107 %		Bench::LeftShift::run<50, 10000000, 32, 64>
99		75		  76 %		Bench::LeftShift::run<1000, 100000, 1, 100000>
96		93		  97 %		Bench::LeftShift::run<99000, 100000, 1, 100000>
94		92		  98 %		Bench::LeftShift::run<1000, 100000, 32, 100000>
94		92		  98 %		Bench::LeftShift::run<99000, 100000, 32, 100000>
42		21		  50 %		Bench::LeftShift::run<1000, 100000, 64, 100000>
42		21		  50 %		Bench::LeftShift::run<99000, 100000, 64, 100000>
136		102		  75 %		Bench::LeftShift::run<1000, 100000, 1000, 100000>
136		102		  75 %		Bench::LeftShift::run<99000, 100000, 1000, 100000>
76		74		  97 %		Bench::LeftShift::run<1000, 100000, 96000, 100000>
76		74		  97 %		Bench::LeftShift::run<99000, 100000, 96000, 100000>
15		14		  93 %		Bench::RightShift::run<10, 10000000, 1, 64>
15		14		  93 %		Bench::RightShift::run<50, 10000000, 1, 64>
15		14		  93 %		Bench::RightShift::run<10, 10000000, 32, 64>
15		14		  93 %		Bench::RightShift::run<50, 10000000, 32, 64>
80		106		 133 %		Bench::RightShift::run<1000, 100000, 1, 100000>
72		106		 147 %		Bench::RightShift::run<99000, 100000, 1, 100000>
73		106		 145 %		Bench::RightShift::run<1000, 100000, 32, 100000>
74		106		 143 %		Bench::RightShift::run<99000, 100000, 32, 100000>
88		33		  38 %		Bench::RightShift::run<1000, 200000, 64, 100000>
77		34		  44 %		Bench::RightShift::run<99000, 200000, 64, 100000>
250		211		  84 %		Bench::RightShift::run<1000, 200000, 100, 100000>
258		237		  92 %		Bench::RightShift::run<99000, 200000, 100, 100000>
256		212		  83 %		Bench::RightShift::run<1000, 200000, 1000, 100000>
250		210		  84 %		Bench::RightShift::run<99000, 200000, 1000, 100000>
73		29		  40 %		Bench::RightShift::run<1000, 200000, 96000, 100000>
64		29		  45 %		Bench::RightShift::run<99000, 200000, 96000, 100000>

*/