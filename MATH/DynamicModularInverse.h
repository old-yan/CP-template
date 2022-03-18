#ifndef __OY_DYNAMICMODULARINVERSE__
#define __OY_DYNAMICMODULARINVERSE__

#include <cstdint>
#include <functional>
#include <numeric>
#include "Montgomery.h"

namespace OY {
    template <typename _ModType>
    struct DynamicModularInverse {
        Montgomery<_ModType> m_mg;
        bool m_isPrime;
        constexpr DynamicModularInverse(_ModType __P, bool __isPrime = false) : m_mg(__P), m_isPrime(__isPrime) {}
        constexpr _ModType query_Fermat(_ModType __a) const {
            assert(m_isPrime);
            if constexpr (std::is_same_v<_ModType, uint64_t>)
                return m_mg.reduce(m_mg.pow(m_mg.init(__a), m_mg.mod() - 2));
            else
                return m_mg.m_brt.pow_64(__a, m_mg.mod() - 2);
        }
        constexpr _ModType query_exgcd(_ModType __a) const {
            _ModType x = m_mg.mod(), y = __a, m0 = 0, m1 = 1;
            while (y) {
                _ModType z = x / y;
                x -= y * z;
                m0 -= m1 * z;
                std::swap(x, y);
                std::swap(m0, m1);
            }
            if (m0 >= m_mg.mod()) m0 += m_mg.mod() / x;
            return m0;
        }
        constexpr _ModType query(_ModType __a) const {
            if (m_isPrime)
                return query_Fermat(__a);
            else
                return query_exgcd(__a);
        }
        template <typename _Iterator>
        std::vector<_ModType> query(_Iterator __first, _Iterator __last) const {
            int n = __last - __first;
            std::vector<_ModType> res(n);
            std::partial_sum(__first, __last, res.begin(), [&](_ModType a, _ModType b) { return m_mg.m_brt.multiply(a, b); });
            _ModType inv = query(res.back());
            for (int i = n - 1; i; i--) {
                res[i] = m_mg.m_brt.multiply(inv, res[i - 1]);
                inv = m_mg.m_brt.multiply(inv, __first[i]);
            }
            res[0] = inv;
            return res;
        }
    };
    using DynamicModularInverse32 = DynamicModularInverse<uint32_t>;
    using DynamicModularInverse64 = DynamicModularInverse<uint64_t>;
}

#endif