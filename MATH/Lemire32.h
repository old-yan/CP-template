/*
最后修改:
20240416
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

namespace OY {
    struct Lemire32 {
        uint32_t m_mod;
        uint64_t m_M;
        Lemire32() = default;
        constexpr explicit Lemire32(uint32_t P) : m_mod(P), m_M(uint64_t(-1) / P + 1) {}
        friend uint32_t operator%(uint32_t a, const Lemire32 &x) { return ((__uint128_t)(x.m_M * a) * x.m_mod) >> 64; }
        friend uint32_t operator/(uint32_t a, const Lemire32 &x) { return ((__uint128_t)x.m_M * a) >> 64; }
    };
}

#endif