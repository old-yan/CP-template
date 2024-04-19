/*
最后修改:
20240419
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BARRETT32__
#define __OY_BARRETT32__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    struct Barrett32 {
        uint32_t m_P, m_Pinv;
        Barrett32() = default;
        constexpr explicit Barrett32(uint32_t P) : m_P(P), m_Pinv(-uint32_t(P) / P + 1) {}
        friend uint32_t operator%(uint32_t a, const Barrett32 &x) {
            a -= ((uint64_t(x.m_Pinv) * a >> 32) + 1) * x.m_P;
            if (a >= x.m_P) a += x.m_P;
            return a;
        }
        friend uint32_t operator/(uint32_t a, const Barrett32 &x) {
            uint32_t b = (uint64_t(x.m_Pinv) * a >> 32) + 1;
            return b - (a - b * x.m_P >= x.m_P);
        }
    };
}

#endif