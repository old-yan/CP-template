#ifndef __OY_BOSTONMORI__
#define __OY_BOSTONMORI__

#include "NTTPolynomial.h"

namespace OY {
    template <typename _Poly, typename _Tp = typename _Poly::value_type>
    _Tp BostonMori_div(const _Poly &a, const _Poly &b, uint64_t n) {
        auto dfs = [&](auto self, _Poly f, _Poly g, uint64_t n) -> _Tp {
            if (f.empty()) return 0;
            if (!n) return f[0] / g[0];
            _Poly g2(g);
            for (uint32_t i = 1; i < g2.size(); i += 2) g2[i] = -g2[i];
            f *= g2, g *= g2;
            for (uint32_t i = n & 1; i < f.size(); i += 2) f[i / 2] = f[i];
            for (uint32_t i = 0; i < g.size(); i += 2) g[i / 2] = g[i];
            return self(self, f.sizeTo((f.size() + ((n & 1) ^ 1)) / 2), g.sizeTo((g.size() + 1) / 2), n / 2);
        };
        return dfs(dfs, a, b, n);
    }
    template <typename _Poly, typename _Tp = typename _Poly::value_type>
    _Tp BostonMori_fill(const _Poly &init, _Poly weight, uint64_t n) {
        weight.insert(weight.begin(), _Tp(-1));
        return BostonMori_div((init * weight).sizeTo(init.size()), weight, n);
    }
}

#endif