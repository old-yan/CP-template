/*
最后修改:
20231121
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MODULARINVERSERANGETABLE__
#define __OY_MODULARINVERSERANGETABLE__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename Tp, uint32_t MAX_RANGE>
    struct ModularInverseRangeTable {
        using mod_type = typename Tp::mod_type;
        Tp m_inverse[MAX_RANGE + 1];
        ModularInverseRangeTable(uint32_t range = MAX_RANGE) {
            m_inverse[1] = Tp::raw(1);
            const mod_type P = Tp::mod();
            for (uint32_t i = 2; i <= range; i++) {
                mod_type q = P / i, r = P - q * i;
                m_inverse[i] = m_inverse[r] * Tp::raw(q ? P - q : 0);
            }
        }
        Tp query(uint32_t x) const { return m_inverse[x]; }
        const Tp &operator[](uint32_t x) const { return m_inverse[x]; }
    };
}

#endif