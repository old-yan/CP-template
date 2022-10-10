#ifndef __OY_COMBINATIONTABLE__
#define __OY_COMBINATIONTABLE__

#include <numeric>

#include "StaticModInt.h"

namespace OY {
    template <typename _Tp>
    struct CombinationTable {
        std::vector<_Tp> m_factorial, m_factorialInv;
        CombinationTable(uint32_t __n) : m_factorial(__n + 1, _Tp(1)), m_factorialInv(__n + 1) {
            for (uint32_t i = 1; i <= __n; i++) m_factorial[i] = m_factorial[i - 1] * _Tp(i);
            m_factorialInv.back() = m_factorial.back().inv();
            for (uint32_t i = __n - 1; ~i; i--) m_factorialInv[i] = m_factorialInv[i + 1] * _Tp(i + 1);
        }
        _Tp comb(uint32_t __n, uint32_t __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return 1;
            return m_factorial[__n] * m_factorialInv[__m] * m_factorialInv[__n - __m];
        }
        _Tp perm(uint32_t __n) const { return m_factorial[__n]; }
        _Tp perm(uint32_t __n, uint32_t __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return m_factorial[__n];
            return m_factorial[__n] * m_factorialInv[__n - __m];
        }
        template <typename _Fp>
        _Tp perm(const std::vector<_Fp> &__ns) const {
            _Tp res(m_factorial[std::accumulate(__ns.begin(), __ns.end(), 0)]);
            for (_Fp n : __ns) res *= m_factorialInv[n];
            return res;
        }
        _Tp nonadjacentPerm(uint32_t __n, uint32_t __m) const {
            if (__n < __m) return 0;
            return comb(__n - __m + 1, __m);
        }
        _Tp staggeredPerm(uint32_t __n) const {
            static std::vector<_Tp> s_res{1, 0};
            while (s_res.size() < __n + 1) s_res.push_back((s_res[s_res.size() - 2] + s_res.back()) * _Tp(s_res.size() - 1));
            return s_res[__n];
        }
        _Tp circularPerm(uint32_t __n) const { return m_factorial[__n - 1]; }
        _Tp circularPerm(uint32_t __n, uint32_t __m) const {
            if (!__m) return 1;
            return comb(__n, __m) * m_factorial[__m - 1];
        }
    };
}

#endif