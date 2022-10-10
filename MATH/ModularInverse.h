#ifndef __OY_MODULARINVERSE__
#define __OY_MODULARINVERSE__

#include <numeric>
#include "StaticModInt.h"

namespace OY {
    template <typename _Tp, bool _IsPrime = false>
    struct ModularInverse {
        static _Tp query_Fermat(_Tp __a) {
            static_assert(_IsPrime);
            return __a.inv_Fermat();
        }
        static _Tp query_exgcd(_Tp __a) { return __a.inv_exgcd(); }
        static _Tp query(_Tp __a) {
            if constexpr (_IsPrime)
                return query_Fermat(__a);
            else
                return query_exgcd(__a);
        }
        template <typename _Iterator>
        static std::vector<_Tp> query(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first;
            std::vector<_Tp> res(__first, __last);
            std::partial_sum(res.begin(), res.end(), res.begin(), std::multiplies<_Tp>());
            _Tp inv(query(res.back()));
            for (uint32_t i = n - 1; i; i--) {
                res[i] = inv * res[i - 1];
                inv *= *(__first + i);
            }
            res[0] = inv;
            return res;
        }
    };
}

#endif