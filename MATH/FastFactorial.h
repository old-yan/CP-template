#ifndef __OY_FASTFACTORIAL__
#define __OY_FASTFACTORIAL__

#include <numeric>
#include "LagrangeInterpolation_ex.h"

namespace OY {
    template <typename _Poly>
    struct FastFactorial {
        using _Tp = typename _Poly::value_type;
        using _ModType = decltype(_Tp::mod());
        const _ModType m_block;
        _Poly m_presum;
        FastFactorial(_Tp __limit = _Tp::mod() - 1) : m_block(1 << (std::__countr_zero(std::__bit_ceil(__limit.val())) + 1) / 2) {
            _Tp blockInv(_Tp(m_block).inv());
            m_presum.reserve(m_block * 2);
            m_presum = {_Tp(1), _Tp(m_block + 1)};
            for (uint32_t cur = 1; cur < m_block; cur *= 2) {
                auto solver = LagrangeInterpolation_ex<_Poly>(cur * 2).prepare(m_presum);
                std::transform(m_presum.begin(), m_presum.end(), solver.calc(blockInv * cur).begin(), m_presum.begin(), std::multiplies<_Tp>());
                if (cur * 2 < m_block || __limit.val() > m_block * (cur * 2 + 50)) {
                    _Poly res1(solver.calc(_Tp(cur * 2))), res2(solver.calc((blockInv + 2) * cur));
                    std::copy(res1.begin(), std::transform(res1.begin(), res1.end(), res2.begin(), res1.begin(), std::multiplies<_Tp>()), m_presum.sizeTo(cur * 4).begin() + cur * 2);
                }
            }
            std::partial_sum(m_presum.begin(), m_presum.end(), m_presum.begin(), std::multiplies<_Tp>());
        }
        _Tp query(_Tp __n) const {
            _ModType m = std::min<_ModType>(__n.val() / m_block, m_presum.size()), r = __n.val() - m * m_block;
            _Tp res(1);
            if (m) res = m_presum[m - 1];
            for (_ModType i = m * m_block + 1, iend = __n.val(); i <= iend; i++) res *= i;
            return res;
        }
    };
}

#endif