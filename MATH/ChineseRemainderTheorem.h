#ifndef __OY_CHINESEREMAINDERTHEOREM__
#define __OY_CHINESEREMAINDERTHEOREM__

#include <cstdint>
#include <functional>
#include "Barrett.h"
#include "DynamicModularInverse.h"
#include "ExGCD.h"

namespace OY {
    class ChineseRemainderTheorem {
        struct node {
            uint64_t r, d;
        };
        std::vector<node> m_sub;
        uint64_t m_prod;

    public:
        ChineseRemainderTheorem(uint32_t __n = 0) : m_prod{1} { m_sub.reserve(__n); }
        template <bool _Check = false>
        std::conditional_t<_Check, bool, void> add(uint64_t __r, uint64_t __d) {
            m_sub.push_back({__r, __d});
            m_prod *= __d;
            if constexpr (_Check) return std::gcd(m_prod, __d) == 1;
        }
        uint64_t query() const {
            uint64_t res = 0;
            Barrett64 brt(m_prod);
            for (auto [r, d] : m_sub) {
                uint64_t rest = m_prod / d, inv = DynamicModularInverse64(d).query_exgcd(rest);
                res = brt.plus(res, brt.multiply_ld(brt.multiply_ld(r, rest), inv));
            }
            return res;
        }
    };
    class ChineseRemainderTheorem_ex {
        uint64_t m_rem, m_div;

    public:
        ChineseRemainderTheorem_ex() : m_rem{0}, m_div{1} {}
        bool add(uint64_t __r, uint64_t __d) {
            auto [g, res, k1] = exGCD(m_div, __d, __r - m_rem);
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