#ifndef __OY_STATICCATALANTABLE__
#define __OY_STATICCATALANTABLE__

#include <cstdint>
#include <functional>
#include "StaticModularInverseTable.h"

namespace OY {
    template <typename _ModType, _ModType _P>
    struct StaticCatalanTable {
        std::vector<_ModType> m_val;
        StaticModularInverseTable<_ModType, _P> m_invTable;
        StaticCatalanTable(uint32_t __n) : m_invTable(__n + 1) {
            m_val.reserve(__n + 1);
            m_val.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) {
                m_val.push_back(Modular<_ModType, _P>::multiply(Modular<_ModType, _P>::multiply(m_val.back(), i * 4 - 2), m_invTable.query(i + 1)));
            }
        }
        _ModType query(uint32_t __i) const { return m_val[__i]; }
    };
    template <uint32_t _P>
    using StaticCatalanTable32 = StaticCatalanTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCatalanTable64 = StaticCatalanTable<uint64_t, _P>;
}

#endif