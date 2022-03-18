#ifndef __OY_STATICLUCAS__
#define __OY_STATICLUCAS__

#include <cstdint>
#include <functional>
#include "Modular.h"

namespace OY {
    template <uint32_t _P>
    struct StaticLucasTable {
        static inline uint32_t s_factorial[_P];
        static inline uint32_t s_factorialInverse[_P];
        static inline bool s_inited = false;
        static void init() {
            s_factorial[0] = 1;
            for (uint32_t i = 1; i < _P; i++) s_factorial[i] = Modular32<_P>::multiply_64(s_factorial[i - 1], i);
            s_factorialInverse[_P - 1] = Modular32<_P>::pow_64(s_factorial[_P - 1], _P - 2);
            for (uint32_t i = _P - 1; i; i--) s_factorialInverse[i - 1] = Modular32<_P>::multiply_64(s_factorialInverse[i], i);
            s_inited = true;
        }
        static uint32_t rawQuery(uint32_t __n, uint32_t __m) {
            // assert(__n < _P && __m < _P);
            if (!s_inited) init();
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            return Modular32<_P>::multiply_64(s_factorial[__n], Modular32<_P>::multiply_64(s_factorialInverse[__m], s_factorialInverse[__n - __m]));
        }
        static uint32_t query(uint64_t __n, uint64_t __m) {
            if (!s_inited) init();
            if (__n < __m) return 0;
            uint32_t res = 1;
            while (__m && __n != __m) {
                uint64_t n = __n / _P, m = __m / _P;
                res = Modular32<_P>::multiply_64(res, rawQuery(__n - n * _P, __m - m * _P));
                __n = n;
                __m = m;
            }
            return res;
        }
    };
}

#endif