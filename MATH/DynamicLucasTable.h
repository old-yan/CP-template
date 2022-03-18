#ifndef __OY_DYNAMICLUCAS__
#define __OY_DYNAMICLUCAS__

#include <cstdint>
#include <functional>
#include "Barrett.h"

namespace OY {
    struct DynamicLucasTable {
        Barrett32 m_brt;
        std::vector<uint32_t> m_factorial;
        std::vector<uint32_t> m_factorialInverse;
        DynamicLucasTable(uint32_t __P) : m_brt(__P) {
            // assert(isPrime(__P));
            m_factorial.reserve(__P);
            m_factorial.push_back(1);
            for (uint32_t i = 1; i < __P; i++) m_factorial.push_back(m_brt.multiply_64(m_factorial.back(), i));
            m_factorialInverse.resize(__P);
            m_factorialInverse.back() = m_brt.pow_64(m_factorial.back(), __P - 2);
            for (uint32_t i = __P - 1; i; i--) m_factorialInverse[i - 1] = m_brt.multiply_64(m_factorialInverse[i], i);
        }
        uint32_t rawQuery(uint32_t __n, uint32_t __m) const {
            // assert(__n < m_brt.mod() && __m < m_brt.mod());
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            return m_brt.multiply_64(m_factorial[__n], m_brt.multiply_64(m_factorialInverse[__m], m_factorialInverse[__n - __m]));
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