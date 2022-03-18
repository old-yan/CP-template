#ifndef __OY_LUCAS__
#define __OY_LUCAS__

#include <cstdint>
#include <functional>
#include "Barrett.h"

namespace OY {
    struct Lucas {
        Barrett32 m_brt;
        Lucas(uint32_t __P) : m_brt(__P) {
            // assert(isPrime(__P));
        }
        uint32_t rawQuery(uint32_t __n, uint32_t __m) const {
            // assert(__n < m_brt.mod() && __m < m_brt.mod());
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            if (__m > __n - __m) __m = __n - __m;
            uint32_t a = 1, b = 1;
            for (uint32_t i = 1, j = __n - __m + 1; i <= __m; i++, j++) {
                a = m_brt.multiply_64(a, i);
                b = m_brt.multiply_64(b, j);
            }
            return m_brt.multiply_64(m_brt.pow(a, m_brt.mod() - 2), b);
        }
        uint32_t query(uint64_t __n, uint64_t __m) const {
            if (__n < __m) return 0;
            uint32_t res = 1;
            while (__m && __n != __m) {
                uint64_t n = __n / m_brt.mod(), m = __m / m_brt.mod();
                res = m_brt.multiply_64(res, rawQuery(__n - n * m_brt.mod(), __m - m * m_brt.mod()));
                __n = n;
                __m = m;
            }
            return res;
        }
    };
}

#endif