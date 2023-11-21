#ifndef __OY_INVERSETABLE__
#define __OY_INVERSETABLE__

#include <algorithm>
#include <cassert>

#include "StaticModInt32.h"

namespace OY {
    template <typename _Tp>
    std::vector<_Tp> getInverseTable(uint32_t n) {
        std::vector<_Tp> res;
        res.reserve(n + 1);
        res.emplace_back(0);
        res.emplace_back(1);
        const auto P = _Tp::mod();
        for (uint32_t i = 2; i <= n; i++) {
            auto q = P / i, r = P - q * i;
            res.push_back(res[r] * _Tp(P - q));
        }
        return res;
    }
}

#endif