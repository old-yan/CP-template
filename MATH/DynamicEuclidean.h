#ifndef __OY_DYNAMICEUCLIDEAN__
#define __OY_DYNAMICEUCLIDEAN__

#include <cstdint>
#include "DynamicModularInverse.h"

namespace OY {
    struct DynamicEuclidean {
        struct _DynamicEuclidean_ans {
            uint32_t f, g, h;
        };
        DynamicModularInverse32 m_dmi;
        Barrett32 &m_brt;
        uint32_t m_inv2;
        uint32_t m_inv6;
        DynamicEuclidean(uint32_t __P) : m_dmi(__P), m_brt(m_dmi.m_mg.m_brt), m_inv2(m_dmi.query_exgcd(2)), m_inv6(m_dmi.query_exgcd(6)) {}
        _DynamicEuclidean_ans calc(uint32_t __a, uint32_t __b, uint32_t __c, uint32_t __n) const {
#define MUL(x, y) m_brt.multiply_64((x), (y))
#define ADD(x, y) ({auto z=(x)+(y);if(z>=m_brt.mod())z-=m_brt.mod();z; })
            uint32_t k1 = __a / __c, k2 = __b / __c;
            __a -= k1 * __c;
            __b -= k2 * __c;
            const uint32_t m = (uint64_t(__a) * __n + __b) / __c, s1 = __n + 1, s2 = MUL(MUL(__n, __n + 1), m_inv2), s3 = MUL(MUL(MUL(__n, __n + 1), __n * 2 + 1), m_inv6);
            uint32_t F = ADD(MUL(s2, k1), MUL(s1, k2));
            uint32_t G = ADD(MUL(s3, k1), MUL(s2, k2));
            uint32_t H = ADD(ADD(MUL(s3, MUL(k1, k1)), MUL(s1, MUL(k2, k2))), MUL(s2 * 2, MUL(k1, k2)));
            if (__a && m) {
                auto [f, g, h] = calc(__c, __c - __b - 1, __a, m - 1);
                uint32_t df = ADD(MUL(__n, m), m_brt.mod() - f);
                F = ADD(F, df);
                uint32_t dg = ADD(MUL(m, s2), m_brt.mod() - MUL(f + h, m_inv2));
                G = ADD(G, dg);
                uint32_t dh = ADD(ADD(ADD(ADD(MUL(__n, MUL(m, m + 1)), ADD(m_brt.mod() - g, m_brt.mod() - g)), ADD(m_brt.mod() - f, m_brt.mod() - f)), m_brt.mod() - df), ADD(MUL(2 * k1, dg), MUL(2 * k2, df)));
                H = ADD(H, dh);
            }
#undef MUL
#undef ADD
            return {F, G, H};
        }
    };
}

#endif