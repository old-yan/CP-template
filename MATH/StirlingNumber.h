#ifndef __OY_STIRLINGNUMBER__
#define __OY_STIRLINGNUMBER__

#include "MTTPolynomial.h"
#include "NTTPolynomial.h"
#include "NTTPolynomial_ex.h"

namespace OY {
    template <typename _Poly>
    struct SecondStirlingNumber {
        using _Tp = typename _Poly::value_type;
        std::vector<bool> m_isPrime;
        std::vector<uint32_t> m_smallestFactor;
        std::vector<_Tp> m_factorialInv;
        SecondStirlingNumber(uint32_t __n) : m_isPrime(__n + 1, true), m_smallestFactor(__n + 1), m_factorialInv(__n + 1) {
            m_isPrime[0] = m_isPrime[1] = false;
            m_smallestFactor[1] = 1;
            m_smallestFactor[2] = 2;
            const uint32_t sqrt = std::sqrt(__n);
            for (uint32_t i = 3; i <= sqrt; i += 2)
                if (m_isPrime[i]) {
                    m_smallestFactor[i] = i;
                    for (uint32_t j = i * i, k = i; j <= __n; j += i * 2, k += 2)
                        if (m_isPrime[j]) {
                            m_isPrime[j] = false;
                            m_smallestFactor[j] = i;
                        }
                }
            for (uint32_t i = sqrt + sqrt % 2 + 1; i <= __n; i += 2)
                if (m_isPrime[i]) m_smallestFactor[i] = i;
            m_factorialInv[0] = _Tp(1);
            m_factorialInv[1] = _Tp(1);
            const auto P(_Tp::mod());
            for (uint32_t i = 2; i <= __n; i++) {
                auto q = P / i, r = P - q * i;
                m_factorialInv[i] = m_factorialInv[r] * _Tp(P - q);
            }
            for (long long i = 2; i <= __n; i++) m_factorialInv[i] = m_factorialInv[i - 1] * m_factorialInv[i];
        }
        std::vector<_Poly> getSecondStirlingTable(uint32_t __n) const {
            std::vector<_Poly> res(__n + 1);
            for (uint32_t i = 0; i <= __n; i++) {
                res[i].reserve(i + 1);
                for (uint32_t j = 0; j < i; j++) res[i].push_back(j ? res[i - 1][j - 1] + j * res[i - 1][j] : _Tp(0));
                res[i].emplace_back(1);
            }
            return res;
        }
        _Poly getSecondStirlingRow(uint32_t __n) const {
            _Tp pow[__n + 1];
            pow[0] = _Tp(0);
            pow[1] = _Tp(1);
            for (uint32_t i = 2; i <= __n; i++)
                if (m_isPrime[i])
                    pow[i] = _Tp(i).pow(__n);
                else {
                    uint32_t a = m_smallestFactor[i], b = i / a;
                    pow[i] = pow[a] * pow[b];
                }
            _Poly a(__n + 1), b(__n + 1);
            for (uint32_t i = 0; i <= __n; i++) {
                a[i] = pow[i] * m_factorialInv[i];
                b[i] = i % 2 ? -m_factorialInv[i] : m_factorialInv[i];
            }
            (a *= b).sizeTo(__n + 1);
            return a;
        }
    };
}

#endif