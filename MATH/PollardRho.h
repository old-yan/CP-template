/*
最后修改:
20231127
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
#include <random>

#include "PrimeCheck.h"
#include "TEST/std_gcd_lcm.h"

namespace OY {
    namespace PollardRho {
        static constexpr size_t batch = 8192;
        std::mt19937_64 pr_rand;
        struct PollardRhoPair {
            uint64_t m_prime;
            uint32_t m_count;
            bool operator<(const PollardRhoPair &rhs) const { return m_prime < rhs.m_prime; }
        };
        uint64_t pick(uint64_t n) {
            if (!(n & 1)) return 2;
            uint64_t pinv = n, ninv, inv = uint64_t(-1) / n + 1;
            for (size_t i = 0; i != 5; i++) pinv *= uint64_t(2) - n * pinv;
#ifdef _MSC_VER
            uint64_t high, low;
            low = _umul128(-n % n, -n % n, &high);
            _udiv128(high, low, n, &ninv);
#else
            ninv = -__uint128_t(n) % n;
#endif
            auto mod = [&](uint64_t val) {
#ifdef _MSC_VER
                uint64_t res;
                _umul128(val, inv, &res);
                res = val - res * n;
#else
                uint64_t res = val - uint64_t((__uint128_t(val) * inv) >> 64) * n;
#endif
                if (res >= n) res += n;
                return res;
            };
#ifdef _MSC_VER
            auto reduce_long = [&](uint64_t high, uint64_t low) {
                uint64_t _, res;
                _umul128(_umul128(low, pinv, &_), n, &res);
                res = high - res;
#else
            auto reduce_long = [&](__uint128_t val) {
                uint64_t res = (val >> 64) - uint64_t((__uint128_t(uint64_t(val) * pinv) * n) >> 64);
#endif
                if (res >= n) res += n;
                return res;
            };
            auto mul = [&](uint64_t a, uint64_t b) {
#ifdef _MSC_VER
                uint64_t high, low;
                low = _umul128(a, b, &high);
                return reduce_long(high, low);
#else
                return reduce_long(__uint128_t(a) * b);
#endif
            };
            std::uniform_int_distribution<uint64_t> distribute(2, n - 1);
            uint64_t v0, v1 = mul(distribute(pr_rand), ninv), prod = mul(1, ninv), c = mul(distribute(pr_rand), ninv);
            for (size_t i = 1; i != batch; i <<= 1) {
                v0 = v1;
                for (size_t j = 0; j != i; j++) v1 = mul(v1, v1) + c;
                for (size_t j = 0; j != i; j++) {
                    v1 = mul(v1, v1) + c;
                    prod = mul(prod, v0 > v1 ? v0 - v1 : v1 - v0);
                    if (!prod) return pick(n);
                }
                uint64_t g = std::gcd(prod, n);
                if (g > 1) return g;
            }
            for (size_t i = batch;; i <<= 1) {
                v0 = v1;
                for (size_t j = 0; j != i; j++) v1 = mul(v1, v1) + c;
                for (size_t j = 0; j != i; j += batch) {
                    for (size_t k = 0; k != batch; k++) {
                        v1 = mul(v1, v1) + c;
                        prod = mul(prod, v0 > v1 ? v0 - v1 : v1 - v0);
                        if (!prod) return pick(n);
                    }
                    uint64_t g = std::gcd(prod, n);
                    if (g > 1) return g;
                }
            }
            return n;
        }
        void _dfs(uint64_t cur, std::vector<PollardRhoPair> &pairs) {
            if (!is_prime(cur)) {
                uint64_t a = pick(cur);
                _dfs(a, pairs), _dfs(cur / a, pairs);
            } else {
                auto find = std::find_if(pairs.begin(), pairs.end(), [&](const PollardRhoPair &x) { return x.m_prime == cur; });
                if (find == pairs.end())
                    pairs.push_back({cur, 1});
                else
                    find->m_count++;
            }
        }
        template <bool Sorted = false>
        std::vector<PollardRhoPair> decomposite(uint64_t n) {
            std::vector<PollardRhoPair> res;
            if (n % 2 == 0) {
                uint32_t ctz = std::countr_zero(n);
                res.push_back({uint64_t(2), ctz}), n >>= ctz;
            }
            if (n > 1) _dfs(n, res);
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        template <typename Callback>
        void _dfs(uint32_t index, uint64_t prod, const std::vector<PollardRhoPair> &pairs, Callback &&call) {
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
        void enumerate_factors(const std::vector<PollardRhoPair> &pairs, Callback &&call) { _dfs(0, 1, pairs, call); }
        template <bool Sorted = false>
        std::vector<uint64_t> get_factors(uint64_t n) {
            std::vector<uint64_t> res;
            uint32_t count = 1;
            auto pairs = decomposite<false>(n);
            for (auto &&pair : pairs) count *= pair.m_count + 1;
            res.reserve(count);
            enumerate_factors(pairs, [&](uint64_t f) { res.push_back(f); });
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        uint64_t get_Euler_Phi(uint64_t n) {
            for (const auto &pair : decomposite(n)) n = n / pair.m_prime * (pair.m_prime - 1);
            return n;
        }
    }
}

#endif