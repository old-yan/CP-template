#ifndef __OY_DYNAMICCONBINATIONTABLE__
#define __OY_DYNAMICCONBINATIONTABLE__

#include <cstdint>
#include <functional>
#include <numeric>
#include "Barrett.h"

namespace OY {
    template <typename _ModType>
    struct DynamicCombinationTable {
        Barrett<_ModType> m_brt;
        std::vector<_ModType> m_factorial;
        std::vector<_ModType> m_factorialInv;
        DynamicCombinationTable(_ModType __P, uint32_t __n) : m_brt(__P) {
            m_factorial.reserve(__n + 1);
            m_factorialInv.reserve(__n + 1);
            m_factorial.push_back(1);
            m_factorialInv.push_back(1);
            m_factorialInv.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) m_factorial.push_back(m_brt.multiply(m_factorial.back(), i));
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = __P / i, r = __P - q * i;
                m_factorialInv.push_back(m_brt.multiply(__P - q, m_factorialInv[r]));
            }
            for (uint32_t i = 1; i <= __n; i++) m_factorialInv[i] = m_brt.multiply(m_factorialInv[i - 1], m_factorialInv[i]);
        }
        _ModType comb(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return 1;
            return m_brt.multiply(m_factorial[__n], m_brt.multiply(m_factorialInv[__m], m_factorialInv[__n - __m]));
        }
        _ModType perm(_ModType __n) const { return m_factorial[__n]; }
        _ModType perm(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return m_factorial[__n];
            return m_brt.multiply(m_factorial[__n], m_factorialInv[__n - __m]);
        }
        template <typename _Tp>
        _ModType perm(const std::vector<_Tp> &__ns) const {
            _ModType res = m_factorial[std::accumulate(__ns.begin(), __ns.end(), _ModType(0))];
            for (_Tp n : __ns) res = m_brt.multiply(res, m_factorialInv[n]);
            return res;
        }
        _ModType nonadjacentPerm(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            return comb(__n - __m + 1, __m);
        }
        _ModType staggeredPerm(_ModType __n) const {
            static std::vector<_ModType> s_res{1, 0};
            while (s_res.size() < __n + 1) s_res.push_back(m_brt.multiply(s_res[s_res.size() - 2] + s_res.back(), s_res.size() - 1));
            return s_res[__n];
        }
        _ModType circularPerm(_ModType __n) const { return m_factorial[__n - 1]; }
        _ModType circularPerm(_ModType __n, _ModType __m) const {
            if (!__m) return 1;
            return m_brt.multiply(comb(__n, __m), m_factorial[__m - 1]);
        }
    };
    using DynamicCombinationTable32 = DynamicCombinationTable<uint32_t>;
    using DynamicCombinationTable64 = DynamicCombinationTable<uint64_t>;
}

#endif