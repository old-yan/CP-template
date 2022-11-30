#ifndef __OY_MONTGOMERY__
#define __OY_MONTGOMERY__

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "Barrett.h"

namespace OY {
    template <typename _ModType>
    struct _MontgomeryTag;
    template <>
    struct _MontgomeryTag<uint32_t> {
        using long_type = uint64_t;
        static constexpr uint32_t limit = (1u << 31) - 1;
        static constexpr uint32_t inv_loop = 4;
        static constexpr uint32_t length = 32;
    };
    template <>
    struct _MontgomeryTag<uint64_t> {
        using long_type = __uint128_t;
        static constexpr uint64_t limit = (1ull << 63) - 1;
        static constexpr uint32_t inv_loop = 5;
        static constexpr uint32_t length = 64;
    };
    template <typename _ModType>
    struct Montgomery {
        using _FastType = _ModType;
        using _LongType = typename _MontgomeryTag<_ModType>::long_type;
        _ModType m_P;
        _ModType m_Pinv;
        _ModType m_Ninv;
        Barrett<_ModType> m_brt;
        constexpr Montgomery() = default;
        constexpr explicit Montgomery(_ModType __P) : m_P(__P), m_Pinv(__P), m_Ninv(-_LongType(__P) % __P), m_brt(__P) {
            // assert((__P & 1) && __P > 1 && __P <= _MontgomeryTag<_ModType>::limit);
            for (int i = 0; i < _MontgomeryTag<_ModType>::inv_loop; i++) m_Pinv *= _ModType(2) - __P * m_Pinv;
        }
        constexpr _ModType mod() const { return m_brt.mod(); }
        constexpr _ModType mod(uint64_t __a) const { return m_brt.mod(__a); }
        constexpr _FastType init(uint64_t __a) const { return reduce(_LongType(mod(__a)) * m_Ninv); }
        constexpr _FastType raw_init(uint64_t __a) const { return reduce(_LongType(__a) * m_Ninv); }
        constexpr _FastType reduce(_LongType __a) const {
            _FastType res = (__a >> _MontgomeryTag<_ModType>::length) - _ModType(_LongType(_ModType(__a) * m_Pinv) * m_P >> _MontgomeryTag<_ModType>::length);
            if (res >= mod()) res += mod();
            return res;
        }
        constexpr _ModType reduce(_FastType __a) const {
            _ModType res = -_ModType(_LongType(__a * m_Pinv) * m_P >> _MontgomeryTag<_ModType>::length);
            if (res >= mod()) res += mod();
            return res;
        }
        constexpr _FastType plus(_FastType __a, _FastType __b) const {
            if (__a += __b; __a >= m_P) __a -= m_P;
            return __a;
        }
        constexpr _FastType minus(_FastType __a, _FastType __b) const {
            if (__a += m_P - __b; __a >= m_P) __a -= m_P;
            return __a;
        }
        constexpr _FastType multiply(_FastType __a, _FastType __b) const { return reduce(_LongType(__a) * __b); }
        constexpr _FastType pow(_FastType __a, uint64_t __n) const {
            _FastType res = reduce(_LongType(1) * m_Ninv);
            while (__n) {
                if (__n & 1) res = multiply(res, __a);
                __a = multiply(__a, __a);
                __n >>= 1;
            }
            return res;
        }
        template <typename _Tp>
        constexpr _Tp divide(_Tp __a) const { return m_brt.divide(__a); }
        template <typename _Tp>
        constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) const { return m_brt.divmod(__a); }
    };
    using Montgomery32 = Montgomery<uint32_t>;
    using Montgomery64 = Montgomery<uint64_t>;
}

#endif