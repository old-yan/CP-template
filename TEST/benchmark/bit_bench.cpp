/*
本文件比较 C++20 引入的 <bit> 库的函数与自写函数的效率比较

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
#include <bit>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "TEST/std_bit.h"

using std::cout;
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
    void handle(Result &res1, Result &res2, Result &res3) {
        if (res1.value != res2.value || res1.value != res3.value) {
            cout << "wrong value!" << res1.bench_name << '\n';
            cout << "res1.value = " << res1.value << '\n';
            cout << "res2.value = " << res2.value << '\n';
            cout << "res3.value = " << res3.value << '\n';
        }
        auto show_ratio = [](auto r1, auto r2) {
            if (r1.time_cost) {
                int x = round(100.0 * r2.time_cost / r1.time_cost);
                if (x < 1000) cout << ' ';
                if (x < 100) cout << ' ';
                if (x < 10) cout << ' ';
                cout << x;
                cout << " %";
            } else if (!r2.time_cost)
                cout << " 100 %";
            else
                cout << "   inf";
        };
        if constexpr (show_content == SHOW_TIME)
            cout << int(res1.time_cost) << "\t"
                 << int(res2.time_cost) << "\t"
                 << int(res3.time_cost) << "\t" << res1.bench_name << '\n';
        else if constexpr (show_content == SHOW_RATIO) {
            show_ratio(res1, res2);
            cout << "\t\t\t\t";
            show_ratio(res1, res3);
            cout << "\t\t\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        } else if constexpr (show_content == SHOW_ALL) {
            cout << int(res1.time_cost) << "\t"
                 << int(res2.time_cost) << "\t"
                 << int(res3.time_cost) << "\t";
            show_ratio(res1, res2);
            cout << '\t';
            show_ratio(res1, res3);
            cout << "\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        }
    }
    template <size_t Id>
    std::string type_name() {
        if constexpr (Id == 0)
            return "<bit>";
        else if constexpr (Id == 1)
            return "gcc";
        else
            return "custom";
    }
    template <size_t Id>
    std::mt19937_64 &get_rng() {
        static std::mt19937_64 s_rng;
        return s_rng;
    }
    namespace CountRightZero {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::countr_zero(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::countr_zero(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::countr_zero_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace CountLeftZero {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::countl_zero(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::countl_zero(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::countl_zero_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace Popcnt {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::popcount(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::popcount(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::popcount_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace BitCeil {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::bit_ceil(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_ceil(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_ceil_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace BitFloor {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::bit_floor(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_floor(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_floor_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace BitWidth {
        template <typename Tp, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            Tp buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += std::bit_width(buffer[j]);
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_width(buffer[j]);
                else
                    for (size_t j = 0; j != Size; j++) sum += std::my_bit_ops::bit_width_msvc(buffer[j]);
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename Tp, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<Tp, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<Tp, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<Tp, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
}

#define RUN(U, ...) __VA_ARGS__(#__VA_ARGS__ ", " #U, U);
int main() {
    if constexpr (show_content == SHOW_TIME)
        cout << Bench::type_name<0>() << '\t'
             << Bench::type_name<1>() << '\t'
             << Bench::type_name<2>() << "\t\n";
    else if constexpr (show_content == SHOW_RATIO)
        cout << Bench::type_name<1>() << "/" << Bench::type_name<0>() << '\t'
             << Bench::type_name<2>() << "/" << Bench::type_name<0>() << "\t\n";
    else if constexpr (show_content == SHOW_ALL)
        cout << Bench::type_name<0>() << '\t'
             << Bench::type_name<1>() << '\t'
             << Bench::type_name<2>() << "\t\n";

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::Popcnt::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::Popcnt::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::Popcnt::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::Popcnt::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::Popcnt::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::Popcnt::run<uint64_t, 20000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::BitCeil::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::BitCeil::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::BitCeil::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::BitCeil::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::BitCeil::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::BitCeil::run<uint64_t, 20000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::BitFloor::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::BitFloor::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::BitFloor::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::BitFloor::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::BitFloor::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::BitFloor::run<uint64_t, 20000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::BitWidth::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::BitWidth::run<uint32_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 45), Bench::BitWidth::run<uint64_t, 20000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint64_t>(0, 1ull << 60), Bench::BitWidth::run<uint64_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.00000001), Bench::BitWidth::run<uint32_t, 20000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000000000000001), Bench::BitWidth::run<uint64_t, 20000, 1 << 12>);
}
/*
atcoder g++
<bit>	gcc	custom	
48	42	55	  88 %	 115 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
47	42	55	  89 %	 117 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
42	47	37	 112 %	  88 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
42	47	37	 112 %	  88 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
47	47	55	 100 %	 117 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
42	47	37	 112 %	  88 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
8	8	91	 100 %	1138 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(8)
8	8	80	 100 %	1000 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(8)
11	11	120	 100 %	1091 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
11	11	95	 100 %	 864 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
8	8	90	 100 %	1125 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(8)
11	11	117	 100 %	1064 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
11	11	11	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
11	11	11	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
11	9	9	  82 %	  82 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
11	9	9	  82 %	  82 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
11	11	11	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
11	11	9	 100 %	  82 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
11	11	104	 100 %	 945 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
11	11	101	 100 %	 918 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
70	70	116	 100 %	 166 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
70	70	162	 100 %	 231 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
11	11	173	 100 %	1573 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
70	70	128	 100 %	 183 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
67	11	97	  16 %	 145 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
67	11	96	  16 %	 143 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
66	57	108	  86 %	 164 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
66	56	142	  85 %	 215 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
66	11	148	  17 %	 224 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
67	53	120	  79 %	 179 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
8	11	78	 138 %	 975 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(8)
8	11	79	 138 %	 988 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(8)
11	14	107	 127 %	 973 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
11	14	109	 127 %	 991 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
8	11	78	 138 %	 975 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(8)
11	14	103	 127 %	 936 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)

*/
/*
atcoder clang++
<bit>	gcc	custom	
12	12	30	 100 %	 250 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
11	11	30	 100 %	 273 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
8	8	33	 100 %	 413 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)		too fast(8)
8	8	33	 100 %	 413 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)		too fast(8)
11	11	30	 100 %	 273 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
8	8	33	 100 %	 413 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)		too fast(8)
6	6	80	 100 %	1333 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(6)
6	6	107	 100 %	1783 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(6)
6	5	112	  83 %	1867 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)		too fast(6)
6	5	142	  83 %	2367 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)		too fast(6)
6	6	83	 100 %	1383 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(6)
6	5	114	  83 %	1900 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)		too fast(6)
11	11	34	 100 %	 309 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
11	11	34	 100 %	 309 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
5	5	35	 100 %	 700 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)		too fast(5)
5	5	35	 100 %	 700 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)		too fast(5)
11	11	34	 100 %	 309 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
5	5	35	 100 %	 700 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)		too fast(5)
14	14	110	 100 %	 786 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
14	14	103	 100 %	 736 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
12	12	135	 100 %	1125 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
12	12	173	 100 %	1442 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
14	14	208	 100 %	1486 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
12	12	155	 100 %	1292 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
14	12	123	  86 %	 879 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
14	12	106	  86 %	 757 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
14	12	136	  86 %	 971 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
14	12	175	  86 %	1250 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
13	12	211	  92 %	1623 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
14	12	128	  86 %	 914 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
8	11	75	 138 %	 938 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(8)
8	11	79	 138 %	 988 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(8)
6	11	131	 183 %	2183 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)		too fast(6)
6	11	139	 183 %	2317 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)		too fast(6)
8	11	102	 138 %	1275 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(8)
6	11	105	 183 %	1750 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)		too fast(6)

*/
/*
luogu g++
<bit>	gcc	custom	
31	31	60	 100 %	 194 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
31	31	60	 100 %	 194 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
31	31	46	 100 %	 148 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
31	31	46	 100 %	 148 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
31	31	60	 100 %	 194 %		Bench::CountRightZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
32	31	46	  97 %	 144 %		Bench::CountRightZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
8	8	91	 100 %	1138 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(8)
8	8	85	 100 %	1063 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(8)
11	11	130	 100 %	1182 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
11	11	123	 100 %	1118 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
8	8	129	 100 %	1613 %		Bench::CountLeftZero::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(8)
11	11	125	 100 %	1136 %		Bench::CountLeftZero::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
31	31	31	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
31	31	31	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
31	32	32	 103 %	 103 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
31	31	32	 100 %	 103 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
31	31	31	 100 %	 100 %		Bench::Popcnt::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
31	32	31	 103 %	 100 %		Bench::Popcnt::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
11	11	114	 100 %	1036 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
11	11	111	 100 %	1009 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
66	66	132	 100 %	 200 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
66	66	156	 100 %	 236 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
11	11	149	 100 %	1355 %		Bench::BitCeil::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
66	66	139	 100 %	 211 %		Bench::BitCeil::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
58	13	92	  22 %	 159 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
58	13	95	  22 %	 164 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
58	52	116	  90 %	 200 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
58	52	138	  90 %	 238 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
58	13	171	  22 %	 295 %		Bench::BitFloor::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)
58	52	126	  90 %	 217 %		Bench::BitFloor::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)
8	13	90	 163 %	1125 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)		too fast(8)
8	13	85	 163 %	1063 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)		too fast(8)
11	15	121	 136 %	1100 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 45)
12	15	121	 125 %	1008 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::uniform_int_distribution<uint64_t>(0, 1ull << 60)
8	13	106	 163 %	1325 %		Bench::BitWidth::run<uint32_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.00000001)		too fast(8)
12	15	118	 125 %	 983 %		Bench::BitWidth::run<uint64_t, 20000, 1 << 12>, std::exponential_distribution<double>(0.0000000000000001)

*/