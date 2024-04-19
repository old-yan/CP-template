/*
本文件比较各种 32 位自取模数数类的构造、运算、并行运算的效率高低

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

#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicMontgomeryModInt32.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

using std::cout;
using mint1 = OY::StaticModInt32<998244353, true>;
using mint2 = OY::DynamicModInt32<0>;
using mint3 = OY::StaticMontgomeryModInt32<998244353, true>;
using mint4 = OY::DynamicMontgomeryModInt32<0>;
auto _ = []() {
    mint2::set_mod(998244353, true);
    mint4::set_mod(998244353, true);
    return 0;
}();
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
    void handle(Result &res1, Result &res2, Result &res3, Result &res4) {
        if (res1.value != res2.value || res1.value != res3.value || res1.value != res4.value) {
            cout << "wrong value!" << res1.bench_name << '\n';
            cout << "res1.value = " << res1.value << '\n';
            cout << "res2.value = " << res2.value << '\n';
            cout << "res3.value = " << res3.value << '\n';
            cout << "res4.value = " << res4.value << '\n';
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
            cout << int(res1.time_cost) << "\t\t"
                 << int(res2.time_cost) << "\t\t"
                 << int(res3.time_cost) << "\t\t"
                 << int(res4.time_cost) << "\t\t" << res1.bench_name << '\n';
        else if constexpr (show_content == SHOW_RATIO) {
            show_ratio(res1, res2);
            cout << "\t\t\t\t";
            show_ratio(res1, res3);
            cout << "\t\t\t\t";
            show_ratio(res1, res4);
            cout << "\t\t\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        } else if constexpr (show_content == SHOW_ALL) {
            cout << int(res1.time_cost) << "\t\t"
                 << int(res2.time_cost) << "\t\t"
                 << int(res3.time_cost) << "\t\t"
                 << int(res4.time_cost) << "\t\t";
            show_ratio(res1, res2);
            cout << '\t';
            show_ratio(res1, res3);
            cout << '\t';
            show_ratio(res1, res4);
            cout << "\t\t" << res1.bench_name;
            if (res1.time_cost < 10) cout << "\t\ttoo fast(" << int(res1.time_cost) << ')';
            cout << '\n';
        }
    }
    template <uint32_t P, bool IsPrime>
    std::string type_name(const OY::StaticModInt32<P, IsPrime> &x) {
        return "StaticModInt ";
    }
    template <size_t Id>
    std::string type_name(const OY::DynamicModInt32<Id> &x) {
        return "DynamicModInt";
    }
    template <uint32_t P, bool IsPrime>
    std::string type_name(const OY::StaticMontgomeryModInt32<P, IsPrime> &x) {
        return "StaticMont   ";
    }
    template <size_t Id>
    std::string type_name(const OY::DynamicMontgomeryModInt32<Id> &x) {
        return "DynamicMont  ";
    }
    template <typename ModInt>
    std::mt19937 &get_rng(const ModInt &x) {
        static std::mt19937 s_rng;
        return s_rng;
    }
    template <typename ModInt>
    std::mt19937_64 &get_rng64(const ModInt &x) {
        static std::mt19937_64 s_rng;
        return s_rng;
    }
    namespace ModUnsigned32 {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            uint32_t buffer1[Len];
            ModInt buffer2[Len];
            for (size_t j = 0; j != Len; j++) buffer1[j] = get_rng(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                for (size_t j = 0; j != Len; j++) buffer2[j] = ModInt(buffer1[j]);
                value += buffer2[get_rng(ModInt{})() % Len].val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace ModSigned32 {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            int buffer1[Len];
            ModInt buffer2[Len];
            for (size_t j = 0; j != Len; j++) buffer1[j] = get_rng(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                for (size_t j = 0; j != Len; j++) buffer2[j] = ModInt(buffer1[j]);
                value += buffer2[get_rng(ModInt{})() % Len].val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace ModUnsigned64 {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            uint64_t buffer1[Len];
            ModInt buffer2[Len];
            for (size_t j = 0; j != Len; j++) buffer1[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                for (size_t j = 0; j != Len; j++) buffer2[j] = ModInt(buffer1[j]);
                value += buffer2[get_rng(ModInt{})() % Len].val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace ModSigned64 {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            int64_t buffer1[Len];
            ModInt buffer2[Len];
            for (size_t j = 0; j != Len; j++) buffer1[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                for (size_t j = 0; j != Len; j++) buffer2[j] = ModInt(buffer1[j]);
                value += buffer2[get_rng(ModInt{})() % Len].val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace Sum {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len];
            for (size_t j = 0; j != Len; j++) buffer[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt sum{};
                for (size_t j = 0; j != Len; j++) sum += buffer[j];
                value = value + sum.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace Prod {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len];
            for (size_t j = 0; j != Len; j++) buffer[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt prod{1};
                for (size_t j = 0; j != Len; j++) prod *= buffer[j];
                value = value + prod.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace Minus {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len];
            for (size_t j = 0; j != Len; j++) buffer[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt sum{};
                for (size_t j = 0; j != Len; j++) sum -= buffer[j];
                value = value + sum.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace Div {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len];
            for (size_t j = 0; j != Len; j++) {
                buffer[j] = get_rng64(ModInt{})();
                assert(buffer[j].val());
            }
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt prod{1};
                for (size_t j = 0; j != Len; j++) prod /= buffer[j];
                value = value + prod.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace MulAdd {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len][2];
            for (size_t j = 0; j != Len; j++) {
                buffer[j][0] = get_rng64(ModInt{})();
                buffer[j][1] = get_rng64(ModInt{})();
            }
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt res{1};
                for (size_t j = 0; j != Len; j++) res = res * buffer[j][0] + buffer[j][1];
                value = value + res.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace ParallelSum {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len * 8];
            for (size_t j = 0; j != Len * 8; j++) buffer[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt res1{}, res2{}, res3{}, res4{}, res5{}, res6{}, res7{}, res8{};
                for (size_t j = 0; j != Len * 4; j += 4) {
                    res1 += buffer[j], res2 += buffer[j + 1], res3 += buffer[j + 2], res4 += buffer[j + 3],
                        res5 += buffer[j + 4], res6 += buffer[j + 5], res7 += buffer[j + 6], res8 += buffer[j + 7];
                }
                value = value + res1.val() + res2.val() + res3.val() + res4.val() + res5.val() + res6.val() + res7.val() + res8.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
    namespace ParallelProd {
        template <typename ModInt, size_t Repeat, size_t Len>
        Result bench(std::string name) {
            ModInt buffer[Len * 8];
            for (size_t j = 0; j != Len * 8; j++) buffer[j] = get_rng64(ModInt{})();
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile size_t value = 0;
            for (size_t i = 0; i != Repeat; i++) {
                ModInt res1{1}, res2{1}, res3{1}, res4{1}, res5{1}, res6{1}, res7{1}, res8{1};
                for (size_t j = 0; j != Len * 4; j += 4) {
                    res1 *= buffer[j], res2 *= buffer[j + 1], res3 *= buffer[j + 2], res4 *= buffer[j + 3],
                        res5 *= buffer[j + 4], res6 *= buffer[j + 5], res7 *= buffer[j + 6], res8 *= buffer[j + 7];
                }
                value = value + res1.val() + res2.val() + res3.val() + res4.val() + res5.val() + res6.val() + res7.val() + res8.val();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            return {type_name(ModInt{}), name, value, Repeat, std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()};
        }
        template <typename ModInt1, typename ModInt2, typename ModInt3, typename ModInt4, size_t Repeat, size_t Len>
        void run(std::string bench_name) {
            auto res1 = bench<ModInt1, Repeat, Len>(bench_name);
            auto res2 = bench<ModInt2, Repeat, Len>(bench_name);
            auto res3 = bench<ModInt3, Repeat, Len>(bench_name);
            auto res4 = bench<ModInt4, Repeat, Len>(bench_name);
            handle(res1, res2, res3, res4);
        }
    }
}

#define RUN(...) __VA_ARGS__(#__VA_ARGS__);
int main() {
    if constexpr (show_content == SHOW_TIME)
        cout << Bench::type_name(mint1()) << '\t'
             << Bench::type_name(mint2()) << '\t'
             << Bench::type_name(mint3()) << '\t'
             << Bench::type_name(mint4()) << "\t\n";
    else if constexpr (show_content == SHOW_RATIO)
        cout << Bench::type_name(mint2()) << "/" << Bench::type_name(mint1()) << '\t'
             << Bench::type_name(mint3()) << "/" << Bench::type_name(mint1()) << '\t'
             << Bench::type_name(mint4()) << "/" << Bench::type_name(mint1()) << "\t\n";
    else if constexpr (show_content == SHOW_ALL)
        cout << Bench::type_name(mint1()) << '\t'
             << Bench::type_name(mint2()) << '\t'
             << Bench::type_name(mint3()) << '\t'
             << Bench::type_name(mint4()) << "\t\n";

    RUN(Bench::ModUnsigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::ModSigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::ModUnsigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::ModSigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::Sum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>);
    RUN(Bench::Minus::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>);
    RUN(Bench::Prod::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::MulAdd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
    RUN(Bench::Div::run<mint1, mint2, mint3, mint4, 1000, 1 << 12>);

    RUN(Bench::ParallelSum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>);
    RUN(Bench::ParallelProd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>);
}
/*
atcoder g++
StaticModInt 	DynamicModInt	StaticMont   	DynamicMont  	
7		36		27		55		 514 %	 386 %	 786 %		Bench::ModUnsigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(7)
7		70		27		78		1000 %	 386 %	1114 %		Bench::ModSigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(7)
28		37		58		60		 132 %	 207 %	 214 %		Bench::ModUnsigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
47		117		69		121		 249 %	 147 %	 257 %		Bench::ModSigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
94		75		75		75		  80 %	  80 %	  80 %		Bench::Sum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
94		76		70		75		  81 %	  74 %	  80 %		Bench::Minus::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
146		164		131		138		 112 %	  90 %	  95 %		Bench::Prod::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
194		189		178		178		  97 %	  92 %	  92 %		Bench::MulAdd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
402		493		398		406		 123 %	  99 %	 101 %		Bench::Div::run<mint1, mint2, mint3, mint4, 1000, 1 << 12>
260		244		184		194		  94 %	  71 %	  75 %		Bench::ParallelSum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
282		319		282		282		 113 %	 100 %	 100 %		Bench::ParallelProd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>

*/
/*
atcoder clang++
StaticModInt 	DynamicModInt	StaticMont   	DynamicMont  	
7		43		21		69		 614 %	 300 %	 986 %		Bench::ModUnsigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(7)
7		70		22		78		1000 %	 314 %	1114 %		Bench::ModSigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(7)
23		32		58		54		 139 %	 252 %	 235 %		Bench::ModUnsigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
45		117		73		117		 260 %	 162 %	 260 %		Bench::ModSigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
75		93		71		75		 124 %	  95 %	 100 %		Bench::Sum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
98		50		98		98		  51 %	 100 %	 100 %		Bench::Minus::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
141		182		128		131		 129 %	  91 %	  93 %		Bench::Prod::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
182		217		171		171		 119 %	  94 %	  94 %		Bench::MulAdd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
409		572		377		371		 140 %	  92 %	  91 %		Bench::Div::run<mint1, mint2, mint3, mint4, 1000, 1 << 12>
289		289		203		167		 100 %	  70 %	  58 %		Bench::ParallelSum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
282		806		434		282		 286 %	 154 %	 100 %		Bench::ParallelProd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>

*/
/*
luogu g++
StaticModInt 	DynamicModInt	StaticMont   	DynamicMont  	
8		38		29		63		 475 %	 363 %	 788 %		Bench::ModUnsigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(8)
9		86		30		120		 956 %	 333 %	1333 %		Bench::ModSigned32::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>		too fast(9)
29		39		55		62		 134 %	 190 %	 214 %		Bench::ModUnsigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
43		295		71		325		 686 %	 165 %	 756 %		Bench::ModSigned64::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
87		66		65		65		  76 %	  75 %	  75 %		Bench::Sum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
87		65		65		65		  75 %	  75 %	  75 %		Bench::Minus::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
129		152		118		118		 118 %	  91 %	  91 %		Bench::Prod::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
173		188		162		162		 109 %	  94 %	  94 %		Bench::MulAdd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>
407		455		383		383		 112 %	  94 %	  94 %		Bench::Div::run<mint1, mint2, mint3, mint4, 1000, 1 << 12>
260		282		217		239		 108 %	  83 %	  92 %		Bench::ParallelSum::run<mint1, mint2, mint3, mint4, 20000, 1 << 12>
264		302		260		260		 114 %	  98 %	  98 %		Bench::ParallelProd::run<mint1, mint2, mint3, mint4, 10000, 1 << 12>

*/