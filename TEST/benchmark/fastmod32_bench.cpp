/*
本文件比较 Lemire 的 32 位无符号数的除法和取模函数与机器原生运算的效率比较
由于各大 oj 的比较结果差距很大，所以贴出各个 oj 的 CPU 和测试结果

结论：
Barrett 比 Lemire 快的平台：
atcoder gcc:    Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
atcoder clang:  Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
luogu gcc：     Intel(R) Xeon(R) Platinum 8369HC CPU @ 3.30GHz
Lemire 比 Barrett 快的平台：
acwing gcc:     Intel(R) Xeon(R) CPU E5-2682 v4 @ 2.50GHz
codeforces gcc: Intel(R) Core(TM) i3-8100 CPU @ 3.60GHz
nowcoder gcc:   Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
nowcoder clang: Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
leetcode clang: Intel(R) Xeon(R) Platinum 8369B CPU @ 2.70GHz
*/
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "MATH/Barrett32.h"
#include "MATH/Lemire32.h"

using std::cout;

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
    template <size_t Id>
    std::string type_name() {
        if constexpr (Id == 0)
            return "native";
        else if constexpr (Id == 1)
            return "Limire";
        else
            return "Barrett";
    }
    template <size_t Id>
    std::mt19937 &get_rng() {
        static std::mt19937 s_rng;
        return s_rng;
    }
    namespace StaticDivide {
        template <uint32_t P, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            uint32_t buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            OY::Lemire32 L(P);
            OY::Barrett32 B(P);
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / P;
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / L;
                else
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / B;
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()};
        }
        template <uint32_t P, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<P, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<P, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<P, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace StaticModular {
        template <uint32_t P, size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, Distributer &&D) {
            uint32_t buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            OY::Lemire32 L(P);
            OY::Barrett32 B(P);
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % P;
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % L;
                else
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % B;
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()};
        }
        template <uint32_t P, size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, Distributer &&D) {
            auto res1 = bench<P, 0, Repeat, Size>(bench_name, D);
            auto res2 = bench<P, 1, Repeat, Size>(bench_name, D);
            auto res3 = bench<P, 2, Repeat, Size>(bench_name, D);
            handle(res1, res2, res3);
        }
    }
    namespace DynamicDivide {
        template <size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, uint32_t P, Distributer &&D) {
            uint32_t buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            OY::Lemire32 L(P);
            OY::Barrett32 B(P);
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / P;
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / L;
                else
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] / B;
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()};
        }
        template <size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, uint32_t P, Distributer &&D) {
            auto res1 = bench<0, Repeat, Size>(bench_name, P, D);
            auto res2 = bench<1, Repeat, Size>(bench_name, P, D);
            auto res3 = bench<2, Repeat, Size>(bench_name, P, D);
            handle(res1, res2, res3);
        }
    }
    namespace DynamicModular {
        template <size_t Id, size_t Repeat, size_t Size, typename Distributer>
        Result bench(std::string name, uint32_t P, Distributer &&D) {
            uint32_t buffer[Size];
            for (size_t j = 0; j != Size; j++) buffer[j] = D(get_rng<Id>());
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            OY::Lemire32 L(P);
            OY::Barrett32 B(P);
            for (size_t i = 0; i != Repeat; i++) {
                size_t sum{};
                if constexpr (Id == 0)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % P;
                else if constexpr (Id == 1)
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % L;
                else
                    for (size_t j = 0; j != Size; j++) sum += buffer[j] % B;
                value += sum;
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name<Id>(), name, value, Repeat, std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()};
        }
        template <size_t Repeat, size_t Size, typename Distributer>
        void run(std::string bench_name, uint32_t P, Distributer &&D) {
            auto res1 = bench<0, Repeat, Size>(bench_name, P, D);
            auto res2 = bench<1, Repeat, Size>(bench_name, P, D);
            auto res3 = bench<2, Repeat, Size>(bench_name, P, D);
            handle(res1, res2, res3);
        }
    }
}

#define RUN(U, ...) __VA_ARGS__(#__VA_ARGS__ ", " #U, U);
#define RUN2(U, P, ...) __VA_ARGS__(#__VA_ARGS__ ", " #U, P, U);
int main() {
    cout << Bench::type_name<0>() << '\t'
         << Bench::type_name<1>() << '\t'
         << Bench::type_name<2>() << "\t\n";
    static constexpr uint32_t P = 10003;
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::StaticDivide::run<P, 4000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::StaticDivide::run<P, 4000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.001), Bench::StaticDivide::run<P, 4000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000001), Bench::StaticDivide::run<P, 4000, 1 << 12>);

    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 15), Bench::StaticModular::run<P, 4000, 1 << 12>);
    RUN(std::uniform_int_distribution<uint32_t>(0, 1 << 30), Bench::StaticModular::run<P, 4000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.001), Bench::StaticModular::run<P, 4000, 1 << 12>);
    RUN(std::exponential_distribution<double>(0.0000001), Bench::StaticModular::run<P, 4000, 1 << 12>);

    RUN2(std::uniform_int_distribution<uint32_t>(0, 1 << 15), P, Bench::DynamicDivide::run<4000, 1 << 12>);
    RUN2(std::uniform_int_distribution<uint32_t>(0, 1 << 30), P, Bench::DynamicDivide::run<4000, 1 << 12>);
    RUN2(std::exponential_distribution<double>(0.001), P, Bench::DynamicDivide::run<4000, 1 << 12>);
    RUN2(std::exponential_distribution<double>(0.0000001), P, Bench::DynamicDivide::run<4000, 1 << 12>);

    RUN2(std::uniform_int_distribution<uint32_t>(0, 1 << 15), P, Bench::DynamicModular::run<4000, 1 << 12>);
    RUN2(std::uniform_int_distribution<uint32_t>(0, 1 << 30), P, Bench::DynamicModular::run<4000, 1 << 12>);
    RUN2(std::exponential_distribution<double>(0.001), P, Bench::DynamicModular::run<4000, 1 << 12>);
    RUN2(std::exponential_distribution<double>(0.0000001), P, Bench::DynamicModular::run<4000, 1 << 12>);
}
/*
在 atcoder C++ 23 (gcc 12.2) 语言条件下测试
cmd:        g++-12 -std=gnu++2b -O2 -DONLINE_JUDGE -DATCODER -Wall -Wextra -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647 -I/opt/ac-library -I/opt/boost/gcc/include -L/opt/boost/gcc/lib-lgmpxx -lgmp -I/usr/include/eigen3
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
native	Limire	Barrett	
2563	9519	5316	 371 %	 207 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
2389	9494	5247	 397 %	 220 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
2379	9539	5156	 401 %	 217 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
2361	10084	5122	 427 %	 217 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
3384	9479	5274	 280 %	 156 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
3375	9465	5245	 280 %	 155 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
3392	9464	5191	 279 %	 153 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
3383	9430	5202	 279 %	 154 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
28236	9440	5188	  33 %	  18 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
28200	9466	5170	  34 %	  18 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
28191	9467	5150	  34 %	  18 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
28192	9469	5140	  34 %	  18 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
28218	9448	5251	  33 %	  19 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
28210	9465	5231	  34 %	  19 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
28198	9501	5181	  34 %	  18 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
28197	9425	5222	  33 %	  19 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)

*/
/*
在 atcoder C++ 23 (clang 16.0.6) 语言条件下测试
cmd:        clang++ -std=c++2b -Wall -Wextra -O2 -DONLINE_JUDGE -DATCODER -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647 -I/opt/boost/clang/include -L/opt/boost/clang/lib -I/opt/ac-library -I/usr/include/eigen3 -fuse-ld=lld
CPU:        Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz
native	Limire	Barrett	
3276	13627	6700	 416 %	 205 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
3056	13627	6233	 446 %	 204 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
3044	13635	6199	 448 %	 204 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
3041	13625	6201	 448 %	 204 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
4617	18250	6153	 395 %	 133 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
4614	18264	6153	 396 %	 133 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
4613	18255	6153	 396 %	 133 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
4611	18264	6153	 396 %	 133 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
28233	13969	7141	  49 %	  25 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
28217	13971	7140	  50 %	  25 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
28192	13932	7144	  49 %	  25 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
28199	13935	7143	  49 %	  25 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
28201	18016	7223	  64 %	  26 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
28235	17970	7217	  64 %	  26 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
28199	17952	7222	  64 %	  26 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
28199	17932	7221	  64 %	  26 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)

*/
/*
在 luogu C++ 20 (gcc 13.2.0) 语言条件下测试
cmd:        g++ -x c++ -std=C++2a -fPIC -DONLINE_JUDGE -Wall -fno-asm -lm -march=native
CPU:        Intel(R) Xeon(R) Platinum 8369HC CPU @ 3.30GHz
native	Limire	Barrett	
2382	9208	5889	 387 %	 247 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
2350	9207	5903	 392 %	 251 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
2371	9176	5874	 387 %	 248 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
2381	9157	5897	 385 %	 248 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
3642	8712	5765	 239 %	 158 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
3624	8713	5770	 240 %	 159 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
3702	8713	5771	 235 %	 156 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
3692	8708	5786	 236 %	 157 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
34748	9131	5886	  26 %	  17 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
34756	9148	5888	  26 %	  17 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
34777	9121	5879	  26 %	  17 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
34770	9117	5875	  26 %	  17 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
34776	8724	5790	  25 %	  17 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
34758	8727	5780	  25 %	  17 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
34754	8713	5780	  25 %	  17 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
34741	8719	5779	  25 %	  17 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)

*/
/*
在 acwing C++ 17 (gcc 9.2.1) 语言条件下测试
cmd:        pragma 开启 Ofast
CPU:        Intel(R) Xeon(R) CPU E5-2682 v4 @ 2.50GHz
native	Limire	Barrett	
7349	12305	20340	 167 %	 277 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
7223	12937	20500	 179 %	 284 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
7247	12199	19339	 168 %	 267 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
7324	12237	19232	 167 %	 263 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
15425	12931	23917	  84 %	 155 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
15482	11705	24050	  76 %	 155 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
15304	11988	23872	  78 %	 156 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
16463	12027	24020	  73 %	 146 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
59691	12169	18700	  20 %	  31 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
58869	13130	19027	  22 %	  32 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
58727	12417	18852	  21 %	  32 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
59344	12413	18835	  21 %	  32 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
58116	12045	20956	  21 %	  36 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
58411	11994	20877	  21 %	  36 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
59315	11820	20949	  20 %	  35 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
58688	12023	20843	  20 %	  36 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
*/
/*
在 codeforces C++ 20 (gcc 13.2.0) 语言条件下测试
cmd:        g++ -Wall -Wextra -Wconversion -static -DONLINE_JUDGE -Wl,--stack=268435456 -O2 -std=c++20
CPU:        Intel(R) Core(TM) i3-8100 CPU @ 3.60GHz
native	Limire	Barrett	
4640	13977	21468	 301 %	 463 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
4806	13791	21398	 287 %	 445 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
5771	13829	21369	 240 %	 370 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
5866	13970	21493	 238 %	 366 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
10910	9257	20763	  85 %	 190 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
10796	9283	20713	  86 %	 192 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
10773	9258	19527	  86 %	 181 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
10867	9246	21171	  85 %	 195 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
32688	9664	18524	  30 %	  57 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
32190	9716	18427	  30 %	  57 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
36820	9658	23152	  26 %	  63 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
36821	9711	23100	  26 %	  63 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
32151	9251	17890	  29 %	  56 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
32176	9261	17996	  29 %	  56 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
36798	9319	17892	  25 %	  49 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
36714	9266	17902	  25 %	  49 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)

*/
/*
在 nowcoder C++ 17 (gcc 7.5.0) 语言条件下测试
cmd:        g++ -std=c++17 -O2 -Wsign-compare -DONLINE_JUDGE -Werror=return-type
CPU:        Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
native	Limire	Barrett	
15827	11377	24506	  72 %	 155 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
14348	11027	24419	  77 %	 170 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
14289	11057	24395	  77 %	 171 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
14342	11026	24440	  77 %	 170 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
15846	10433	22408	  66 %	 141 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
15894	10379	22421	  65 %	 141 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
15886	10436	22254	  66 %	 140 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
15906	10389	22288	  65 %	 140 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
41597	11054	24194	  27 %	  58 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
41453	11011	24297	  27 %	  59 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
41347	10975	24122	  27 %	  58 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
41418	10975	24122	  26 %	  58 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
43853	10502	22441	  24 %	  51 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
41411	10412	22407	  25 %	  54 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
41476	10393	22471	  25 %	  54 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
41494	10411	22480	  25 %	  54 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
*/
/*
在 nowcoder C++ 17 (clang 11.0.1) 语言条件下测试
cmd:        clang++ -std=c++17 -O2 -Wsign-compare
CPU:        Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
native	Limire	Barrett	
9271	19173	20924	 207 %	 226 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
7594	19145	20922	 252 %	 276 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
7633	19148	20887	 251 %	 274 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
7604	19146	20905	 252 %	 275 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
10434	10369	19457	  99 %	 186 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
10399	10380	19487	 100 %	 187 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
10433	10352	19534	  99 %	 187 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
10423	10362	19525	  99 %	 187 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
41127	19233	16424	  47 %	  40 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
38850	19176	16421	  49 %	  42 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
33663	18684	16452	  56 %	  49 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
33683	18660	16477	  55 %	  49 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
33783	10370	19190	  31 %	  57 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
33750	10361	19152	  31 %	  57 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
33678	10373	19149	  31 %	  57 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
33666	10375	19360	  31 %	  58 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
*/
/*
在 leetcode C++ 20 (clang) 语言条件下测试
cmd:        未知
CPU:        Intel(R) Xeon(R) Platinum 8369B CPU @ 2.70GHz
native	Limire	Barrett	
11024	12994	19968	 118 %	 181 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
10535	13362	31181	 127 %	 296 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
11896	14281	20238	 120 %	 170 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
10956	13777	22277	 126 %	 203 %		Bench::StaticDivide::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
14277	14082	21351	  99 %	 150 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
13873	13273	20775	  96 %	 150 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
19085	13781	20818	  72 %	 109 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.001)
14704	14609	20555	  99 %	 140 %		Bench::StaticModular::run<P, 4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
29009	12614	19930	  43 %	  69 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
29164	13347	20245	  46 %	  69 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
28895	12505	20724	  43 %	  72 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
29163	13615	20126	  47 %	  69 %		Bench::DynamicDivide::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)
29118	13259	20169	  46 %	  69 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 15)
28728	13666	21086	  48 %	  73 %		Bench::DynamicModular::run<4000, 1 << 12>, std::uniform_int_distribution<uint32_t>(0, 1 << 30)
29515	13889	19977	  47 %	  68 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.001)
29554	14323	20121	  48 %	  68 %		Bench::DynamicModular::run<4000, 1 << 12>, std::exponential_distribution<double>(0.0000001)

*/