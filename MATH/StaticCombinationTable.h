#ifndef __OY_STATICCONBINATIONTABLE__
#define __OY_STATICCONBINATIONTABLE__

#include <cstdint>
#include <functional>
#include <numeric>
#include "Modular.h"

namespace OY {
    template <typename _ModType, _ModType _P>
    struct StaticCombinationTable {
        std::vector<_ModType> m_factorial;
        std::vector<_ModType> m_factorialInv;
        StaticCombinationTable(uint32_t __n) {
            m_factorial.reserve(__n + 1);
            m_factorialInv.reserve(__n + 1);
            m_factorial.push_back(1);
            m_factorialInv.push_back(1);
            m_factorialInv.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) m_factorial.push_back(Modular<_ModType, _P>::multiply(m_factorial.back(), i));
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = _P / i, r = _P - q * i;
                m_factorialInv.push_back(Modular<_ModType, _P>::multiply(_P - q, m_factorialInv[r]));
            }
            for (uint32_t i = 1; i <= __n; i++) m_factorialInv[i] = Modular<_ModType, _P>::multiply(m_factorialInv[i - 1], m_factorialInv[i]);
        }
        _ModType comb(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return 1;
            return Modular<_ModType, _P>::multiply(m_factorial[__n], Modular<_ModType, _P>::multiply(m_factorialInv[__m], m_factorialInv[__n - __m]));
        }
        _ModType perm(_ModType __n) const { return m_factorial[__n]; }
        _ModType perm(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return m_factorial[__n];
            return Modular<_ModType, _P>::multiply(m_factorial[__n], m_factorialInv[__n - __m]);
        }
        template <typename _Tp>
        _ModType perm(const std::vector<_Tp> &__ns) const {
            _ModType res = m_factorial[std::accumulate(__ns.begin(), __ns.end(), _ModType(0))];
            for (_Tp n : __ns) res = Modular<_ModType, _P>::multiply(res, m_factorialInv[n]);
            return res;
        }
        _ModType nonadjacentPerm(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            return comb(__n - __m + 1, __m);
        }
        _ModType staggeredPerm(_ModType __n) const {
            static std::vector<_ModType> s_res{1, 0};
            while (s_res.size() < __n + 1) s_res.push_back(Modular<_ModType, _P>::multiply(s_res[s_res.size() - 2] + s_res.back(), s_res.size() - 1));
            return s_res[__n];
        }
        _ModType circularPerm(_ModType __n) const { return m_factorial[__n - 1]; }
        _ModType circularPerm(_ModType __n, _ModType __m) const {
            if (!__m) return 1;
            return Modular<_ModType, _P>::multiply(comb(__n, __m), m_factorial[__m - 1]);
        }
    };
    template <uint32_t _P>
    using StaticCombinationTable32 = StaticCombinationTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCombinationTable64 = StaticCombinationTable<uint64_t, _P>;
}

#endif