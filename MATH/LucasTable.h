#ifndef __OY_LUCASTABLE__
#define __OY_LUCASTABLE__

#include "StaticModInt.h"

namespace OY {
    template <typename _Tp>
    struct LucasTable {
        const uint32_t m_P;
        std::vector<_Tp> m_factorial, m_factorialInv;
        LucasTable() : m_P(_Tp::mod()), m_factorial(_Tp::mod(), _Tp(1)), m_factorialInv(_Tp::mod()) {
            for (uint32_t i = 1; i < m_factorial.size(); i++) m_factorial[i] = m_factorial[i - 1] * _Tp(i);
            m_factorialInv.back() = m_factorial.back().inv();
            for (uint32_t i = m_factorialInv.size() - 2; ~i; i--) m_factorialInv[i] = m_factorialInv[i + 1] * _Tp(i + 1);
        }
        _Tp rawQuery(uint32_t __n, uint32_t __m) const {
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            return m_factorial[__n] * m_factorialInv[__m] * m_factorialInv[__n - __m];
        }
        _Tp query(uint64_t __n, uint64_t __m) const {
            if (__n < __m) return 0;
            _Tp res(1);
            while (__m && __n != __m) {
                uint64_t nq = __n / m_P, nr = __n - nq * m_P, mq = __m / m_P, mr = __m - mq * m_P;
                res *= rawQuery(nr, mr);
                __n = nq, __m = mq;
            }
            return res;
        }
    };
}

#endif