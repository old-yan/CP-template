#ifndef __OY_STATICEUCLIDEAN__
#define __OY_STATICEUCLIDEAN__

#include <cstdint>
#include "StaticModularInverse.h"

namespace OY {
    template <uint32_t _P>
    struct StaticEuclidean {
        static constexpr uint32_t inv_2 = OY::StaticModularInverse32<_P>::query_exgcd(2), inv_6 = OY::StaticModularInverse32<_P>::query_exgcd(6);
        struct _StaticEuclidean_ans {
            uint32_t f, g, h;
        };
        static _StaticEuclidean_ans calc(uint32_t __a, uint32_t __b, uint32_t __c, uint32_t __n) {
#define MUL(x, y) OY::Modular32<_P>::multiply_64((x), (y))
#define ADD(x, y) ({auto z=(x)+(y);if(z>=_P)z-=_P;z; })
            uint32_t k1 = __a / __c, k2 = __b / __c;
            __a -= k1 * __c;
            __b -= k2 * __c;
            const uint32_t m = (uint64_t(__a) * __n + __b) / __c, s1 = __n + 1, s2 = MUL(MUL(__n, __n + 1), inv_2), s3 = MUL(MUL(MUL(__n, __n + 1), __n * 2 + 1), inv_6);
            uint32_t F = ADD(MUL(s2, k1), MUL(s1, k2));
            uint32_t G = ADD(MUL(s3, k1), MUL(s2, k2));
            uint32_t H = ADD(ADD(MUL(s3, MUL(k1, k1)), MUL(s1, MUL(k2, k2))), MUL(s2 * 2, MUL(k1, k2)));
            if (__a && m) {
                auto [f, g, h] = calc(__c, __c - __b - 1, __a, m - 1);
                uint32_t df = ADD(MUL(__n, m), _P - f);
                F = ADD(F, df);
                uint32_t dg = ADD(MUL(m, s2), _P - MUL(f + h, inv_2));
                G = ADD(G, dg);
                uint32_t dh = ADD(ADD(ADD(ADD(MUL(__n, MUL(m, m + 1)), ADD(_P - g, _P - g)), ADD(_P - f, _P - f)), _P - df), ADD(MUL(2 * k1, dg), MUL(2 * k2, df)));
                H = ADD(H, dh);
            }
#undef MUL
#undef ADD
            return {F, G, H};
        }
    };
}

#endif