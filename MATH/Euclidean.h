#ifndef __OY_EUCLIDEAN__
#define __OY_EUCLIDEAN__

#include "StaticModInt.h"

namespace OY {
    template <typename _Tp>
    struct Euclidean {
        struct _Euclidean_ans {
            _Tp f, g, h;
        };
        const _Tp m_inv2, m_inv6;
        Euclidean() : m_inv2(_Tp(2).inv()), m_inv6(_Tp(6).inv()) {}
        _Euclidean_ans calc(uint32_t __a, uint32_t __b, uint32_t __c, uint32_t __n) const {
            auto [aq, ar] = std::div(int(__a), int(__c));
            auto [bq, br] = std::div(int(__b), int(__c));
            const uint32_t m = (uint64_t(ar) * __n + br) / __c;
            const _Tp k1(aq), k2(bq), s1(__n + 1), s2(_Tp(__n) * _Tp(__n + 1) * m_inv2), s3(_Tp(__n) * _Tp(__n + 1) * _Tp(__n * 2 + 1) * m_inv6);
            _Tp F(s2 * k1 + s1 * k2), G(s3 * k1 + s2 * k2), H(s3 * k1 * k1 + s1 * k2 * k2 + s2 * k1 * k2 * _Tp(2));
            if (ar && m) {
                auto [f, g, h] = calc(__c, __c - br - 1, ar, m - 1);
                _Tp df = _Tp(__n) * _Tp(m) - f;
                F += df;
                _Tp dg = s2 * _Tp(m) - (f + h) * m_inv2;
                G += dg;
                _Tp dh = _Tp(__n) * _Tp(m) * _Tp(m) + (k1 * dg + k2 * df - g) * _Tp(2) - f;
                H += dh;
            }
            return {F, G, H};
        }
    };
}

#endif