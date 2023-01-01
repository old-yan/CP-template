#ifndef __OY_STIRLINGNUMBER__
#define __OY_STIRLINGNUMBER__

#include "MTTPolynomial.h"
#include "NTTPolynomial.h"
#include "NTTPolynomial_ex.h"

namespace OY {
    template <typename _Poly>
    struct StirlingNumber {
        using _Tp = typename _Poly::value_type;
        std::vector<std::array<uint32_t, 2>> m_factors;
        std::vector<_Tp> m_factorial, m_factorialInv;
        StirlingNumber(uint32_t __n) : m_factors(__n + 1), m_factorial(__n + 1, _Tp(1)), m_factorialInv(__n + 1) {
            const uint32_t sqrt = std::sqrt(__n);
            for (uint32_t i = 4, j = 2; i <= __n; i += 2, j++) m_factors[i] = {2, j};
            for (uint32_t i = 3; i <= sqrt; i += 2)
                if (!m_factors[i][0])
                    for (uint32_t j = i * i, k = i; j <= __n; j += i * 2, k += 2) m_factors[j] = {i, k};
            for (uint32_t i = 1; i <= __n; i++) m_factorial[i] = m_factorial[i - 1] * _Tp(i);
            m_factorialInv.back() = m_factorial.back().inv();
            for (uint32_t i = __n - 1; ~i; i--) m_factorialInv[i] = m_factorialInv[i + 1] * _Tp(i + 1);
        }
        std::vector<_Poly> getFirstStirlingTable(uint32_t __n) const {
            std::vector<_Poly> res(__n + 1);
            for (uint32_t i = 0; i <= __n; i++) {
                res[i].reserve(i + 1);
                for (uint32_t j = 0; j < i; j++) res[i].push_back(j ? res[i - 1][j - 1] + _Tp(i - 1) * res[i - 1][j] : _Tp(0));
                res[i].emplace_back(1);
            }
            return res;
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
        _Poly getFirstStirlingRow(uint32_t __row) const {
            _Poly res{_Tp(0), _Tp(1)};
            if (!__row) {
                res.pop_back();
                return res;
            }
            for (uint32_t i = std::__countr_zero(std::__bit_floor(__row)) - 1; ~i; i--) {
                _Poly a(res), b(res.size());
                for (uint32_t j = 0; j < b.size(); j++) b[j] = j ? b[j - 1] * _Tp(a.size() - 1) : 1;
                for (uint32_t j = 0; j < b.size(); j++) b[j] *= m_factorialInv[j];
                for (uint32_t j = 0; j < a.size(); j++) a[j] *= m_factorial[j];
                (a.reverse() *= b).sizeTo(b.size()).reverse();
                for (uint32_t j = 0; j < a.size(); j++) a[j] *= m_factorialInv[j];
                if (__row >> i & 1)
                    for (uint32_t j = a.sizeTo(a.size() + 1).size() - 1, k = b.size() * 2 - 2; ~j; j--) (a[j] *= k) += a.at(j - 1);
                res *= a;
            }
            return res;
        }
        _Poly getSecondStirlingRow(uint32_t __row) const {
            _Tp pow[__row + 1];
            pow[0] = _Tp(0), pow[1] = _Tp(1);
            for (uint32_t i = 2; i <= __row; i++)
                if (auto &[a, b] = m_factors[i]; a)
                    pow[i] = pow[a] * pow[b];
                else
                    pow[i] = _Tp(i).pow(__row);
            _Poly a(__row + 1), b(__row + 1);
            for (uint32_t i = 0; i <= __row; i++) a[i] = pow[i] * m_factorialInv[i], b[i] = i % 2 ? -m_factorialInv[i] : m_factorialInv[i];
            (a *= b).sizeTo(__row + 1);
            return a;
        }
        _Poly getFirstStirlingCol(uint32_t __col, uint32_t __maxRow) const {
            if (!__col) {
                _Poly res(__maxRow + 1);
                res[0] = _Tp(1);
                return res;
            }
            if (__col == 1) {
                _Poly res(__maxRow + 1);
                res[__col] = _Tp(1);
                for (uint32_t i = __col + 1; i < res.size(); i++) res[i] = res[i - 1] * _Tp(i - __col);
                return res;
            }
            if (__maxRow < __col) return _Poly(__maxRow + 1);
            _Poly a(__maxRow);
            for (uint32_t i = 0; i < __maxRow; i++) a[i] = m_factorial[i] * m_factorialInv[i + 1];
            _Poly b(_Poly::_simplePow(a, _Tp(__col))), res(__maxRow + 1);
            for (uint32_t i = __col; i <= __maxRow; i++) res[i] = b[i - __col] * m_factorial[i] * m_factorialInv[__col];
            return res;
        }
        _Poly getSecondStirlingCol(uint32_t __col, uint32_t __maxRow) const {
            if (!__col) {
                _Poly res(__maxRow + 1);
                res[0] = _Tp(1);
                return res;
            }
            if (__col == 1) return _Poly(__maxRow + 1, _Tp(1));
            if (__maxRow < __col) return _Poly(__maxRow + 1);
            _Poly a(m_factorialInv.begin() + 1, m_factorialInv.begin() + (__maxRow - __col + 2)), b(_Poly::_simplePow(a, _Tp(__col))), res(__maxRow + 1);
            for (uint32_t i = __col; i <= __maxRow; i++) res[i] = b[i - __col] * m_factorial[i] * m_factorialInv[__col];
            return res;
        }
    };
}

#endif