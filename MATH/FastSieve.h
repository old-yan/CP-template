/*
最后修改:
20231129
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FASTSIEVE__
#define __OY_FASTSIEVE__

#include <algorithm>
#include <bitset>
#include <limits>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace FASTSIEVE {
        using size_type = uint32_t;
        using mask_type = unsigned char;
        struct SieveNode {
            size_type m_val, m_pos[8];
        };
        static constexpr size_type remainder_30[8] = {1, 7, 11, 13, 17, 19, 23, 29};
        static constexpr size_type block_size = 7 * 11 * 13 * 17;
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
        constexpr size_type get_estimated_sqrt_Pi(size_type x) { return 5 << size_type(get_estimated_ln(x) * 0.55); }
        template <size_type MAX_RANGE>
        struct Sieve {
            static constexpr size_type max_r = (get_estimated_sqrt_Pi(MAX_RANGE) + block_size * 2 - 1) / (block_size * 2) * block_size * 2, max_pi = get_estimated_Pi(MAX_RANGE);
            size_type m_primes[max_pi + 1], m_prime_cnt;
            mask_type m_masks[block_size], m_buffer[block_size];
            SieveNode m_nodes[max_r];
            template <typename Callback>
            void _init_sieve(size_type range, Callback &&call) const {
                if (range < 19) return;
                std::vector<bool> vis(range + 1);
                for (size_type i = 3; i * i <= range; i += 2)
                    if (!vis[i])
                        for (size_type j = i * i; j <= range; j += i << 1) vis[j] = true;
                for (size_type i = 19; i <= range; i += 2)
                    if (!vis[i]) call(i);
            }
            void _add_prime(size_type p) { m_primes[m_prime_cnt++] = p; }
            Sieve(size_type range = MAX_RANGE) { resize(range); }
            void resize(size_type range) {
                if (!range) return;
                m_prime_cnt = 0;
                for (size_type p : {2, 3, 5, 7, 11, 13, 17})
                    if (p <= range) _add_prime(p);
                if (range <= block_size) {
                    _init_sieve(range, [&](size_type i) { _add_prime(i); });
                    return;
                }
                std::fill_n(m_masks, block_size, -1);
                for (size_type p : {7, 11, 13, 17})
                    for (size_type i = 0; i != 8; i++) {
                        size_type j = p;
                        while (j % 30 != remainder_30[i]) j += p << 1;
                        for (j /= 30; j < block_size; j += p) m_masks[j] &= ~(1 << i);
                    }
                SieveNode *end = m_nodes;
                auto add_node = [&](size_type p) {
                    for (size_type i = 0; i != 8; i++) {
                        size_type j = p * p;
                        while (j % 30 != remainder_30[i]) j += p << 1;
                        end->m_pos[i] = j / 30;
                    }
                    end++->m_val = p;
                };
                _init_sieve(sqrt(range), add_node);
                for (size_type first = 0, tot = (range - 1) / 30 + 1; first < tot; first += block_size) {
                    size_type last = std::min(tot, first + block_size);
                    std::copy_n(m_masks, block_size, m_buffer);
                    if (!first) m_buffer[0] &= 0xfe;
                    for (auto it = m_nodes; it != end; ++it)
                        for (size_type p = it->m_val, i = 0; i != 8; i++) {
                            size_type j = it->m_pos[i];
                            for (; j < last; j += p) m_buffer[j - first] &= ~(1 << i);
                            it->m_pos[i] = j;
                        }
                    for (size_type i = first; i != last; i++)
                        for (mask_type mask = m_buffer[i - first]; mask;) {
                            size_type x = std::countr_zero(mask);
                            _add_prime(i * 30 + remainder_30[x]), mask -= size_type(1) << x;
                        }
                }
                while (m_primes[m_prime_cnt - 1] > range) m_prime_cnt--;
                m_primes[m_prime_cnt] = std::numeric_limits<size_type>::max() / 2;
            }
            std::bitset<MAX_RANGE + 1> to_bitset() const {
                std::bitset<MAX_RANGE + 1> res;
                for (size_type i = 0; i != m_prime_cnt; i++) res.set(m_primes[i]);
                return res;
            }
            size_type query_kth_prime(size_type k) const { return m_primes[k]; }
            size_type count() const { return m_prime_cnt; }
        };
    }
}

#endif