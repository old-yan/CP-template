/*
最后修改:
20240418
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BITSET__
#define __OY_BITSET__

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace Bitset {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <size_type N = 1000>
        struct Table {
            mask_type m_mask[(N + MASK_SIZE - 1) / MASK_SIZE];
            size_type m_size;
            static mask_type _get_mask(size_type l, size_type r) { return r ? (mask_type(1) << r) - (mask_type(1) << l) : -(mask_type(1) << l); }
            static mask_type _get_trail_mask(size_type l) { return -(mask_type(1) << l); }
            static mask_type _get_lead_mask(size_type r) { return r ? (mask_type(1) << r) - 1 : -1; }
            void _sanitize() {
                m_mask[(m_size - 1) / MASK_SIZE] &= _get_lead_mask(m_size % MASK_SIZE);
            }
            size_type _next_one_small(size_type x) const {
                if (x + 1 >= m_size) return -1;
                mask_type a = m_mask[0] & _get_trail_mask(x + 1);
                return a ? std::countr_zero(a) : -1;
            }
            size_type _next_one_big(size_type x) const {
                if (x + 1 >= m_size) return -1;
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                mask_type a = m_mask[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                if (i == last_bucket) return -1;
                while (++i != last_bucket && !m_mask[i]) {}
                size_type ctz = std::countr_zero(m_mask[i]);
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            size_type _next_zero_small(size_type x) const {
                if (x + 1 >= m_size) return -1;
                mask_type a = ~m_mask[0] & _get_mask(x + 1, m_size % MASK_SIZE);
                return a ? std::countr_zero(a) : -1;
            }
            size_type _next_zero_big(size_type x) const {
                if (x + 1 >= m_size) return -1;
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                if (i == last_bucket) {
                    mask_type a = ~m_mask[i] & _get_mask(j, m_size % MASK_SIZE);
                    return a ? (i << MASK_WIDTH) + std::countr_zero(a) : -1;
                }
                mask_type a = ~m_mask[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                while (++i != last_bucket && !~m_mask[i]) {}
                if (i != last_bucket) return (i << MASK_WIDTH) + std::countr_zero(~m_mask[i]);
                size_type ctz = std::countr_zero(~m_mask[i] & _get_lead_mask(m_size % MASK_SIZE));
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            template <size_type I>
            void _set_small() {
                m_mask[I / MASK_SIZE] = -1;
                if constexpr (I + MASK_SIZE < N) _set_small<I + MASK_SIZE>();
            }
            template <size_type I>
            void _reset_small() {
                m_mask[I / MASK_SIZE] = 0;
                if constexpr (I + MASK_SIZE < N) _reset_small<I + MASK_SIZE>();
            }
            template <size_type I>
            size_type _count_small() const {
                if constexpr (I + MASK_SIZE < N)
                    return std::popcount(m_mask[I / MASK_SIZE]) + _count_small<I + MASK_SIZE>();
                else
                    return std::popcount(m_mask[I / MASK_SIZE]);
            }
            void _left_shift(size_type x) {
                if (x) {
                    size_type last_bucket = (m_size - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = last_bucket; i > y; i--) m_mask[i] = ((m_mask[i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_mask[i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_mask[y] = (m_mask[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        } else {
                            for (size_type i = last_bucket; i; i--) m_mask[i] = ((m_mask[i] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_mask[i - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_mask[0] = (m_mask[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        }
                    else
                        for (size_type i = last_bucket; i >= y; i--) m_mask[i] = m_mask[i - y];
                    std::fill_n(m_mask, y, mask_type(0));
                    _sanitize();
                }
            }
            void _right_shift(size_type x) {
                if (x) {
                    size_type last_bucket = (m_size - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = 0; i + y < last_bucket; i++) m_mask[i] = ((m_mask[i + y] & _get_trail_mask(z)) >> z) | ((m_mask[i + y + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_mask[last_bucket - y] = (m_mask[last_bucket] & _get_trail_mask(z)) >> z;
                        } else {
                            for (size_type i = 0; i < last_bucket; i++) m_mask[i] = ((m_mask[i] & _get_trail_mask(z)) >> z) | ((m_mask[i + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_mask[last_bucket] = (m_mask[last_bucket] & _get_trail_mask(z)) >> z;
                        }
                    else {
                        for (size_type i = 0; i + y < last_bucket; i++) m_mask[i] = m_mask[i + y];
                        m_mask[last_bucket - y] = m_mask[last_bucket] & _get_lead_mask(m_size % MASK_SIZE);
                    }
                    std::fill_n(m_mask + last_bucket - y + 1, y, mask_type(0));
                }
            }
            explicit Table(size_type length = N) { resize(length); }
            Table(const Table<N> &rhs) = default;
            void resize(size_type length) {
                if (!(m_size = length)) return;
                std::fill_n(m_mask, (m_size + MASK_SIZE - 1) / MASK_SIZE, 0);
            }
            void set() {
                if constexpr (N <= MASK_SIZE * 8)
                    _set_small<0>();
                else
                    memset(m_mask, -1, ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
                _sanitize();
            }
            void set(size_type i) { m_mask[i / MASK_SIZE] |= mask_type(1) << (i % MASK_SIZE); }
            void set(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_mask[l] |= _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_mask[l] |= _get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i != r; i++) m_mask[i] = -1;
                    m_mask[r] |= _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            void reset() {
                if constexpr (N <= MASK_SIZE * 8)
                    _reset_small<0>();
                else
                    memset(m_mask, 0, ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
            }
            void reset(size_type i) { m_mask[i / MASK_SIZE] -= mask_type(1) << (i % MASK_SIZE); }
            void reset(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_mask[l] &= ~_get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_mask[l] &= ~_get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i != r; i++) m_mask[i] = 0;
                    m_mask[r] &= ~_get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            void flip() {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_mask[i] = ~m_mask[i];
                _sanitize();
            }
            void flip(size_type i) { m_mask[i / MASK_SIZE] ^= mask_type(1) << (i % MASK_SIZE); }
            void flip(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_mask[l] ^= _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_mask[l] ^= _get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i < r; i++) m_mask[i] = ~m_mask[i];
                    m_mask[r] ^= _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            size_type count() const {
                if constexpr (N <= MASK_SIZE * 4)
                    return _count_small<0>();
                else
                    return count(0, m_size - 1);
            }
            size_type count(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return std::popcount(m_mask[l] & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    size_type res = std::popcount(m_mask[l] & _get_trail_mask(left % MASK_SIZE));
                    for (size_type i = l + 1; i != r; i++) res += std::popcount(m_mask[i]);
                    return res + std::popcount(m_mask[r] & _get_lead_mask((right + 1) % MASK_SIZE));
                }
            }
            bool any() const { return any(0, m_size - 1); }
            bool any(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return m_mask[l] & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    if (m_mask[l] & _get_trail_mask(left % MASK_SIZE)) return true;
                    for (size_type i = l + 1; i != r; i++)
                        if (m_mask[i]) return true;
                    return m_mask[r] & _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            bool all() const { return all(0, m_size - 1); }
            bool all(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return !((~m_mask[l]) & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    if (((~m_mask[l]) & _get_trail_mask(left % MASK_SIZE))) return false;
                    for (size_type i = l + 1; i != r; i++)
                        if (~m_mask[i]) return false;
                    return !((~m_mask[r]) & _get_lead_mask((right + 1) % MASK_SIZE));
                }
            }
            size_type first_one() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = 0;
                while (i != last_bucket && !m_mask[i]) i++;
                if (i != last_bucket)
                    return (i << MASK_WIDTH) + std::countr_zero(m_mask[i]);
                else {
                    size_type ctz = std::countr_zero(m_mask[i]);
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_one() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = last_bucket;
                size_type clz = std::countl_zero(m_mask[i]);
                if (clz != MASK_SIZE) return (i << MASK_WIDTH) + MASK_SIZE - clz - 1;
                while (~--i && !m_mask[i]) {}
                return ~i ? (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(m_mask[i]) - 1 : -1;
            }
            size_type first_zero() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = 0;
                while (i != last_bucket && !~m_mask[i]) i++;
                if (i != last_bucket)
                    return (i << MASK_WIDTH) + std::countr_zero(~m_mask[i]);
                else {
                    size_type ctz = std::countr_zero(~m_mask[i] & _get_lead_mask(m_size % MASK_SIZE));
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_zero() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = last_bucket;
                size_type clz = std::countl_zero(~m_mask[i] & _get_lead_mask(m_size % MASK_SIZE));
                if (clz != MASK_SIZE) return (i << MASK_WIDTH) + MASK_SIZE - clz - 1;
                while (~--i && !~m_mask[i]) {}
                return ~i ? (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(~m_mask[i]) - 1 : -1;
            }
            size_type prev_one(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) / MASK_SIZE;
                mask_type a = m_mask[i] & _get_lead_mask(x % MASK_SIZE);
                if (a) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(a) - 1;
                while (~--i && !m_mask[i]) {};
                if (~i) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(m_mask[i]) - 1;
                return -1;
            }
            size_type next_one(size_type x) const {
                if constexpr (N <= MASK_SIZE)
                    return _next_one_small(x);
                else
                    return _next_one_big(x);
            }
            size_type prev_zero(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) / MASK_SIZE;
                mask_type a = (~m_mask[i]) & _get_lead_mask(x % MASK_SIZE);
                if (a) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(a) - 1;
                while (~--i && !~m_mask[i]) {};
                if (~i) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(~m_mask[i]) - 1;
                return -1;
            }
            size_type next_zero(size_type x) const {
                if constexpr (N <= MASK_SIZE)
                    return _next_zero_small(x);
                else
                    return _next_zero_big(x);
            }
            bool at(size_type i) const { return m_mask[i / MASK_SIZE] >> (i % MASK_SIZE) & 1; }
            bool operator[](size_type i) const { return at(i); }
            size_type kth(size_type k) const {
                size_type i = 0;
                while (true) {
                    size_type a = std::popcount(m_mask[i]);
                    if (k < a) {
                        mask_type b = m_mask[i];
                        while (k--) b -= b & -b;
                        return (i << MASK_WIDTH) + std::countr_zero(b);
                    }
                    k -= a, i++;
                }
                return -1;
            }
            void bitor_lshift(size_type x, size_type range = -1) {
                if (x >= m_size) return;
                range = std::min(range, m_size - x);
                size_type last_bucket = (range - 1 + x) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                if (z) {
                    for (size_type i = 0; i + y < last_bucket; i++) m_mask[last_bucket - i] |= ((m_mask[last_bucket - i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_mask[last_bucket - i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                    m_mask[y] |= (m_mask[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                } else
                    for (size_type i = 0; i + y <= last_bucket; i++) m_mask[last_bucket - i] |= m_mask[last_bucket - i - y];
                _sanitize();
            }
            Table<N> &operator|=(const Table<N> &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_mask[i] |= rhs.m_mask[i];
                return *this;
            }
            Table<N> &operator&=(const Table<N> &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_mask[i] &= rhs.m_mask[i];
                return *this;
            }
            Table<N> &operator^=(const Table<N> &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_mask[i] ^= rhs.m_mask[i];
                return *this;
            }
            Table<N> &operator>>=(size_type x) {
                if (x >= m_size)
                    reset();
                else
                    _right_shift(x);
                return *this;
            }
            Table<N> &operator<<=(size_type x) {
                if (x >= m_size)
                    reset();
                else
                    _left_shift(x);
                return *this;
            }
            template <typename Callback>
            void do_for_each_one(Callback &&call) const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0, j = 0; i != last_bucket; i++, j += MASK_SIZE)
                    if (m_mask[i]) {
                        auto mask = m_mask[i];
                        do {
                            size_type ctz = std::countr_zero(mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (mask);
                    }
                size_type j = last_bucket * MASK_SIZE;
                auto mask = m_mask[last_bucket];
                while (mask) {
                    size_type ctz = std::countr_zero(mask);
                    call(j + ctz);
                    mask ^= mask_type(1) << ctz;
                }
            }
            template <typename Callback>
            void do_for_each_zero(Callback &&call) const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0, j = 0; i != last_bucket; i++, j += MASK_SIZE)
                    if (~m_mask[i]) {
                        auto mask = m_mask[i];
                        do {
                            size_type ctz = std::countr_zero(~mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (~mask);
                    }
                size_type j = last_bucket * MASK_SIZE;
                auto mask = m_mask[last_bucket] ^ _get_lead_mask(m_size % MASK_SIZE);
                while (mask) {
                    size_type ctz = std::countr_zero(mask);
                    call(j + ctz);
                    mask ^= mask_type(1) << ctz;
                }
            }
            friend Table<N> operator|(const Table<N> &lhs, const Table<N> &rhs) {
                Table<N> res(lhs);
                res |= rhs;
                return res;
            }
            friend Table<N> operator&(const Table<N> &lhs, const Table<N> &rhs) {
                Table<N> res(lhs);
                res &= rhs;
                return res;
            }
            friend Table<N> operator^(const Table<N> &lhs, const Table<N> &rhs) {
                Table<N> res(lhs);
                res ^= rhs;
                return res;
            }
            friend Table<N> operator<<(const Table<N> &o, size_type x) {
                Table<N> res(o);
                res <<= x;
                return res;
            }
            friend Table<N> operator>>(const Table<N> &o, size_type x) {
                Table<N> res(o);
                res >>= x;
                return res;
            }
        };
        template <typename Ostream, size_type N>
        Ostream &operator<<(Ostream &out, const Table<N> &x) {
            out << "{";
            for (size_type i = 0, j = 0; i < x.m_size; i++)
                if (x.at(i)) out << (j++ ? ", " : "") << i;
            return out << "}";
        }
    }
}

#endif