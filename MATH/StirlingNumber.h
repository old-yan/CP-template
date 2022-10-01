#ifndef __OY_STIRLINGNUMBER__
#define __OY_STIRLINGNUMBER__

#include "Eratosthenes.h"
#include "NTTPolynomial.h"
#include "NTTPolynomial_ex.h"
#include "NTTPolynomial_mtt.h"

namespace OY {
    template <typename _Tp, uint32_t _N>
    struct SecondStirlingNumber {
        EratosthenesSieve<_N, false, false, true, false> m_sieve;
        _Tp m_factorialInv[_N + 1];
        SecondStirlingNumber() {
            m_factorialInv[0] = 1;
            m_factorialInv[1] = 1;
            for (long long _P = _Tp::mod(), i = 2; i <= _N; i++) {
                auto [q, r] = std::div(_P, i);
                m_factorialInv[i] = m_factorialInv[r] * (_P - q);
            }
            for (long long i = 2; i <= _N; i++) m_factorialInv[i] = m_factorialInv[i - 1] * m_factorialInv[i];
        }
        std::vector<std::vector<_Tp>> getSecondStirlingTable(uint32_t __n) const {
            std::vector<std::vector<_Tp>> res(__n + 1);
            for (uint32_t i = 0; i <= __n; i++) {
                res[i].reserve(i + 1);
                for (uint32_t j = 0; j < i; j++) res[i].push_back(j ? res[i - 1][j - 1] + j * res[i - 1][j] : 0);
                res[i].push_back(1);
            }
            return res;
        }
        template <typename _Poly>
        _Poly getSecondStirlingRow(uint32_t __n) const {
            _Tp pow[__n + 1];
            pow[0] = 0;
            pow[1] = 1;
            for (uint32_t i = 2; i <= __n; i++)
                if (m_sieve.isPrime(i))
                    pow[i] = _Tp(i).pow(__n);
                else {
                    uint32_t a = m_sieve.querySmallestFactor(i), b = i / a;
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