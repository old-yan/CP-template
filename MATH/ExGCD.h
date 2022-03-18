#ifndef __OY_EXGCD__
#define __OY_EXGCD__

#include <cstdint>
#include <tuple>
#include "Barrett.h"

namespace OY {
    constexpr auto exGCD(int64_t a, int64_t b) {
        struct _exGCD_ans {
            int64_t g, k1, k2;
        };
        if (!a && !b) return _exGCD_ans{0, 0, 0};
        int64_t s = b, t = a, m0 = 0, m1 = 1, m2 = 1, m3 = 0;
        while (t) {
            int64_t u = s / t;
            std::tie(s, m0, m2) = std::make_tuple(s - t * u, m0 - m1 * u, m2 - m3 * u);
            std::swap(s, t);
            std::swap(m0, m1);
            std::swap(m2, m3);
        }
        if (s < 0) std::tie(s, m0, m2) = std::make_tuple(-s, -m0, -m2);
        return _exGCD_ans{s, m0, m2};
    }
    constexpr auto exGCD(int64_t a, int64_t b, int64_t c) {
        struct _exGCD_ans {
            int64_t g, res, k1;
        };
        if (!a) return b && c % b == 0 ? _exGCD_ans{b, c, 0} : _exGCD_ans{b, 0, 0};
        if (!b) return a && c % a == 0 ? _exGCD_ans{a, c, c / a} : _exGCD_ans{a, 0, 0};
        int64_t s = b, t = a, m0 = 0, m1 = 1;
        while (t) {
            int64_t u = s / t;
            std::tie(s, m0) = std::make_tuple(s - t * u, m0 - m1 * u);
            std::swap(s, t);
            std::swap(m0, m1);
        }
        if (c % s) return _exGCD_ans{std::abs(s), 0, 0};
        std::tie(a, b) = std::make_tuple(a / s, b / s);
        if (b < 0) std::tie(a, b) = std::make_tuple(-a, -b);
        if (b <= UINT32_MAX) {
            Barrett32 brt(b);
            if (m0 >= 0)
                m0 = brt.mod(m0);
            else {
                m0 = brt.mod(-m0);
                if (m0) m0 = b - m0;
            }
            int64_t x = c / s;
            if (x >= 0)
                x = brt.mod(x);
            else {
                x = brt.mod(-x);
                if (x) x = b - x;
            }
            m0 = brt.multiply_64(m0, x);
        } else {
            Barrett64 brt(b);
            if (m0 >= 0)
                m0 = brt.mod(m0);
            else {
                m0 = brt.mod(-m0);
                if (m0) m0 = b - m0;
            }
            int64_t x = c / s;
            if (x >= 0)
                x = brt.mod(x);
            else {
                x = brt.mod(-x);
                if (x) x = b - x;
            }
            m0 = brt.multiply_ld(m0, x);
        }
        return _exGCD_ans{std::abs(s), c, m0};
    }
}

#endif