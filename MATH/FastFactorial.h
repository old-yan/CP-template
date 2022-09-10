#ifndef __OY_FASTFACTORIAL__
#define __OY_FASTFACTORIAL__

#include "LagrangeInterpolation_ex.h"

namespace OY {
    template <typename _Tp, typename _Poly>
    struct FastFactorial {
        uint32_t m_block;
        _Poly m_presum;
        FastFactorial(_Tp __limit = _Tp::mod() - 1) {
            m_block = 1 << (std::__countr_zero(std::__bit_ceil(__limit.val()))) / 2;
            _Tp blockInv = _Tp(m_block).inv();
            m_presum.reserve(m_block * 2);
            m_presum = {1, m_block + 1};
            for (uint32_t cur = 1; cur < m_block; cur *= 2) {
                OY::LagrangeInterpolation_ex L(m_presum);
                _Poly::transform(m_presum.begin(), L.calc(blockInv * cur).begin(), cur * 2, std::multiplies<_Tp>());
                if (cur * 2 < m_block || __limit.val() > m_block * (cur * 2 + 50)) {
                    m_presum.resize(cur * 4);
                    std::copy_n(_Poly::transform(L.calc(_Tp(cur) * 2).begin(), L.calc((blockInv + 2) * cur).begin(), cur * 2, std::multiplies<_Tp>()), cur * 2, m_presum.begin() + cur * 2);
                }
            }
            std::partial_sum(m_presum.begin(), m_presum.end(), m_presum.begin(), std::multiplies<_Tp>());
        }
        _Tp query(_Tp __n) {
            _Tp res = 1;
            uint32_t m = std::min<uint32_t>(__n.val() / m_block, m_presum.size());
            if (m) res = m_presum[m - 1];
            for (uint32_t i = m * m_block + 1; i <= __n.val(); i++) res *= i;
            return res;
        }
    };
}

#endif