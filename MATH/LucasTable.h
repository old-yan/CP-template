#ifndef __OY_LUCASTABLE__
#define __OY_LUCASTABLE__

#include "Barrett.h"
#include "DynamicModInt.h"
#include "StaticModInt.h"

namespace OY {
    template <typename _Tp>
    struct LucasTable {
        const uint32_t m_P;
        std::vector<_Tp> m_factorial, m_factorialInv;
        LucasTable() : m_P(_Tp::mod()), m_factorial(_Tp::mod(), _Tp(1)), m_factorialInv(_Tp::mod()) {
            for (uint32_t i = 1; i < m_factorial.size(); i++) m_factorial[i] = m_factorial[i - 1] * _Tp(i);
            m_factorialInv.back() = m_factorial.back().inv();
            for (uint32_t i = m_factorialInv.size() - 2; ~i; i--) m_factorialInv[i] = m_factorialInv[i + 1] * _Tp(i + 1);
        }
        _Tp rawQuery(uint32_t __n, uint32_t __m) const {
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            return m_factorial[__n] * m_factorialInv[__m] * m_factorialInv[__n - __m];
        }
        _Tp query(uint64_t __n, uint64_t __m) const {
            if (__n < __m) return 0;
            _Tp res(1);
            while (__m && __n != __m) {
                uint64_t nq = __n / m_P, nr = __n - nq * m_P, mq = __m / m_P, mr = __m - mq * m_P;
                res *= rawQuery(nr, mr);
                __n = nq, __m = mq;
            }
            return res;
        }
    };
    template <typename _Tp>
    struct LucasTable_ex {
        static constexpr uint32_t max_prime_count = 15;
        uint32_t m_P, m_prime_count, m_decomposite[max_prime_count][4], m_coef[max_prime_count][64];
        Barrett32 m_brt[max_prime_count];
        std::vector<uint32_t> m_factorials[max_prime_count];
        LucasTable_ex() : m_P(_Tp::mod()), m_prime_count(0) {
            uint32_t rest = m_P;
            auto add_prime = [&](uint32_t P, uint32_t base, uint32_t exp) { m_decomposite[m_prime_count][0] = P, m_decomposite[m_prime_count][1] = P / base * (base - 1), m_decomposite[m_prime_count][2] = base, m_decomposite[m_prime_count++][3] = exp; };
            auto reduce = [&](uint32_t val, uint32_t base) {
                uint32_t quot, rem = 0;
                while (!rem)
                    if (quot = val / base, rem = val - quot * base; !rem) val = quot;
                return val;
            };
            for (uint32_t i = 2; i * i <= rest; i++) {
                uint32_t quot, rem = 0, P = 1, count = 0;
                while (!rem)
                    if (quot = rest / i, rem = rest - quot * i; !rem) count++, rest = quot, P *= i;
                if (count) add_prime(P, i, count);
            }
            if (rest > 1) add_prime(rest, rest, 1);
            for (uint32_t i = 0; i < m_prime_count; i++) {
                const uint32_t P = m_decomposite[i][0], base = m_decomposite[i][2], exp = m_decomposite[i][3];
                m_coef[i][0] = 1;
                for (uint32_t j = 1; j < exp; j++) m_coef[i][j] = m_coef[i][j - 1] * base;
                m_brt[i] = Barrett32(P);
                m_factorials[i].resize(P + 1);
                m_factorials[i][0] = 1;
                for (uint32_t j = 1; j <= P; j++) m_factorials[i][j] = m_brt[i].multiply_64(m_factorials[i][j - 1], reduce(j, base));
            }
        }
        _Tp rawQuery(uint32_t __n, uint32_t __m) const {
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            _Tp res(0);
            for (uint32_t i = 0; i < m_prime_count; i++) {
                const uint32_t P = m_decomposite[i][0], phi = m_decomposite[i][1], base = m_decomposite[i][2], exp = m_decomposite[i][3];
                auto fast_power = [&](uint32_t a, uint32_t n) {
                    uint32_t res(1);
                    while (n) {
                        if (n & 1) res = m_brt[i].multiply_64(res, a);
                        a = m_brt[i].multiply_64(a, a), n >>= 1;
                    }
                    return res;
                };
                auto inv = [&](uint32_t a) { return fast_power(a, phi - 1); };
                auto fac = [&](uint32_t n) { return m_factorials[i][n]; };
                auto get_count = [&](uint32_t val) {
                    uint32_t count = 0;
                    do count += (val /= base);
                    while (val);
                    return count;
                };
                if (uint32_t count = get_count(__n) - get_count(__m) - get_count(__n - __m); count < exp) res += _Tp(m_P / P).pow(phi) * m_brt[i].multiply_64(m_brt[i].multiply_64(m_brt[i].multiply_64(m_factorials[i][__n], inv(m_factorials[i][__m])), inv(m_factorials[i][__n - __m])), m_coef[i][count]);
            }
            return res;
        }
        _Tp query(uint64_t __n, uint64_t __m) const {
            if (!__m || __n == __m) return 1;
            if (__n < __m) return 0;
            _Tp res(0);
            for (uint32_t i = 0; i < m_prime_count; i++) {
                const uint32_t P = m_decomposite[i][0], phi = m_decomposite[i][1], base = m_decomposite[i][2], exp = m_decomposite[i][3];
                auto fast_power = [&](uint32_t a, uint32_t n) {
                    uint32_t res(1);
                    while (n) {
                        if (n & 1) res = m_brt[i].multiply_64(res, a);
                        a = m_brt[i].multiply_64(a, a), n >>= 1;
                    }
                    return res;
                };
                auto inv = [&](uint32_t a) { return fast_power(a, phi - 1); };
                auto fac = [&](uint64_t n) {
                    auto dfs = [&](auto self, uint64_t n) {
                        if (n <= P) return m_factorials[i][n];
                        uint64_t quot = n / P, rem = n - quot * P;
                        return m_brt[i].multiply_64(m_brt[i].multiply_64(fast_power(m_factorials[i][P], quot), m_factorials[i][rem]), self(self, quot));
                    };
                    return dfs(dfs, n);
                };
                auto get_count = [&](uint64_t val) {
                    uint64_t count = 0;
                    do count += (val /= base);
                    while (val);
                    return count;
                };
                if (uint64_t count = get_count(__n) - get_count(__m) - get_count(__n - __m); count < exp) res += _Tp(m_P / P).pow(phi) * m_brt[i].multiply_64(m_brt[i].multiply_64(m_brt[i].multiply_64(fac(__n), inv(fac(__m))), inv(fac(__n - __m))), m_coef[i][count]);
            }
            return res;
        }
    };
}

#endif