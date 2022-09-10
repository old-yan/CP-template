#ifndef __OY_LAGRANGEINTERPOLATIONEX__
#define __OY_LAGRANGEINTERPOLATIONEX__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Poly>
    struct LagrangeInterpolation_ex {
        using _Tp = typename _Poly::value_type;
        uint32_t m_length;
        _Poly m_ys, m_base, m_bi, m_bv;
        LagrangeInterpolation_ex(const _Poly &__ys) : m_length(__ys.size()), m_ys(__ys), m_base(__ys.size()), m_bi(__ys.size() * 2 - 1), m_bv(__ys.size() * 2 - 1) {
            _Tp fac[m_length], ai[m_length], av[m_length];
            for (uint32_t i = 0; i < m_length; i++) fac[i] = i ? fac[i - 1] * i : 1;
            for (uint32_t i = 0; i < m_length; i++) ai[i] = fac[i] * fac[m_length - 1 - i];
            for (uint32_t i = 0; i < m_length; i++) av[i] = i ? av[i - 1] * ai[i - 1] : 1;
            _Tp inv = (av[m_length - 1] * ai[m_length - 1]).inv();
            for (uint32_t i = m_length - 1; ~i; i--) av[i] *= inv, inv *= ai[i];
            for (uint32_t i = 0; i < m_length; i++) m_base[i] = (m_length - 1 - i) % 2 ? -__ys[i] * av[i] : __ys[i] * av[i];
        }
        _Poly rawCalc(_Tp __shift) {
            for (uint32_t i = 0; i < m_length * 2 - 1; i++) m_bi[i] = __shift - m_length + 1 + i;
            for (uint32_t i = 0; i < m_length * 2 - 1; i++) m_bv[i] = i ? m_bv[i - 1] * m_bi[i - 1] : 1;
            _Tp inv = (m_bv.back() * m_bi.back()).inv();
            for (uint32_t i = m_length * 2 - 2; ~i; i--) m_bv[i] *= inv, inv *= m_bi[i];
            _Poly res = _Poly::product(m_base, m_bv, std::__bit_ceil(m_length * 2));
            res.sizeTo(m_length * 2 - 1).erase(res.begin(), res.begin() + m_length - 1);
            _Tp v = 1;
            for (uint32_t i = m_length - 1; ~i; i--) v *= __shift - i;
            for (uint32_t i = 0; i < m_length; i++) res[i] *= v, v *= m_bv[i] * (i + __shift + 1);
            return res;
        }
        _Poly calc(_Tp __shift) {
            if (__shift.val() < m_length) {
                _Poly res = rawCalc(m_length);
                std::rotate(res.begin(), res.begin() + __shift.val(), std::copy_n(m_ys.begin() + __shift.val(), m_length - __shift.val(), res.begin() + __shift.val()));
                return res;
            } else if (__shift.val() > _Tp::mod() - m_length) {
                _Poly res = rawCalc(-_Tp(m_length));
                std::rotate(res.begin(), std::copy_n(m_ys.begin(), m_length - (_Tp::mod() - __shift.val()), res.begin()), res.end());
                return res;
            } else
                return rawCalc(__shift);
        }
    };
}

#endif