#ifndef __OY_BELLNUMBER__
#define __OY_BELLNUMBER__

#include "NTTPolynomial.h"

namespace OY {
    template <typename _Poly>
    _Poly BellNumber(uint32_t n) {
        using _Tp = typename _Poly::value_type;
        _Poly res(n + 1);
        std::vector<_Tp> factorial(n + 1);
        for (uint32_t i = 0; i <= n; i++) factorial[i] = i ? factorial[i - 1] * _Tp(i) : _Tp(1);
        _Tp facinv = factorial[n].inv();
        for (uint32_t i = n; i; i--) res[i] = facinv, facinv *= _Tp(i);
        res = res.exponent();
        for (uint32_t i = 0; i <= n; i++) res[i] *= factorial[i];
        return res;
    }
}

#endif