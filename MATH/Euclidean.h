/*
最后修改:
20240323
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EUCLIDEAN__
#define __OY_EUCLIDEAN__

#include "StaticModInt32.h"

namespace OY {
    template <typename Tp>
    struct Euclidean {
        struct EuclideanTuple {
            Tp m_f, m_g, m_h;
        };
        const Tp m_inv2 = Tp::raw(2).inv(), m_inv6 = Tp::raw(6).inv();
        EuclideanTuple calc(uint32_t a, uint32_t b, uint32_t c, uint32_t n) const {
            const uint32_t aq = a / c, ar = a - aq * c, bq = b / c, br = b - bq * c, m = (uint64_t(ar) * n + br) / c;
            const Tp k1(aq), k2(bq), s1(n + 1), s2(Tp::raw(n) * Tp::raw(n + 1) * m_inv2), s3(Tp::raw(n) * Tp::raw(n + 1) * Tp::raw(n * 2 + 1) * m_inv6);
            Tp F(s2 * k1 + s1 * k2), G(s3 * k1 + s2 * k2), H(s3 * k1 * k1 + s1 * k2 * k2 + s2 * k1 * k2 * Tp::raw(2));
            if (ar && m) {
                auto sub = calc(c, c - br - 1, ar, m - 1);
                auto f = sub.m_f, g = sub.m_g, h = sub.m_h;
                Tp df = Tp::raw(n) * Tp::raw(m) - f;
                F += df;
                Tp dg = s2 * Tp::raw(m) - (f + h) * m_inv2;
                G += dg;
                Tp dh = Tp::raw(n) * Tp::raw(m) * Tp::raw(m) + (k1 * dg + k2 * df - g) * Tp::raw(2) - f;
                H += dh;
            }
            return {F, G, H};
        }
    };
    uint64_t Euclidean_floor_sum(uint32_t a, uint32_t b, uint32_t c, uint32_t n) {
        uint64_t _a = a, _b = b, _c = c, _n = n + 1, ans{};
        while (true) {
            if (_a >= _c) ans += _n * (_n - 1) / 2 * (_a / _c), _a %= _c;
            if (_b >= _c) ans += _n * (_b / _c), _b %= _c;
            uint64_t y_max = _a * _n + _b;
            if (y_max < _c) break;
            _n = y_max / _c, _b = y_max % _c, std::swap(_c, _a);
        }
        return ans;
    }
}

#endif