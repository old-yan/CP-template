#ifndef __OY_STATICMODULARINVERSETABLE__
#define __OY_STATICMODULARINVERSETABLE__

#include <cstdint>
#include "Modular.h"

namespace OY {
    template <typename _ModType, _ModType _P>
    class StaticModularInverseTable {
        std::vector<_ModType> m_inv;

    public:
        StaticModularInverseTable(uint32_t __n) {
            // assert(OY::isPrime<_ModType>(__P));
            if (__n >= _P) __n = _P - 1;
            m_inv.reserve(__n + 1);
            m_inv.push_back(0);
            m_inv.push_back(1);
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = _P / i, r = _P - q * i;
                m_inv.push_back(Modular<_ModType, _P>::multiply(_P - q, m_inv[r]));
            }
        }
        _ModType query(uint32_t __a) const {
            // assert(__a<m_brt.mod());
            return m_inv[__a];
        }
    };
    template <uint32_t _P>
    using StaticModularInverseTable32 = StaticModularInverseTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticModularInverseTable64 = StaticModularInverseTable<uint64_t, _P>;
}

#endif