/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_POLLARDRHO__
#define __OY_POLLARDRHO__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"
#include "PrimeCheck.h"

namespace OY {
    struct PollardRho {
        struct PollardRhoPair {
            uint64_t m_prime;
            uint32_t m_count;
            bool operator<(const PollardRhoPair &rhs) const { return m_prime < rhs.m_prime; }
        };
        static uint64_t pick(uint64_t n) {
            if (n % 2 == 0) { return 2; }
            struct Info {
                uint64_t m_mod, m_pinv;
                void set_mod(uint64_t n) {
                    m_mod = m_pinv = n;
                    for (size_t i = 0; i < 5; ++i) m_pinv *= 2 - m_mod * m_pinv;
                }
                uint64_t mul_add(uint64_t a, uint64_t b, uint64_t c) const {
#ifdef _MSC_VER
                    uint64_t high, low, _, res;
                    low = _umul128(a, b, &high);
                    _umul128(_umul128(low, m_pinv, &_), m_mod, &res);
                    return c + m_mod + high - res;
#else
                    __uint128_t d = __uint128_t(a) * b;
                    return c + m_mod + (d >> 64) - uint64_t((__uint128_t(uint64_t(d) * m_pinv) * m_mod) >> 64);
#endif
                }
                uint64_t mul(uint64_t a, uint64_t b) const { return mul_add(a, b, 0); }
            };
            Info info;
            info.set_mod(n);
            uint64_t C1 = 1, C2 = 2, M = 512, Z1 = 1, Z2 = 2, ans = 0;
            auto find = [&]() {
                uint64_t z1 = Z1, z2 = Z2;
                for (uint64_t k = M;; k *= 2) {
                    uint64_t x1 = z1 + n, x2 = z2 + n;
                    for (uint64_t j = 0; j < k; j += M) {
                        uint64_t y1 = z1, y2 = z2, q1 = 1, q2 = 2;
                        z1 = info.mul_add(z1, z1, C1), z2 = info.mul_add(z2, z2, C2);
                        for (uint64_t i = 0; i < M; ++i) {
                            uint64_t t1 = x1 - z1, t2 = x2 - z2;
                            z1 = info.mul_add(z1, z1, C1), z2 = info.mul_add(z2, z2, C2);
                            q1 = info.mul(q1, t1), q2 = info.mul(q2, t2);
                        }
                        q1 = info.mul(q1, x1 - z1), q2 = info.mul(q2, x2 - z2);
                        uint64_t q3 = info.mul(q1, q2), g3 = std::gcd(n, q3);
                        if (g3 == 1) continue;
                        if (g3 != n) return void(ans = g3);
                        uint64_t g1 = std::gcd(n, q1), g2 = std::gcd(n, q2), C = g1 != 1 ? C1 : C2, x = g1 != 1 ? x1 : x2, z = g1 != 1 ? y1 : y2, g = g1 != 1 ? g1 : g2;
                        if (g == n)
                            do z = info.mul_add(z, z, C), g = std::gcd(n, x - z);
                            while (g == 1);
                        if (g != n) return void(ans = g);
                        Z1 += 2, Z2 += 2;
                        return;
                    }
                }
            };
            do find();
            while (!ans);
            return ans;
        }
        static bool is_prime(uint64_t x) { return is_prime64(x); }
        template <typename Callback>
        static void _dfs(uint64_t cur, Callback &&call) {
            if (!is_prime(cur)) {
                uint64_t a = pick(cur);
                _dfs(a, call), _dfs(cur / a, call);
            } else
                call(cur);
        }
        template <typename Callback>
        static void enumerate_prime_factors(uint64_t n, Callback &&call) {
            if (n % 2 == 0) {
                uint32_t ctz = std::countr_zero(n);
                n >>= ctz;
                while (ctz--) call(uint64_t(2));
            }
            if (n > 1) _dfs(n, call);
        }
        template <bool Sorted = false>
        static std::vector<PollardRhoPair> decomposite(uint64_t n) {
            std::vector<PollardRhoPair> res;
            if (n % 2 == 0) {
                uint32_t ctz = std::countr_zero(n);
                res.push_back({uint64_t(2), ctz}), n >>= ctz;
            }
            auto call = [&](uint64_t x) {
                auto find = std::find_if(res.begin(), res.end(), [&](const PollardRhoPair &p) { return p.m_prime == x; });
                if (find == res.end())
                    res.push_back({x, 1});
                else
                    find->m_count++;
            };
            if (n > 1) _dfs(n, call);
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        template <typename Callback>
        static void _dfs(uint32_t index, uint64_t prod, const std::vector<PollardRhoPair> &pairs, Callback &&call) {
            if (index == pairs.size())
                call(prod);
            else {
                auto &&pair = pairs[index];
                uint64_t p = pair.m_prime, c = pair.m_count;
                _dfs(index + 1, prod, pairs, call);
                while (c--) _dfs(index + 1, prod *= p, pairs, call);
            }
        }
        template <typename Callback>
        static void enumerate_factors(const std::vector<PollardRhoPair> &pairs, Callback &&call) { _dfs(0, 1, pairs, call); }
        template <typename Callback>
        static void enumerate_factors(uint64_t n, Callback &&call) { enumerate_factors(decomposite<false>(n), call); }
        template <bool Sorted = false>
        static std::vector<uint64_t> get_factors(uint64_t n) {
            std::vector<uint64_t> res;
            uint32_t count = 1;
            auto pairs = decomposite<false>(n);
            for (auto &&pair : pairs) count *= pair.m_count + 1;
            res.reserve(count);
            enumerate_factors(pairs, [&](uint64_t f) { res.push_back(f); });
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        static uint64_t get_Euler_Phi(uint64_t n) {
            for (const auto &pair : decomposite(n)) n = n / pair.m_prime * (pair.m_prime - 1);
            return n;
        }
    };
}

#endif