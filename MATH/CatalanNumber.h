#ifndef __OY_CATALANNUMBER__
#define __OY_CATALANNUMBER__

#include "InverseTable.h"

namespace OY {
    template <typename _Tp>
    std::vector<_Tp> CatalanNumber(uint32_t n) {
        std::vector<_Tp> invTable(getInverseTable<_Tp>(n + 1)), res;
        res.reserve(n + 1);
        res.emplace_back(1);
        for (uint32_t i = 1; i <= n; i++) res.push_back(res.back() * invTable[i + 1] * _Tp(i * 4 - 2));
        return res;
    }
}

#endif