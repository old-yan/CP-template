/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BITSET__
#define __OY_BITSET__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace Bitset {
        using size_type = uint32_t;
        template <typename MaskType = uint64_t, size_type N = 1000>
        struct Table {
            static constexpr size_type mask_size = sizeof(MaskType) << 3, mask_width = mask_size / 32 + 4;
            MaskType m_mask[(N + mask_size - 1) / mask_size];
            size_type m_size, m_sum;
            static MaskType _get_mask(size_type l, size_type r) { return r ? (MaskType(1) << r) - (MaskType(1) << l) : -(MaskType(1) << l); }
            static MaskType _get_trail_mask(size_type l) { return -(MaskType(1) << l); }
            static MaskType _get_lead_mask(size_type r) { return r ? (MaskType(1) << r) - 1 : -1; }
            Table(size_type length = N) { resize(length); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                std::fill_n(m_mask, (m_size + mask_size - 1) >> mask_width, 0);
                m_sum = 0;
            }
            void set() {
                size_type last_bucket = (m_size - 1) >> mask_width, rem = m_size & (mask_size - 1);
                std::fill_n(m_mask, last_bucket, MaskType(-1));
                m_mask[last_bucket] = _get_lead_mask(m_size & (mask_size - 1)), m_sum = m_size;
            }
            void set(size_type i) {
                size_type quot = i >> mask_width, rem = i & (mask_size - 1);
                if (m_mask[quot] >> rem & 1) return;
                m_mask[quot] |= MaskType(1) << rem, m_sum++;
            }
            void set(size_type left, size_type right) {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r) {
                    MaskType old = m_mask[l];
                    m_mask[l] |= _get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1));
                    m_sum += std::popcount(old ^ m_mask[l]);
                } else {
                    MaskType old = m_mask[l];
                    m_mask[l] |= _get_trail_mask(left & (mask_size - 1));
                    m_sum += std::popcount(old ^ m_mask[l]);
                    for (size_type i = l + 1; i < r; i++) m_sum += mask_size - std::popcount(m_mask[i]), m_mask[i] = -1;
                    old = m_mask[r];
                    m_mask[r] |= _get_lead_mask((right + 1) & (mask_size - 1));
                    m_sum += std::popcount(old ^ m_mask[r]);
                }
            }
            void reset() { std::fill_n(m_mask, (m_size + mask_size - 1) >> mask_width, MaskType(0)), m_sum = 0; }
            void reset(size_type i) {
                size_type quot = i >> mask_width, rem = i & (mask_size - 1);
                if (!(m_mask[quot] >> rem & 1)) return;
                m_mask[quot] -= MaskType(1) << rem, m_sum--;
            }
            void reset(size_type left, size_type right) {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r) {
                    MaskType old = m_mask[l];
                    m_mask[l] &= ~_get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1));
                    m_sum -= std::popcount(old ^ m_mask[l]);
                } else {
                    MaskType old = m_mask[l];
                    m_mask[l] &= ~_get_trail_mask(left & (mask_size - 1));
                    m_sum -= std::popcount(old ^ m_mask[l]);
                    for (size_type i = l + 1; i < r; i++) m_sum -= std::popcount(m_mask[i]), m_mask[i] = 0;
                    old = m_mask[r];
                    m_mask[r] &= ~_get_lead_mask((right + 1) & (mask_size - 1));
                    m_sum -= std::popcount(old ^ m_mask[r]);
                }
            }
            void flip() {
                size_type last_bucket = (m_size - 1) >> mask_width;
                for (size_type i = 0; i != last_bucket; i++) m_mask[i] = ~m_mask[i];
                m_mask[last_bucket] ^= _get_lead_mask(m_size & (mask_size - 1)), m_sum = m_size - m_sum;
            }
            void flip(size_type i) {
                size_type quot = i >> mask_width, rem = i & (mask_size - 1);
                if (!(m_mask[quot] >> rem & 1))
                    m_sum++;
                else
                    m_sum--;
                m_mask[quot] ^= MaskType(1) << rem;
            }
            void flip(size_type left, size_type right) {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r) {
                    MaskType old = m_mask[l];
                    m_mask[l] ^= _get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1));
                    m_sum += std::popcount(m_mask[l]) - std::popcount(old);
                } else {
                    MaskType old = m_mask[l];
                    m_mask[l] ^= _get_trail_mask(left & (mask_size - 1));
                    m_sum += std::popcount(m_mask[l]) - std::popcount(old);
                    for (size_type i = l + 1; i < r; i++) m_sum += mask_size - (std::popcount(m_mask[i]) << 1), m_mask[i] = ~m_mask[i];
                    old = m_mask[r];
                    m_mask[r] ^= _get_lead_mask((right + 1) & (mask_size - 1));
                    m_sum += std::popcount(m_mask[r]) - std::popcount(old);
                }
            }
            size_type count() const { return m_sum; }
            size_type count(size_type left, size_type right) const {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r)
                    return std::popcount(m_mask[l] & _get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1)));
                else {
                    size_type res = std::popcount(m_mask[l] & _get_trail_mask(left & (mask_size - 1)));
                    for (size_type i = l + 1; i < r; i++) res += std::popcount(m_mask[i]);
                    return res + std::popcount(m_mask[r] & _get_lead_mask((right + 1) & (mask_size - 1)));
                }
            }
            bool any() const { return m_sum; }
            bool any(size_type left, size_type right) const {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r)
                    return m_mask[l] & _get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1));
                else {
                    if (m_mask[l] & _get_trail_mask(left & (mask_size - 1))) return true;
                    for (size_type i = l + 1; i < r; i++)
                        if (m_mask[i]) return true;
                    return m_mask[r] & _get_lead_mask((right + 1) & (mask_size - 1));
                }
            }
            bool all() const { return m_sum == m_size; }
            bool all(size_type left, size_type right) const {
                size_type l = left >> mask_width, r = right >> mask_width;
                if (l == r)
                    return !((~m_mask[l]) & _get_mask(left & (mask_size - 1), (right + 1) & (mask_size - 1)));
                else {
                    if (((~m_mask[l]) & _get_trail_mask(left & (mask_size - 1)))) return false;
                    for (size_type i = l + 1; i < r; i++)
                        if (~m_mask[i]) return false;
                    return !((~m_mask[r]) & _get_lead_mask((right + 1) & (mask_size - 1)));
                }
            }
            size_type first_one() const {
                if (!m_sum) return -1;
                size_type i = 0;
                while (!m_mask[i]) i++;
                return (i << mask_width) + std::countr_zero(m_mask[i]);
            }
            size_type last_one() const {
                if (!m_sum) return -1;
                size_type i = (m_size - 1) >> mask_width;
                while (!m_mask[i]) i--;
                return (i << mask_width) + mask_size - std::countl_zero(m_mask[i]) - 1;
            }
            size_type first_zero() const {
                if (m_sum == m_size) return -1;
                size_type i = 0;
                while (!~m_mask[i]) i++;
                return (i << mask_width) + std::countr_zero(~m_mask[i]);
            }
            size_type last_zero() const {
                if (m_sum == m_size) return -1;
                size_type i = (m_size - 1) >> mask_width;
                MaskType a = (~m_mask[i]) & _get_lead_mask(m_size & (mask_size - 1));
                if (a) return (i << mask_width) + mask_size - std::countl_zero(a) - 1;
                while (!~m_mask[--i]) {}
                return (i << mask_width) + mask_size - std::countl_zero(~m_mask[i]) - 1;
            }
            size_type prev_one(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) >> mask_width, j = x & (mask_size - 1);
                MaskType a = m_mask[i] & _get_lead_mask(j);
                if (a) return (i << mask_width) + mask_size - std::countl_zero(a) - 1;
                while (~--i && !m_mask[i]) {};
                if (~i) return (i << mask_width) + mask_size - std::countl_zero(m_mask[i]) - 1;
                return -1;
            }
            size_type next_one(size_type x) const {
                if (x + 1 == m_size) return -1;
                size_type last_bucket = (m_size - 1) >> mask_width, i = (x + 1) >> mask_width, j = (x + 1) & (mask_size - 1);
                if (i == last_bucket) {
                    MaskType a = m_mask[i] & _get_mask(j, m_size & (mask_size - 1));
                    return a ? (i << mask_width) + std::countr_zero(a) : -1;
                }
                MaskType a = m_mask[i] & _get_trail_mask(j);
                if (a) return (i << mask_width) + std::countr_zero(a);
                while (++i <= last_bucket && !m_mask[i]) {}
                if (i <= last_bucket) return (i << mask_width) + std::countr_zero(m_mask[i]);
                return -1;
            }
            size_type prev_zero(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) >> mask_width, j = x & (mask_size - 1);
                MaskType a = (~m_mask[i]) & _get_lead_mask(j);
                if (a) return (i << mask_width) + mask_size - std::countl_zero(a) - 1;
                while (~--i && !~m_mask[i]) {};
                if (~i) return (i << mask_width) + mask_size - std::countl_zero(~m_mask[i]) - 1;
                return -1;
            }
            size_type next_zero(size_type x) const {
                if (x + 1 == m_size) return -1;
                size_type last_bucket = (m_size - 1) >> mask_width, i = (x + 1) >> mask_width, j = (x + 1) & (mask_size - 1);
                if (i == last_bucket) {
                    MaskType a = (~m_mask[i]) & _get_mask(j, m_size & (mask_size - 1));
                    return a ? (i << mask_width) + std::countr_zero(a) : -1;
                }
                MaskType a = (~m_mask[i]) & _get_trail_mask(j);
                if (a) return (i << mask_width) + std::countr_zero(a);
                while (++i <= last_bucket && !~m_mask[i]) {}
                if (i < last_bucket) return (i << mask_width) + std::countr_zero(~m_mask[i]);
                if (i == last_bucket) {
                    MaskType a = (~m_mask[i]) & _get_lead_mask(m_size & (mask_size - 1));
                    return a ? (i << mask_width) + std::countr_zero(a) : -1;
                }
                return -1;
            }
            bool at(size_type i) const { return m_mask[i >> mask_width] >> (i & (mask_size - 1)) & 1; }
            bool operator[](size_type i) const { return at(i); }
            size_type kth(size_type k) const {
                size_type i = 0;
                while (true) {
                    size_type a = std::popcount(m_mask[i]);
                    if (k < a) {
                        MaskType b = m_mask[i];
                        while (k--) b -= b & -b;
                        return (i << mask_width) + std::countr_zero(b);
                    }
                    k -= a, i++;
                }
                return -1;
            }
            Table<MaskType, N> &operator|=(const Table<MaskType, N> &rhs) {
                size_type last_bucket = (m_size - 1) >> mask_width;
                m_sum = 0;
                for (size_type i = 0; i <= last_bucket; i++) m_sum += std::popcount(m_mask[i] |= rhs.m_mask[i]);
                return *this;
            }
            Table<MaskType, N> &operator&=(const Table<MaskType, N> &rhs) {
                size_type last_bucket = (m_size - 1) >> mask_width;
                m_sum = 0;
                for (size_type i = 0; i <= last_bucket; i++) m_sum += std::popcount(m_mask[i] &= rhs.m_mask[i]);
                return *this;
            }
            Table<MaskType, N> &operator^=(const Table<MaskType, N> &rhs) {
                size_type last_bucket = (m_size - 1) >> mask_width;
                m_sum = 0;
                for (size_type i = 0; i <= last_bucket; i++) m_sum += std::popcount(m_mask[i] ^= rhs.m_mask[i]);
                return *this;
            }
            Table<MaskType, N> &operator>>=(size_type x) {
                if (x >= m_size)
                    std::fill_n(m_mask, (m_size + mask_size - 1) >> mask_width, MaskType(0)), m_sum = 0;
                else {
                    size_type last_bucket = (m_size - 1) >> mask_width, y = x >> mask_width, z = x & (mask_size - 1);
                    if (z) {
                        for (size_type i = 0; i < y; i++) m_sum -= std::popcount(m_mask[i]);
                        m_sum -= std::popcount(m_mask[y] & _get_lead_mask(z));
                        for (size_type i = 0; i + y < last_bucket; i++) m_mask[i] = ((m_mask[i + y] & _get_trail_mask(z)) >> z) | ((m_mask[i + y + 1] & _get_lead_mask(z)) << (mask_size - z));
                        m_mask[last_bucket - y] = (m_mask[last_bucket] & _get_trail_mask(z)) >> z;
                        std::fill_n(m_mask + last_bucket - y + 1, y, MaskType(0));
                    } else {
                        for (size_type i = 0; i < y; i++) m_sum -= std::popcount(m_mask[i]);
                        for (size_type i = 0; i + y <= last_bucket; i++) m_mask[i] = m_mask[i + y];
                        std::fill_n(m_mask + last_bucket - y + 1, y, MaskType(0));
                    }
                }
                return *this;
            }
            Table<MaskType, N> &operator<<=(size_type x) {
                if (x >= m_size)
                    std::fill_n(m_mask, (m_size + mask_size - 1) >> mask_width, MaskType(0)), m_sum = 0;
                else {
                    size_type last_bucket = (m_size - 1) >> mask_width, y = x >> mask_width, z = x & (mask_size - 1);
                    if (z) {
                        reset(m_size - x, m_size - 1);
                        for (size_type i = 0; i + y < last_bucket; i++) {
                            m_mask[last_bucket - i] = ((m_mask[last_bucket - i - y] & _get_lead_mask(mask_size - z)) << z) | ((m_mask[last_bucket - i - y - 1] & _get_trail_mask(mask_size - z)) >> (mask_size - z));
                        }
                        m_mask[y] = (m_mask[0] & _get_lead_mask(mask_size - z)) << z;
                        std::fill_n(m_mask, y, MaskType(0));
                    } else {
                        for (size_type i = 0; i < y; i++) m_sum -= std::popcount(m_mask[last_bucket - i]);
                        for (size_type i = 0; i + y <= last_bucket; i++) m_mask[last_bucket - i] = m_mask[last_bucket - i - y];
                        std::fill_n(m_mask, y, MaskType(0));
                    }
                }
                return *this;
            }
            friend Table<MaskType, N> operator|(const Table<MaskType, N> &lhs, const Table<MaskType, N> &rhs) {
                Table<MaskType, N> res(lhs);
                res |= rhs;
                return res;
            }
            friend Table<MaskType, N> operator&(const Table<MaskType, N> &lhs, const Table<MaskType, N> &rhs) {
                Table<MaskType, N> res(lhs);
                res &= rhs;
                return res;
            }
            friend Table<MaskType, N> operator^(const Table<MaskType, N> &lhs, const Table<MaskType, N> &rhs) {
                Table<MaskType, N> res(lhs);
                res ^= rhs;
                return res;
            }
            friend Table<MaskType, N> operator<<(const Table<MaskType, N> &o, size_type x) {
                Table<MaskType, N> res(o);
                res <<= x;
                return res;
            }
            friend Table<MaskType, N> operator>>(const Table<MaskType, N> &o, size_type x) {
                Table<MaskType, N> res(o);
                res >>= x;
                return res;
            }
        };
        template <typename Ostream, typename MaskType, size_type N>
        Ostream &operator<<(Ostream &out, const Table<MaskType, N> &x) {
            out << "{";
            for (size_type i = 0, j = 0; i < x.m_size; i++)
                if (x.at(i)) out << (j++ ? ", " : "") << i;
            return out << "}";
        }
    }
}

#endif