#ifndef __OY_STATICMODULARINVERSE__
#define __OY_STATICMODULARINVERSE__

#include <cstdint>
#include <functional>
#include <numeric>
#include "Modular.h"

namespace OY {
    template <typename _ModType, _ModType _P, bool _IsPrime = false>
    struct StaticModularInverse {
        static constexpr _ModType query_Fermat(_ModType __a) {
            static_assert(_IsPrime);
            return Modular<_ModType, _P>::pow(__a, _P - 2);
        }
        static constexpr _ModType query_exgcd(_ModType __a) {
            _ModType x = _P, y = __a, m0 = 0, m1 = 1;
            while (y) {
                _ModType z = x / y;
                x -= y * z;
                m0 -= m1 * z;
                std::swap(x, y);
                std::swap(m0, m1);
            }
            if (m0 >= _P) m0 += _P / x;
            return m0;
        }
        static constexpr _ModType query(_ModType __a) {
            if constexpr (_IsPrime)
                return query_Fermat(__a);
            else
                return query_exgcd(__a);
        }
        template <typename _Iterator>
        static std::vector<_ModType> query(_Iterator __first, _Iterator __last) {
            int n = __last - __first;
            std::vector<_ModType> res(n);
            std::partial_sum(__first, __last, res.begin(), [&](_ModType a, _ModType b) { return Modular<_ModType, _P>::multiply(a, b); });
            _ModType inv = query(res.back());
            for (int i = n - 1; i; i--) {
                res[i] = Modular<_ModType, _P>::multiply(inv, res[i - 1]);
                inv = Modular<_ModType, _P>::multiply(inv, __first[i]);
            }
            res[0] = inv;
            return res;
        }
    };
    template <uint32_t _P, bool _IsPrime = false>
    using StaticModularInverse32 = StaticModularInverse<uint32_t, _P, _IsPrime>;
    template <uint64_t _P, bool _IsPrime = false>
    using StaticModularInverse64 = StaticModularInverse<uint64_t, _P, _IsPrime>;
}

#endif