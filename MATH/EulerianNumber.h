#ifndef __OY_EULERIANNUMBER__
#define __OY_EULERIANNUMBER__

#include "MTTPolynomial.h"
#include "NTTPolynomial.h"
#include "NTTPolynomial_ex.h"

namespace OY {
    template <typename _Poly>
    struct EulerianNumber {
        using _Tp = typename _Poly::value_type;
        std::vector<std::array<uint32_t, 2>> m_factors;
        std::vector<_Tp> m_factorial, m_factorialInv;
        EulerianNumber(uint32_t __n) : m_factors(__n + 1), m_factorial(__n + 2, _Tp(1)), m_factorialInv(__n + 2) {
            const uint32_t sqrt = std::sqrt(__n);
            for (uint32_t i = 4, j = 2; i <= __n; i += 2, j++) m_factors[i] = {2, j};
            for (uint32_t i = 3; i <= sqrt; i += 2)
                if (!m_factors[i][0])
                    for (uint32_t j = i * i, k = i; j <= __n; j += i * 2, k += 2) m_factors[j] = {i, k};
            for (uint32_t i = 1; i <= __n + 1; i++) m_factorial[i] = m_factorial[i - 1] * _Tp(i);
            m_factorialInv.back() = m_factorial.back().inv();
            for (uint32_t i = __n; ~i; i--) m_factorialInv[i] = m_factorialInv[i + 1] * _Tp(i + 1);
        }
        std::vector<_Poly> getEulerianTable(uint32_t __n) const {
            std::vector<_Poly> res(__n + 1);
            res[0] = {1};
            for (uint32_t i = 1; i <= __n; i++) {
                res[i].reserve(i + 1);
                res[i].push_back(0);
                for (uint32_t j = 1; j < i; j++) res[i].push_back(res[i - 1][j - 1] * (i - j + 1) + res[i - 1][j] * j);
                res[i].push_back(1);
            }
            return res;
        }
        _Poly getEulerianRow(uint32_t __row) const {
            _Poly f(__row), g(__row + 1);
            if (!__row) {
                f.push_back(1);
                return f;
            }
            for (uint32_t i = 0; i < __row; i++) f[i] = i % 2 ? -m_factorial[__row + 1] * m_factorialInv[i] * m_factorialInv[__row + 1 - i] : m_factorial[__row + 1] * m_factorialInv[i] * m_factorialInv[__row + 1 - i];
            for (uint32_t i = 1; i <= __row; i++)
                if (auto &[a, b] = m_factors[i]; a)
                    g[i] = g[a] * g[b];
                else
                    g[i] = _Tp(i).pow(__row);
            (f *= g).sizeTo(__row + 1);
            return f;
        }
    };
}

#endif