/*
最后修改:
20240703
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MOBIUS__
#define __OY_MOBIUS__

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace MOBIUS {
        using size_type = uint32_t;
        struct CompoundPlus {
            template <typename Tp>
            void operator()(Tp &x, const Tp &y) const { x += y; }
        };
        struct CompoundMinus {
            template <typename Tp>
            void operator()(Tp &x, const Tp &y) const { x -= y; }
        };
        template <size_type MAX_RANGE, bool Flag>
        struct OptionalArray {
            size_type m_data[MAX_RANGE + 1];
            template <typename Mapping>
            void prepare_presum(size_type range, Mapping mapping) {
                m_data[0] = 0;
                for (size_type i = 1; i <= range; i++) m_data[i] = m_data[i - 1] + mapping(i);
            }
            void set(size_type i, size_type val) { m_data[i] = val; }
            size_type query(size_type left, size_type right) const { return m_data[right] - m_data[left - 1]; }
            size_type operator[](size_type i) const { return m_data[i]; }
        };
        template <size_type MAX_RANGE>
        struct OptionalArray<MAX_RANGE, false> {};
        constexpr size_type get_estimated_ln(size_type x) {
            return x <= 7            ? 1
                   : x <= 32         ? 2
                   : x <= 119        ? 3
                   : x <= 359        ? 4
                   : x <= 1133       ? 5
                   : x <= 3093       ? 6
                   : x <= 8471       ? 7
                   : x <= 24299      ? 8
                   : x <= 64719      ? 9
                   : x <= 175196     ? 10
                   : x <= 481451     ? 11
                   : x <= 1304718    ? 12
                   : x <= 3524653    ? 13
                   : x <= 9560099    ? 14
                   : x <= 25874783   ? 15
                   : x <= 70119984   ? 16
                   : x <= 189969353  ? 17
                   : x <= 514278262  ? 18
                   : x <= 1394199299 ? 19
                                     : 20;
        }
        constexpr size_type get_estimated_Pi(size_type x) { return x / get_estimated_ln(x); }
        template <size_type MAX_RANGE, bool RangeQuery = false>
        struct Table {
            static constexpr size_type max_pi = get_estimated_Pi(MAX_RANGE);
            char m_val[MAX_RANGE + 1];
            std::bitset<MAX_RANGE + 1> m_notp;
            size_type m_primes[max_pi + 1], m_pcnt;
            OptionalArray<MAX_RANGE, RangeQuery> m_mobius_presum;
            Table(size_type range = MAX_RANGE) { resize(range); }
            void resize(size_type range) {
                if (!range) return;
                m_val[1] = 1;
                m_notp.set(1);
                m_pcnt = 0;
                m_primes[m_pcnt++] = 2;
                for (size_type i = 3; i <= range; i += 2) {
                    if (!m_notp[i]) m_primes[m_pcnt++] = i, m_val[i] = -1;
                    for (size_type j = 1; j != m_pcnt && i * m_primes[j] <= range; j++) {
                        m_notp.set(i * m_primes[j]);
                        if (i % m_primes[j] == 0) break;
                        m_val[i * m_primes[j]] = -m_val[i];
                    }
                }
                if constexpr (RangeQuery) m_mobius_presum.prepare_presum(range, [&](size_type i) { return query_mobius(i); });
                m_primes[m_pcnt] = std::numeric_limits<size_type>::max() / 2;
            }
            int query_mobius(size_type x) const { return (x & 3) ? ((x & 1) ? m_val[x] : -m_val[x >> 1]) : 0; }
            int query_mobius(size_type left, size_type right) const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return m_mobius_presum.query(left, right);
            }
            bool is_prime(size_type x) const { return (x & 1) ? !m_notp[x] : x == 2; }
            size_type query_kth_prime(size_type k) const { return m_primes[k]; }
        };
        template <size_type MAX_RANGE>
        struct Multiplicative {
            static constexpr size_type max_pi = get_estimated_Pi(MAX_RANGE);
            struct node {
                size_type m_val, m_cnt, m_low, m_prod;
            };
            node m_minp[MAX_RANGE / 2];
            size_type m_primes[max_pi];
            template <typename Tp, typename CalcPrime, typename CalcPrimePow>
            void solve(size_type range, Tp *array, CalcPrime &&calc_prime, CalcPrimePow &&calc_prime_pow) {
                if (range >= 1) array[1] = Tp(1);
                if (range >= 2) array[2] = calc_prime(2);
                for (size_type x = 4, c = 2; x <= range; x <<= 1, c++) array[x] = calc_prime_pow(2, c, x >> 1);
                for (size_type i = 3, odd_pcnt = 0; i <= range; i += 2) {
                    if (!m_minp[i / 2].m_val) {
                        m_primes[odd_pcnt++] = i;
                        m_minp[i / 2] = {i, 1, 1, i};
                        array[i] = calc_prime(i);
                    } else
                        array[i] = m_minp[i / 2].m_prod == i ? calc_prime_pow(m_minp[i / 2].m_val, m_minp[i / 2].m_cnt, m_minp[i / 2].m_low) : array[m_minp[i / 2].m_prod] * array[m_minp[i / 2].m_low];
                    for (size_type j = 0; j != odd_pcnt && i * m_primes[j] <= range; j++) {
                        size_type p = m_primes[j];
                        m_minp[i * p / 2].m_val = p;
                        if (i % p == 0) {
                            m_minp[i * p / 2].m_cnt = m_minp[i / 2].m_cnt + 1;
                            m_minp[i * p / 2].m_low = m_minp[i / 2].m_prod == i ? i : m_minp[i / 2].m_low;
                            m_minp[i * p / 2].m_prod = m_minp[i / 2].m_prod * p;
                            break;
                        }
                        m_minp[i * p / 2].m_cnt = 1;
                        m_minp[i * p / 2].m_low = i;
                        m_minp[i * p / 2].m_prod = p;
                    }
                    if (i + 1 <= range) {
                        size_type ctz = std::countr_zero(i + 1);
                        if (i + 1 != 1 << ctz) array[i + 1] = array[1 << ctz] * array[i + 1 >> ctz];
                    }
                }
            }
            template <typename Tp, typename CalcPrime, typename CalcPrimePow>
            std::vector<Tp> solve(size_type range, CalcPrime &&calc_prime, CalcPrimePow &&calc_prime_pow) {
                std::vector<Tp> res(range + 1);
                solve(range, res.data(), calc_prime, calc_prime_pow);
                return res;
            }
        };
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundPlus>
        inline void partial_sum_Dirichlet_divisor(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = 1, k = p; k <= range; j++, k += p) op(array[k], array[j]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundMinus>
        inline void adjacent_difference_Dirichlet_divisor(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = range / p, k = j * p; k; j--, k -= p) op(array[k], array[j]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundPlus>
        inline void partial_sum_Dirichlet_multiple(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = range / p, k = j * p; k; j--, k -= p) op(array[j], array[k]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundMinus>
        inline void adjacent_difference_Dirichlet_multiple(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = 1, k = p; k <= range; j++, k += p) op(array[j], array[k]);
            }
        }
    }
}

#endif