#ifndef __OY_LAGRANGEINTERPOLATIONEX__
#define __OY_LAGRANGEINTERPOLATIONEX__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    template <typename _Poly>
    struct LagrangeInterpolation_ex {
        using _Tp = typename _Poly::value_type;
        struct _Solver {
            uint32_t length;
            const _Poly ys;
            _Poly base, bi, bv;
            _Solver(const _Poly &_ys) : length(_ys.size()), ys(_ys), base(_ys.size()), bi(_ys.size() * 2 - 1), bv(_ys.size() * 2 - 1) {}
            _Poly rawCalc(_Tp __shift) {
                for (uint32_t i = 0; i < length * 2 - 1; i++) bi[i] = __shift - _Tp(length) + _Tp(1 + i);
                for (uint32_t i = 0; i < length * 2 - 1; i++) bv[i] = i ? bv[i - 1] * bi[i - 1] : _Tp(1);
                _Tp inv((bv.back() * bi.back()).inv());
                for (uint32_t i = length * 2 - 2; ~i; i--) bv[i] *= inv, inv *= bi[i];
                _Poly res(_Poly::product(base, bv, std::__bit_ceil(length * 2)));
                res.sizeTo(length * 2 - 1).erase(res.begin(), res.begin() + length - 1);
                _Tp v(1);
                for (uint32_t i = length - 1; ~i; i--) v *= __shift - _Tp(i);
                for (uint32_t i = 0; i < length; i++) res[i] *= v, v *= bv[i] * (__shift + _Tp(i + 1));
                return res;
            }
            _Poly calc(_Tp __shift) {
                if (__shift.val() < length) {
                    _Poly res(rawCalc(length));
                    std::rotate(res.begin(), res.begin() + __shift.val(), std::copy_n(ys.begin() + __shift.val(), length - __shift.val(), res.begin() + __shift.val()));
                    return res;
                } else if (__shift.val() > _Tp::mod() - length) {
                    _Poly res(rawCalc(-_Tp(length)));
                    std::rotate(res.begin(), std::copy_n(ys.begin(), length - (_Tp::mod() - __shift.val()), res.begin()), res.end());
                    return res;
                } else
                    return rawCalc(__shift);
            }
        };
        static inline _Tp s_factorial[sizeof(_Poly::s_dftRoots) / sizeof(*_Poly::s_dftRoots)], s_factorialInv[sizeof(_Poly::s_dftRoots) / sizeof(*_Poly::s_dftRoots)];
        static inline uint32_t s_factorialSize = 0;
        uint32_t m_length;
        _Poly m_av;
        LagrangeInterpolation_ex(uint32_t __length) : m_length(__length), m_av(__length) {
            prepareFactorial(m_length);
            for (uint32_t i = 0; i < m_length; i++) m_av[i] = s_factorialInv[i] * s_factorialInv[m_length - 1 - i];
        }
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
        _Solver prepare(const _Poly &__ys) {
            _Solver res(__ys);
            for (uint32_t i = 0; i < m_length; i++) res.base[i] = (m_length - 1 - i) % 2 ? -__ys[i] * m_av[i] : __ys[i] * m_av[i];
            return res;
        }
    };
}

#endif