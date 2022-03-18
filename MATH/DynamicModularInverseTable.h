#ifndef __OY_DYNAMICMODULARINVERSETABLE__
#define __OY_DYNAMICMODULARINVERSETABLE__

#include <cstdint>
#include "Barrett.h"

namespace OY {
    template <typename _ModType>
    class DynamicModularInverseTable {
        std::vector<_ModType> m_inv;
        Barrett<_ModType> m_brt;

    public:
        DynamicModularInverseTable(_ModType __P, uint32_t __n) : m_brt(__P) {
            // assert(OY::isPrime<_ModType>(__P));
            if (__n >= __P) __n = __P - 1;
            m_inv.reserve(__n + 1);
            m_inv.push_back(0);
            m_inv.push_back(1);
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = __P / i, r = __P - q * i;
                m_inv.push_back(m_brt.multiply(__P - q, m_inv[r]));
            }
        }
        _ModType query(uint32_t __a) const {
            // assert(__a<m_brt.mod());
            return m_inv[__a];
        }
    };
    using DynamicModularInverseTable32 = DynamicModularInverseTable<uint32_t>;
    using DynamicModularInverseTable64 = DynamicModularInverseTable<uint64_t>;
}

#endif