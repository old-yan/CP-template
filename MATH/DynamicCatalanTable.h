#ifndef __OY_DYNAMICCATALANTABLE__
#define __OY_DYNAMICCATALANTABLE__

#include <cstdint>
#include <functional>
#include "DynamicModularInverseTable.h"

namespace OY {
    template <typename _ModType>
    struct DynamicCatalanTable {
        std::vector<_ModType> m_val;
        DynamicModularInverseTable<_ModType> m_invTable;
        Barrett<_ModType> m_brt;
        DynamicCatalanTable(_ModType __P, uint32_t __n) : m_invTable(__P, __n + 1), m_brt(__P) {
            m_val.reserve(__n + 1);
            m_val.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) {
                m_val.push_back(m_brt.multiply(m_brt.multiply(m_val.back(), i * 4 - 2), m_invTable.query(i + 1)));
            }
        }
        _ModType query(uint32_t __i) const { return m_val[__i]; }
    };
    using DynamicCatalanTable32 = DynamicCatalanTable<uint32_t>;
    using DynamicCatalanTable64 = DynamicCatalanTable<uint64_t>;
}

#endif