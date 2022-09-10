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
        template <typename _Tp>
        void transform(std::vector<_Tp> &__array) {
            __array.resize(m_length);
            for (uint32_t i = 1; i < m_length; i *= 2)
                for (uint32_t j = 0; j < m_length; j += i * 2)
                    for (uint32_t k = j; k < j + i; k++) {
                        _Tp x = __array[k], y = __array[k + i];
                        if constexpr (_Tag == FWT_BITXOR) {
                            __array[k] += y;
                            __array[k + i] = x - y;
                        } else if constexpr (_Tag == FWT_BITAND) {
                            __array[k] += y;
                        } else if constexpr (_Tag == FWT_BITOR) {
                            __array[k + i] += x;
                        }
                    }
        }
        template <typename _Tp>
        void detransform(std::vector<_Tp> &__array) {
            _Tp inv2 = 1 / _Tp(2);
            for (uint32_t i = 1; i < m_length; i *= 2)
                for (uint32_t j = 0; j < m_length; j += i*2)
                    for (uint32_t k = j; k < j + i; k++) {
                        _Tp x = __array[k], y = __array[k + i];
                        if constexpr (_Tag == FWT_BITXOR) {
                            __array[k] =(x+y)*inv2;
                            __array[k + i] = (x - y)*inv2;
                        } else if constexpr (_Tag == FWT_BITAND) {
                            __array[k] -= y;
                        } else if constexpr (_Tag == FWT_BITOR) {
                            __array[k+i] -= x;
                        }
                    }
        }
        template <typename _Tp>
        void calc(std::vector<_Tp> &__a, std::vector<_Tp> &__b) {
            transform(__a);
            transform(__b);
            for (uint32_t i = 0; i < m_length; i++) __a[i] *= __b[i];
            detransform(__a);
        }
    };
}

#endif