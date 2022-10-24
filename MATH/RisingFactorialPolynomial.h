#ifndef __OY_RISINGFACTORIALPOLYNOMIAL__
#define __OY_RISINGFACTORIALPOLYNOMIAL__

#include "NTTPolynomial.h"

namespace OY {
    template <typename _Poly>
    struct RisingFactorialPolynomial {
        using rfpoly = RisingFactorialPolynomial<_Poly>;
        using _Tp = typename _Poly::value_type;
        static inline _Tp s_factorial[sizeof(_Poly::s_dftRoots) / sizeof(*_Poly::s_dftRoots)], s_factorialInv[sizeof(_Poly::s_dftRoots) / sizeof(*_Poly::s_dftRoots)];
        static inline uint32_t s_factorialSize = 0;
        _Poly m_coef;
        static void prepareFactorial(uint32_t __length) {
            if (s_factorialSize >= __length) return;
            if (!s_factorialSize) {
                s_factorial[0] = s_factorialInv[0] = s_factorial[1] = s_factorialInv[1] = _Tp(1);
                s_factorialSize = 2;
            }
            const auto P(_Tp::mod());
            for (uint32_t i = s_factorialSize; i < __length; i++) {
                auto q = P / i, r = P - q * i;
                s_factorial[i] = s_factorial[i - 1] * _Tp(i), s_factorialInv[i] = s_factorialInv[i - 1] * s_factorialInv[r] * s_factorial[r - 1] * _Tp(P - q);
            }
            s_factorialSize = std::max<uint32_t>(__length, 2);
        }
        static _Poly ex(uint32_t __length) { return prepareFactorial(__length), _Poly(s_factorialInv, s_factorialInv + __length); }
        static _Poly exInv(uint32_t __length) {
            _Poly res(ex(__length));
            for (uint32_t i = 1; i < res.size(); i += 2) res[i] = -res[i];
            return res;
        }
        static rfpoly fromPoints(_Poly __ys) {
            prepareFactorial(__ys.size());
            const uint32_t length = __ys.size();
            for (uint32_t i = 0; i < __ys.size(); i++) __ys[i] *= s_factorialInv[i];
            rfpoly res;
            (res.m_coef = __ys * exInv(__ys.size())).sizeTo(length);
            for (uint32_t i = 1; i < res.m_coef.size(); i += 2) res.m_coef[i] = -res.m_coef[i];
            return res;
        }
        static _Poly toPoints(const rfpoly &__a) { return __a.calcRange(__a.m_coef.size()); }
        static rfpoly fromNormal(const _Poly &__a) {
            _Poly xs(__a.size());
            for (uint32_t i = 0; i < xs.size(); i++) xs[i] = -_Tp(i);
            return fromPoints(__a.calc(xs));
        }
        static _Poly toNormal(const rfpoly &__a) {
            auto dfs = [&](auto self, uint32_t start, uint32_t length) -> std::pair<_Poly, _Poly> {
                std::pair<_Poly, _Poly> res;
                if (length == 1) {
                    res.first.push_back(__a.m_coef.at(start));
                    res.second.emplace_back(start);
                    res.second.emplace_back(1);
                } else {
                    auto [lf, lg] = self(self, start, length / 2);
                    auto [rf, rg] = self(self, start + length / 2, length / 2);
                    (res.first = rf * lg) += lf;
                    res.second = _Poly::product(lg, rg, length);
                    res.second[0]--;
                    res.second.emplace_back(1);
                }
                return res;
            };
            return dfs(dfs, 0, std::__bit_ceil(__a.m_coef.size())).first;
        }
        rfpoly &operator*=(const rfpoly &__other) {
            if (m_coef.empty()) return *this;
            if (__other.m_coef.empty()) {
                m_coef.clear();
                return *this;
            }
            const uint32_t length = m_coef.size() + __other.m_coef.size() - 1;
            _Poly ys(calcRange(length)), ys2(__other.calcRange(length));
            for (uint32_t i = 0; i < length; i++) ys[i] *= ys2[i];
            *this = fromPoints(ys);
            return *this;
        }
        friend rfpoly operator*(const rfpoly &__a, const rfpoly &__b) {
            rfpoly res(__a);
            res *= __b;
            return res;
        }
        _Poly calcRange(uint32_t __range) const {
            prepareFactorial(__range);
            _Poly a(m_coef);
            for (uint32_t i = 1; i < a.size(); i += 2) a[i] = -a[i];
            _Poly res(a * ex(__range));
            res.sizeTo(__range);
            for (uint32_t i = 0; i < __range; i++) res[i] *= s_factorial[i];
            return res;
        }
        _Tp calc(_Tp __x) const {
            _Tp cur(1), res(0);
            for (uint32_t i = 0; i < m_coef.size(); i++) res += cur * m_coef[i], cur *= __x + _Tp(i);
            return res;
        }
    };
}

#endif