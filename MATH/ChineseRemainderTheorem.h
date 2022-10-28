#ifndef __OY_CHINESEREMAINDERTHEOREM__
#define __OY_CHINESEREMAINDERTHEOREM__

#include <cstdint>
#include <functional>
#include "Barrett.h"
#include "ExtendedEuclidean.h"

namespace OY {
    struct ChineseRemainderTheorem {
        struct node {
            uint64_t r, d;
        };
        std::vector<node> m_sub;
        uint64_t m_prod;
        ChineseRemainderTheorem(uint32_t __n = 0) : m_prod{1} { m_sub.reserve(__n); }
        void add(uint64_t __r, uint64_t __d) {
            if (__d > 1) {
                m_sub.push_back({__r, __d});
                m_prod *= __d;
            }
        }
        static uint64_t getInv(uint64_t __a, uint64_t __p) {
            uint64_t x = __p, y = __a, m0 = 0, m1 = 1;
            while (y) {
                uint64_t z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= __p) m0 += __p / x;
            return m0;
        }
        uint64_t query() const {
            uint64_t res = 0;
            Barrett64 brt(m_prod);
            for (auto &[r, d] : m_sub) {
                uint64_t rest = m_prod / d, inv = getInv(rest, d);
                res = brt.plus(res, brt.multiply_ld(brt.multiply_ld(r, rest), inv));
            }
            return res;
        }
    };
    struct ChineseRemainderTheorem_ex {
        uint64_t m_rem, m_div;
        ChineseRemainderTheorem_ex() : m_rem{0}, m_div{1} {}
        bool add(uint64_t __r, uint64_t __d) {
            auto [g, res, k1] = ExtendedEuclidean(m_div, __d, __r - m_rem);
            if (res != __r - m_rem) return false;
            m_rem += m_div * k1;
            m_div = m_div / g * __d;
            m_rem %= m_div;
            return true;
        }
        uint64_t query() const { return m_rem; }
    };
}

#endif