/*
最后修改:
20231123
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CHINESEREMAINDERTHEOREM__
#define __OY_CHINESEREMAINDERTHEOREM__

#include <algorithm>
#include <cstdint>
#include <vector>

#ifdef _MSC_VER
#include <immintrin.h>
#endif

namespace OY {
    struct ChineseRemainderTheorem {
        struct node {
            uint64_t m_rem, m_div;
        };
        std::vector<node> m_sub;
        uint64_t m_prod = 1;
        ChineseRemainderTheorem(uint32_t n = 0) { m_sub.reserve(n); }
        void add(uint64_t rem, uint64_t div) {
            if (div > 1) m_sub.push_back({rem, div}), m_prod *= div;
        }
        static uint64_t getInv(uint64_t a, uint64_t p) {
            uint64_t x = p, y = a, m0 = 0, m1 = 1;
            while (y) {
                uint64_t z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= p) m0 += p / x;
            return m0;
        }
        node query() const {
            uint64_t res = 0;
            for (auto &sub : m_sub) {
                const uint64_t rem = sub.m_rem, div = sub.m_div;
                uint64_t rest = m_prod / div, inv = getInv(rest, div);
                auto mul = [&](uint64_t a, uint64_t b) {
#ifdef _MSC_VER
                    uint64_t high, low, res;
                    low = _umul128(a, b, &high);
                    _udiv128(high, low, m_prod, &res);
#else
                    uint64_t res = __uint128_t(a) * b % m_prod;
#endif
                    return res;
                };
                res += mul(mul(rem, rest), inv);
                if (res >= m_prod) res -= m_prod;
            }
            return {res, m_prod};
        }
    };
}

#endif