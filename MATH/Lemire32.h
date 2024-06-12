/*
最后修改:
20240611
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LEMIRE32__
#define __OY_LEMIRE32__

#include <algorithm>
#include <cstdint>
#include <numeric>

#ifdef _MSC_VER
#include <immintrin.h>
#endif

namespace OY {
    struct Lemire32 {
        uint32_t m_mod;
        uint64_t m_M;
        Lemire32() = default;
        static uint32_t _mul_high(uint64_t a, uint64_t b) {
#ifdef _MSC_VER
            uint64_t high;
            _umul128(a, b, &high);
            return high;
#else
            return ((__uint128_t)a * b) >> 64;
#endif
        }
        constexpr explicit Lemire32(uint32_t P) : m_mod(P), m_M(uint64_t(-1) / P + 1) {}
        friend uint32_t operator%(uint32_t a, const Lemire32 &x) { return _mul_high(x.m_M * a, x.m_mod); }
        friend uint32_t operator/(uint32_t a, const Lemire32 &x) { return _mul_high(x.m_M, a); }
    };
}

#endif