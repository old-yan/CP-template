#ifndef __OY_FWT__
#define __OY_FWT__

#include <algorithm>
#include <cstdint>

namespace OY {
    enum FWT_TYPE {
        FWT_BITXOR = 0,
        FWT_BITAND = 1,
        FWT_BITOR = 2
    };
    template <uint32_t _Tag>
    struct FWT {
        uint32_t m_length;
        FWT(uint32_t __length) : m_length(__length) {}
        template <typename _Iterator>
        void transform(_Iterator __iter) {
            for (uint32_t i = 1; i < m_length; i *= 2)
                for (uint32_t j = 0; j < m_length; j += i * 2)
                    for (uint32_t k = j; k < j + i; k++) {
                        auto x = __iter[k], y = __iter[k + i];
                        if constexpr (_Tag == FWT_BITXOR) {
                            __iter[k] += y;
                            __iter[k + i] = x - y;
                        } else if constexpr (_Tag == FWT_BITAND) {
                            __iter[k] += y;
                        } else if constexpr (_Tag == FWT_BITOR) {
                            __iter[k + i] += x;
                        }
                    }
        }
        template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
        void detransform(_Iterator __iter) {
            auto inv2 = _Tp(1) / 2;
            for (uint32_t i = 1; i < m_length; i *= 2)
                for (uint32_t j = 0; j < m_length; j += i * 2)
                    for (uint32_t k = j; k < j + i; k++) {
                        auto x = __iter[k], y = __iter[k + i];
                        if constexpr (_Tag == FWT_BITXOR) {
                            __iter[k] = (x + y) * inv2;
                            __iter[k + i] = (x - y) * inv2;
                        } else if constexpr (_Tag == FWT_BITAND) {
                            __iter[k] -= y;
                        } else if constexpr (_Tag == FWT_BITOR) {
                            __iter[k + i] -= x;
                        }
                    }
        }
    };
}

#endif