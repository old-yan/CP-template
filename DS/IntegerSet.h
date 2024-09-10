/*
最后修改:
20240526
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_INTEGERSET__
#define __OY_INTEGERSET__

#include <algorithm>
#include <cstdint>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace INTSET {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4, S0 = 1, S1 = MASK_SIZE, S2 = S1 * MASK_SIZE, S3 = S2 * MASK_SIZE, S4 = S3 * MASK_SIZE, S5 = S4 * MASK_SIZE;
        constexpr size_type calc_buffer_size(size_type n) { return n <= S1 ? 1 : (n <= S2 ? 1 + S1 : (n <= S3 ? 1 + S1 + S2 : (n <= S4 ? 1 + S1 + S2 + S3 : 1 + S1 + S2 + S3 + S4))); }
        template <size_t MAX_LEVEL>
        class Table {
            static constexpr size_type mask_count = calc_buffer_size(1 << MAX_LEVEL), capacity = 1 << (MAX_LEVEL + MASK_WIDTH - 1) / MASK_WIDTH * MASK_WIDTH, shift = mask_count + MASK_SIZE - 1, leaf_start = shift / MASK_SIZE, leaf_end = shift / MASK_SIZE + capacity / MASK_SIZE;
            mask_type m_mask[mask_count + 1]{};
            bool _get(size_type x) const { return m_mask[x / MASK_SIZE] >> (x % MASK_SIZE) & 1; }
            void _set(size_type x) { m_mask[x / MASK_SIZE] |= mask_type(1) << (x % MASK_SIZE); }
            void _flip(size_type x) { m_mask[x / MASK_SIZE] ^= mask_type(1) << (x % MASK_SIZE); }
            void _reset(size_type x) { m_mask[x / MASK_SIZE] &= ~(mask_type(1) << (x % MASK_SIZE)); }
            bool _try_set(size_type x) {
                size_type quot = x / MASK_SIZE, rem = x % MASK_SIZE;
                if (m_mask[quot] >> rem & 1) return false;
                return m_mask[quot] |= mask_type(1) << rem, true;
            }
            bool _try_reset(size_type x) {
                size_type quot = x / MASK_SIZE, rem = x % MASK_SIZE;
                if (!(m_mask[quot] >> rem & 1)) return false;
                return m_mask[quot] ^= mask_type(1) << rem, true;
            }
        public:
            Table() { m_mask[0] = -1; }
            void set(size_type pos) {
                size_type x = pos + shift;
                while (_try_set(x)) x = x / MASK_SIZE + MASK_SIZE - 2;
            }
            void reset(size_type pos) {
                size_type x = pos + shift;
                if (!_get(x)) return;
                do {
                    size_type quot = x / MASK_SIZE, rem = x % MASK_SIZE;
                    if (m_mask[quot] ^= mask_type(1) << rem) break;
                    x = quot + MASK_SIZE - 2;
                } while (x);
            }
            bool empty() const { return !m_mask[1]; }
            size_type count() const {
                size_type res = 0;
                for (size_type i = leaf_start; i != leaf_end; i++) res += std::popcount(m_mask[i]);
                return res;
            }
            static constexpr size_type size() { return capacity; }
            void set() { std::fill_n(m_mask + 1, mask_count, -1); }
            void reset() { std::fill_n(m_mask + 1, mask_count, 0); }
            bool at(size_type pos) const { return _get(pos + shift); }
            bool operator[](size_type pos) const { return _get(pos + shift); }
            size_type lower_bound(size_type pos) const {
                if (pos >= capacity) return -1;
                size_type x = pos + shift, x2 = capacity + shift - 1;
                while (true) {
                    if (x > x2) return -1;
                    if (m_mask[x / MASK_SIZE] >> (x % MASK_SIZE)) break;
                    x = x / MASK_SIZE + MASK_SIZE - 1, x2 = x2 / MASK_SIZE + MASK_SIZE - 2;
                }
                while (x < shift) x = ((x + std::countr_zero(m_mask[x / MASK_SIZE] >> (x % MASK_SIZE))) - MASK_SIZE + 2) * MASK_SIZE;
                return x - shift + std::countr_zero(m_mask[x / MASK_SIZE] >> (x % MASK_SIZE));
            }
            size_type upper_bound(size_type pos) const {
                if (pos + 1 >= capacity) return -1;
                size_type x = pos + shift + 1, x2 = capacity + shift - 1;
                while (true) {
                    if (x > x2) return -1;
                    if (m_mask[x / MASK_SIZE] >> (x % MASK_SIZE)) break;
                    x = x / MASK_SIZE + MASK_SIZE - 1, x2 = x2 / MASK_SIZE + MASK_SIZE - 2;
                }
                while (x < shift) x = ((x + std::countr_zero(m_mask[x / MASK_SIZE] >> (x % MASK_SIZE))) - MASK_SIZE + 2) * MASK_SIZE;
                return x - shift + std::countr_zero(m_mask[x / MASK_SIZE] >> (x % MASK_SIZE));
            }
            size_type smaller_bound(size_type pos) const {
                if (!pos) return -1;
                size_type x = pos + shift - 1, x2 = shift;
                while (true) {
                    if (x < x2) return -1;
                    if (m_mask[x / MASK_SIZE] << (MASK_SIZE - 1 - x % MASK_SIZE)) break;
                    x = x / MASK_SIZE + MASK_SIZE - 3, x2 = x2 / MASK_SIZE + MASK_SIZE - 2;
                }
                while (x < shift) x = ((x - std::countl_zero(m_mask[x / MASK_SIZE] << (MASK_SIZE - 1 - x % MASK_SIZE))) - MASK_SIZE + 3) * MASK_SIZE - 1;
                return x - shift - std::countl_zero(m_mask[x / MASK_SIZE] << (MASK_SIZE - 1 - x % MASK_SIZE));
            }
        };
    }
}

#endif