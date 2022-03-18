#ifndef __OY_LIMITEDGCD__
#define __OY_LIMITEDGCD__

#include <cmath>
#include <cstdint>
#include <functional>
#include "Barrett.h"

namespace OY {
    template <uint32_t _N>
    class LimitedGCD {
        static constexpr uint32_t sqrt = [] {
            uint32_t i = 1;
            while (i * i + i * 2 + 1 <= _N) i++;
            return i;
        }();
        uint32_t m_decomposition[_N + 1][3];
        uint32_t m_smallestFactor[_N + 1]{0};
        uint32_t m_gcd[sqrt][sqrt];
        uint32_t m_primeList[_N >= 1000000 ? _N / 12 : 100000];
        uint32_t m_primeCnt = 0;

    public:
        LimitedGCD() {
            m_decomposition[0][0] = m_decomposition[0][1] = 1;
            m_decomposition[0][2] = 0;
            m_decomposition[1][0] = m_decomposition[1][1] = m_decomposition[1][2] = 1;
            for (int i = 2; i <= _N; i++) {
                if (!m_smallestFactor[i]) {
                    m_smallestFactor[i] = i;
                    m_primeList[m_primeCnt++] = i;
                    m_decomposition[i][0] = m_decomposition[i][1] = 1;
                    m_decomposition[i][2] = i;
                } else {
                    uint32_t a = i / m_smallestFactor[i];
                    std::copy(m_decomposition[a], m_decomposition[a + 1], m_decomposition[i]);
                    m_decomposition[i][0] *= m_smallestFactor[i];
                    std::sort(m_decomposition[i], m_decomposition[i + 1]);
                }
                for (uint32_t *it = m_primeList, *end = m_primeList + m_primeCnt; it < end; ++it) {
                    auto p = *it, q = p * i;
                    if (q > _N) break;
                    if (i % p)
                        m_smallestFactor[q] = p;
                    else {
                        m_smallestFactor[q] = m_smallestFactor[p];
                        break;
                    }
                }
            }
            for (int j = 0; j < sqrt; j++) m_gcd[0][j] = j;
            for (int i = 1; i < sqrt; i++) {
                m_gcd[i][0] = i;
                for (int j = 1; j < i; j++) m_gcd[i][j] = m_gcd[i - j][j];
                m_gcd[i][i] = i;
                for (int j = i + 1; j < sqrt; j++) m_gcd[i][j] = m_gcd[i][j - i];
            }
        }
        uint32_t gcd(uint32_t __a, uint32_t __b) const {
            if (__a < sqrt)
                return __b < sqrt ? m_gcd[__a][__b] : m_gcd[__a][__b % __a];
            else if (__b < sqrt)
                return m_gcd[__b][__a % __b];
            if (m_decomposition[__a][1] == 1) return __b % __a ? 1 : __a;
            if (m_decomposition[__b][1] == 1) return __a % __b ? 1 : __b;
            uint32_t res = 1;
            auto transfer = [&](uint32_t x) {
                res *= x;
                __b /= x;
            };
            if (uint32_t f1 = m_decomposition[__a][0]; f1 > 1) {
                if (int x = __b % f1; !x) {
                    res *= f1;
                    __b /= f1;
                } else if (m_decomposition[f1][1] > 1)
                    transfer(m_gcd[f1][x]);
            }
            uint32_t f2 = m_decomposition[__a][1];
            if (int x = __b % f2; !x) {
                res *= f2;
                __b /= f2;
            } else if (m_decomposition[f2][1] > 1)
                transfer(m_gcd[f2][x]);
            uint32_t f3 = m_decomposition[__a][2];
            if (int x = __b % f3; !x)
                res *= f3;
            else if (f3 < sqrt && m_decomposition[f3][1] > 1)
                transfer(m_gcd[f3][x]);
            return res;
        }
        uint64_t lcm(uint32_t __a, uint32_t __b) const { return __a && __b ? uint64_t(__a / gcd(__a, __b)) * __b : 0; }
        auto getSolver(uint32_t __a) const {
            class _solver {
                Barrett32 brt, brt1, brt2, brt3;
                uint32_t f1, f2, f3;
                const uint32_t *gcd1, *gcd2, *gcd3;
                bool isPrime1, isPrime2, isPrime3;
                const LimitedGCD &helper;
                void transfer(uint32_t &res, uint32_t &b, uint32_t t) const {
                    res *= t;
                    b /= t;
                }

            public:
                _solver(uint32_t a, const LimitedGCD &_helper) : helper(_helper) {
                    f1 = _helper.m_decomposition[a][0];
                    f2 = _helper.m_decomposition[a][1];
                    f3 = _helper.m_decomposition[a][2];
                    brt.m_P = a;
                    if (f3 <= 1) return;
                    brt = Barrett32(a);
                    brt3 = Barrett32(f3);
                    gcd3 = _helper.m_gcd[f3];
                    isPrime3 = f3 >= sqrt || helper.m_decomposition[f3][1] == 1;
                    if (f3 > sqrt) {
                        f2 *= f1;
                        f1 = 1;
                    }
                    if (f2 == 1) return;
                    brt2 = Barrett32(f2);
                    gcd2 = _helper.m_gcd[f2];
                    isPrime2 = helper.m_decomposition[f2][1] == 1;
                    if (f1 == 1) return;
                    brt1 = Barrett32(f1);
                    gcd1 = _helper.m_gcd[f1];
                    isPrime1 = helper.m_decomposition[f1][1] == 1;
                }
                uint32_t queryGCD(uint32_t b) const {
                    if (!b) return brt.mod();
                    if (f3 == 0) return b;
                    if (f3 == 1) return 1;
                    if (f2 == 1) return brt.mod(b) ? 1 : brt.mod();
                    if (helper.m_decomposition[b][1] == 1) return brt.mod() % b ? 1 : b;
                    uint32_t res = 1;
                    if (f1 > 1) {
                        if (auto [quo, rem] = brt1.divmod(b); !rem) {
                            res *= f1;
                            b = quo;
                        } else if (!isPrime1)
                            transfer(res, b, *(gcd1 + rem));
                    }
                    if (auto [quo, rem] = brt2.divmod(b); !rem) {
                        res *= f2;
                        b = quo;
                    } else if (!isPrime2)
                        transfer(res, b, *(gcd2 + rem));
                    if (auto [quo, rem] = brt3.divmod(b); !rem)
                        res *= f3;
                    else if (!isPrime3)
                        transfer(res, b, *(gcd3 + rem));
                    return res;
                }
                uint64_t queryLCM(uint32_t b) const {
                    if (!b || !f3) return 0;
                    if (f3 == 1) return b;
                    if (f2 == 1) return brt.mod(b) ? uint64_t(f3) * b : b;
                    if (helper.m_decomposition[b][1] == 1) return brt.mod() % b ? uint64_t(brt.mod()) * b : brt.mod();
                    if (f1 > 1) {
                        if (auto [quo, rem] = brt1.divmod(b); !rem)
                            b = quo;
                        else if (!isPrime1)
                            b /= *(gcd1 + rem);
                    }
                    if (auto [quo, rem] = brt2.divmod(b); !rem)
                        b = quo;
                    else if (!isPrime2)
                        b /= *(gcd2 + rem);
                    if (auto [quo, rem] = brt3.divmod(b); !rem)
                        b = quo;
                    else if (!isPrime3)
                        b /= *(gcd3 + rem);
                    return uint64_t(brt.mod()) * b;
                    ;
                }
            };
            return _solver(__a, *this);
        }
    };
}

#endif