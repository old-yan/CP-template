/*
最后修改:
20240824
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICBITSET__
#define __OY_STATICBITSET__

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace STATICBITSET {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <size_type N = 1000>
        class Table {
            mask_type m_data[(N + MASK_SIZE - 1) / MASK_SIZE];
            static size_type _conti32(mask_type mask) {
                size_type cur = 32;
                if (mask_type y = mask & (mask << 16)) mask = y, cur += 16;
                if (mask_type y = mask & (mask << 8)) mask = y, cur += 8;
                if (mask_type y = mask & (mask << 4)) mask = y, cur += 4;
                if (mask_type y = mask & (mask << 2)) mask = y, cur += 2;
                if (mask_type y = mask & (mask << 1)) mask = y, cur += 1;
                return cur;
            }
            static size_type _conti16(mask_type mask) {
                size_type cur = 16;
                if (mask_type y = mask & (mask << 8)) mask = y, cur += 8;
                if (mask_type y = mask & (mask << 4)) mask = y, cur += 4;
                if (mask_type y = mask & (mask << 2)) mask = y, cur += 2;
                if (mask_type y = mask & (mask << 1)) mask = y, cur += 1;
                return cur;
            }
            static size_type _conti8(mask_type mask) {
                size_type cur = 8;
                if (mask_type y = mask & (mask << 4)) mask = y, cur += 4;
                if (mask_type y = mask & (mask << 2)) mask = y, cur += 2;
                if (mask_type y = mask & (mask << 1)) mask = y, cur += 1;
                return cur;
            }
            static size_type _conti4(mask_type mask) {
                size_type cur = 4;
                if (mask_type y = mask & (mask << 2)) mask = y, cur += 2;
                if (mask_type y = mask & (mask << 1)) mask = y, cur += 1;
                return cur;
            }
            static size_type _conti2(mask_type mask) {
                size_type cur = 2;
                if (mask_type y = mask & (mask << 1)) mask = y, cur += 1;
                return cur;
            }
            static size_type _conti(mask_type mask) {
                if (!mask) return 0;
                if (!~mask) return 64;
                if (mask_type y = mask & (mask << 1))
                    mask = y;
                else
                    return 1;
                if (mask_type y = mask & (mask << 2))
                    mask = y;
                else
                    return _conti2(mask);
                if (mask_type y = mask & (mask << 4))
                    mask = y;
                else
                    return _conti4(mask);
                if (mask_type y = mask & (mask << 8))
                    mask = y;
                else
                    return _conti8(mask);
                if (mask_type y = mask & (mask << 16))
                    return _conti32(y);
                else
                    return _conti16(mask);
            }
            static size_type _conti_zeros(mask_type mask) { return _conti(~mask); }
            static mask_type _get_mask(size_type l, size_type r) { return r ? (mask_type(1) << r) - (mask_type(1) << l) : -(mask_type(1) << l); }
            static mask_type _get_trail_mask(size_type l) { return -(mask_type(1) << l); }
            static mask_type _get_lead_mask(size_type r) { return r ? (mask_type(1) << r) - 1 : -1; }
            static size_type _kth(mask_type x, size_type k) {
                while (k--) x -= x & -x;
                return std::countr_zero(x);
            }
            void _sanitize() {
                if constexpr (N % MASK_SIZE) m_data[(N - 1) / MASK_SIZE] &= _get_lead_mask(N % MASK_SIZE);
            }
            size_type _next_one_small(size_type x) const {
                if (x + 1 >= N) return -1;
                mask_type a = m_data[0] & _get_trail_mask(x + 1);
                return a ? std::countr_zero(a) : -1;
            }
            size_type _next_one_big(size_type x) const {
                if (x + 1 >= N) return -1;
                size_type i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                mask_type a = m_data[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                if (i == _last()) return -1;
                while (++i != _last() && !m_data[i]) {}
                size_type ctz = std::countr_zero(m_data[i]);
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            size_type _next_zero_small(size_type x) const {
                if (x + 1 >= N) return -1;
                mask_type a = ~m_data[0] & _get_mask(x + 1, N % MASK_SIZE);
                return a ? std::countr_zero(a) : -1;
            }
            size_type _next_zero_big(size_type x) const {
                if (x + 1 >= N) return -1;
                size_type i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                if (i == _last()) {
                    mask_type a = ~m_data[i] & _get_mask(j, N % MASK_SIZE);
                    return a ? (i << MASK_WIDTH) + std::countr_zero(a) : -1;
                }
                mask_type a = ~m_data[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                while (++i != _last() && !~m_data[i]) {}
                if (i != _last()) return (i << MASK_WIDTH) + std::countr_zero(~m_data[i]);
                size_type ctz = std::countr_zero(~m_data[i] & _get_lead_mask(N % MASK_SIZE));
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            template <size_type I>
            void _set_small() {
                m_data[I / MASK_SIZE] = -1;
                if constexpr (I + MASK_SIZE < N) _set_small<I + MASK_SIZE>();
            }
            template <size_type I>
            void _reset_small() {
                m_data[I / MASK_SIZE] = 0;
                if constexpr (I + MASK_SIZE < N) _reset_small<I + MASK_SIZE>();
            }
            template <size_type I>
            size_type _count_small() const {
                if constexpr (I + MASK_SIZE < N)
                    return std::popcount(m_data[I / MASK_SIZE]) + _count_small<I + MASK_SIZE>();
                else
                    return std::popcount(m_data[I / MASK_SIZE]);
            }
            void _left_shift(size_type x) {
                if (x) {
                    size_type y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = _last(); i > y; i--) m_data[i] = ((m_data[i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_data[y] = (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        } else {
                            for (size_type i = _last(); i; i--) m_data[i] = ((m_data[i] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[i - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_data[0] = (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        }
                    else
                        for (size_type i = _last(); i >= y; i--) m_data[i] = m_data[i - y];
                    std::fill_n(m_data, y, mask_type(0));
                    _sanitize();
                }
            }
            void _right_shift(size_type x) {
                if (x) {
                    size_type y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = 0; i + y < _last(); i++) m_data[i] = ((m_data[i + y] & _get_trail_mask(z)) >> z) | ((m_data[i + y + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_data[_last() - y] = (m_data[_last()] & _get_trail_mask(z)) >> z;
                        } else {
                            for (size_type i = 0; i < _last(); i++) m_data[i] = ((m_data[i] & _get_trail_mask(z)) >> z) | ((m_data[i + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_data[_last()] = (m_data[_last()] & _get_trail_mask(z)) >> z;
                        }
                    else {
                        for (size_type i = 0; i + y < _last(); i++) m_data[i] = m_data[i + y];
                        m_data[_last() - y] = m_data[_last()] & _get_lead_mask(N % MASK_SIZE);
                    }
                    std::fill_n(m_data + _last() - y + 1, y, mask_type(0));
                }
            }
            static constexpr size_type _last() { return (N - 1) / MASK_SIZE; }
        public:
            static constexpr size_type size() { return N; }
            void set() {
                if constexpr (N <= MASK_SIZE * 8)
                    _set_small<0>();
                else
                    memset(m_data, -1, ((N + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
                _sanitize();
            }
            void set(size_type i) { m_data[i / MASK_SIZE] |= mask_type(1) << (i % MASK_SIZE); }
            void set(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_data[l] |= _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_data[l] |= _get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i != r; i++) m_data[i] = -1;
                    m_data[r] |= _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            void reset() {
                if constexpr (N <= MASK_SIZE * 8)
                    _reset_small<0>();
                else
                    memset(m_data, 0, ((N + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
            }
            void reset(size_type i) { m_data[i / MASK_SIZE] &= ~(mask_type(1) << (i % MASK_SIZE)); }
            void reset(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_data[l] &= ~_get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_data[l] &= ~_get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i != r; i++) m_data[i] = 0;
                    m_data[r] &= ~_get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            void flip() {
                for (size_type i = 0; i <= _last(); i++) m_data[i] = ~m_data[i];
                _sanitize();
            }
            void flip(size_type i) { m_data[i / MASK_SIZE] ^= mask_type(1) << (i % MASK_SIZE); }
            void flip(size_type left, size_type right) {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    m_data[l] ^= _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    m_data[l] ^= _get_trail_mask(left % MASK_SIZE);
                    for (size_type i = l + 1; i < r; i++) m_data[i] = ~m_data[i];
                    m_data[r] ^= _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            size_type count() const {
                if constexpr (N <= MASK_SIZE * 4)
                    return _count_small<0>();
                else
                    return count(0, N - 1);
            }
            size_type count(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return std::popcount(m_data[l] & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    size_type res = std::popcount(m_data[l] & _get_trail_mask(left % MASK_SIZE));
                    for (size_type i = l + 1; i != r; i++) res += std::popcount(m_data[i]);
                    return res + std::popcount(m_data[r] & _get_lead_mask((right + 1) % MASK_SIZE));
                }
            }
            bool any() const { return any(0, N - 1); }
            bool any(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return m_data[l] & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE);
                else {
                    if (m_data[l] & _get_trail_mask(left % MASK_SIZE)) return true;
                    for (size_type i = l + 1; i != r; i++)
                        if (m_data[i]) return true;
                    return m_data[r] & _get_lead_mask((right + 1) % MASK_SIZE);
                }
            }
            bool all() const { return all(0, N - 1); }
            bool all(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return !((~m_data[l]) & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    if (((~m_data[l]) & _get_trail_mask(left % MASK_SIZE))) return false;
                    for (size_type i = l + 1; i != r; i++)
                        if (~m_data[i]) return false;
                    return !((~m_data[r]) & _get_lead_mask((right + 1) % MASK_SIZE));
                }
            }
            size_type first_one() const {
                size_type i = 0;
                while (i != _last() && !m_data[i]) i++;
                if (i != _last())
                    return (i << MASK_WIDTH) + std::countr_zero(m_data[i]);
                else {
                    size_type ctz = std::countr_zero(m_data[i]);
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_one() const {
                size_type i = _last();
                size_type clz = std::countl_zero(m_data[i]);
                if (clz != MASK_SIZE) return (i << MASK_WIDTH) + MASK_SIZE - clz - 1;
                while (~--i && !m_data[i]) {}
                return ~i ? (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(m_data[i]) - 1 : -1;
            }
            size_type first_zero() const {
                size_type i = 0;
                while (i != _last() && !~m_data[i]) i++;
                if (i != _last())
                    return (i << MASK_WIDTH) + std::countr_zero(~m_data[i]);
                else {
                    size_type ctz = std::countr_zero(~m_data[i] & _get_lead_mask(N % MASK_SIZE));
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_zero() const {
                size_type i = _last();
                size_type clz = std::countl_zero(~m_data[i] & _get_lead_mask(N % MASK_SIZE));
                if (clz != MASK_SIZE) return (i << MASK_WIDTH) + MASK_SIZE - clz - 1;
                while (~--i && !~m_data[i]) {}
                return ~i ? (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(~m_data[i]) - 1 : -1;
            }
            size_type prev_one(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) / MASK_SIZE;
                mask_type a = m_data[i] & _get_lead_mask(x % MASK_SIZE);
                if (a) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(a) - 1;
                while (~--i && !m_data[i]) {};
                if (~i) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(m_data[i]) - 1;
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
                mask_type a = (~m_data[i]) & _get_lead_mask(x % MASK_SIZE);
                if (a) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(a) - 1;
                while (~--i && !~m_data[i]) {};
                if (~i) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(~m_data[i]) - 1;
                return -1;
            }
            size_type next_zero(size_type x) const {
                if constexpr (N <= MASK_SIZE)
                    return _next_zero_small(x);
                else
                    return _next_zero_big(x);
            }
            bool at(size_type i) const { return m_data[i / MASK_SIZE] >> (i % MASK_SIZE) & 1; }
            bool operator[](size_type i) const { return at(i); }
            size_type longest_ones(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return _conti(m_data[l] & _get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    mask_type x = m_data[l] & _get_trail_mask(left % MASK_SIZE);
                    size_type mx = _conti(x), lst = std::countl_zero(~x);
                    for (size_type i = l + 1; i != r; i++) {
                        mask_type x = m_data[i];
                        mx = std::max({mx, lst + std::countr_zero(~x), _conti(x)}), lst = (~x) ? std::countl_zero(~x) : MASK_SIZE + lst;
                    }
                    x = m_data[r] & _get_lead_mask((right + 1) % MASK_SIZE);
                    return std::max({mx, lst + std::countr_zero(~x), _conti(x)});
                }
            }
            size_type longest_zeros(size_type left, size_type right) const {
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return _conti_zeros(m_data[l] | ~_get_mask(left % MASK_SIZE, (right + 1) % MASK_SIZE));
                else {
                    mask_type x = m_data[l] | ~_get_trail_mask(left % MASK_SIZE);
                    size_type mx = _conti_zeros(x), lst = std::countl_zero(x);
                    for (size_type i = l + 1; i != r; i++) {
                        mask_type x = m_data[i];
                        mx = std::max({mx, lst + std::countr_zero(x), _conti_zeros(x)}), lst = x ? std::countl_zero(x) : MASK_SIZE + lst;
                    }
                    x = m_data[r] | ~_get_lead_mask((right + 1) % MASK_SIZE);
                    return std::max({mx, lst + std::countr_zero(x), _conti_zeros(x)});
                }
            }
            size_type kth(size_type k, size_type pos = 0) const {
                size_type i = pos / MASK_SIZE;
                mask_type x = m_data[i] & _get_trail_mask(pos % MASK_SIZE);
                size_type a = std::popcount(x);
                if (k < a) return (i << MASK_WIDTH) + _kth(m_data[i] & _get_trail_mask(pos % MASK_SIZE), k);
                k -= a;
                while (++i <= _last()) {
                    size_type a = std::popcount(m_data[i]);
                    if (k < a) return (i << MASK_WIDTH) + _kth(m_data[i], k);
                    k -= a;
                }
                return -1;
            }
            void bitor_lshift(size_type x, size_type range = N) {
                if (!range || !x || x >= N) return;
                range = std::min(range, N - x);
                size_type last_bucket = (range - 1 + x) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                if (z)
                    if (y == last_bucket)
                        m_data[y] |= (m_data[0] & _get_lead_mask(range)) << z;
                    else {
                        m_data[last_bucket] |= (((m_data[last_bucket - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[last_bucket - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z))) & _get_lead_mask((range + x) & (MASK_SIZE - 1));
                        for (size_type i = 1; i + y < last_bucket; i++) m_data[last_bucket - i] |= ((m_data[last_bucket - i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[last_bucket - i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                        m_data[y] |= (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                    }
                else {
                    m_data[last_bucket] |= m_data[last_bucket - y] & _get_lead_mask((range + x) & (MASK_SIZE - 1));
                    for (size_type i = 1; i + y <= last_bucket; i++) m_data[last_bucket - i] |= m_data[last_bucket - i - y];
                }
                _sanitize();
            }
            void bitor_lshift_unbounded(size_type x) {
                if (!x || x >= N) return;
                size_type last_bucket = (N - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                if (!z)
                    for (size_type i = y; i <= last_bucket; i++) m_data[i] |= m_data[i - y];
                else if (y) {
                    m_data[y] |= (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                    for (size_type i = y + 1; i <= last_bucket; i++) m_data[i] |= ((m_data[i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                } else {
                    mask_type pre{};
                    for (size_type i = 0; i <= last_bucket; i++) {
                        m_data[i] |= pre;
                        while ((m_data[i] << x) & ~m_data[i]) m_data[i] |= m_data[i] << x;
                        pre = m_data[i] >> (MASK_SIZE - z);
                    }
                }
                _sanitize();
            }
            Table<N> &operator|=(const Table<N> &rhs) {
                for (size_type i = 0; i <= _last(); i++) m_data[i] |= rhs.m_data[i];
                return *this;
            }
            Table<N> &operator&=(const Table<N> &rhs) {
                for (size_type i = 0; i <= _last(); i++) m_data[i] &= rhs.m_data[i];
                return *this;
            }
            Table<N> &operator^=(const Table<N> &rhs) {
                for (size_type i = 0; i <= _last(); i++) m_data[i] ^= rhs.m_data[i];
                return *this;
            }
            Table<N> &operator>>=(size_type x) {
                if (x >= N)
                    reset();
                else
                    _right_shift(x);
                return *this;
            }
            Table<N> &operator<<=(size_type x) {
                if (x >= N)
                    reset();
                else
                    _left_shift(x);
                return *this;
            }
            template <typename Callback>
            void enumerate_one(Callback &&call) const {
                for (size_type i = 0, j = 0; i != _last(); i++, j += MASK_SIZE)
                    if (m_data[i]) {
                        auto mask = m_data[i];
                        do {
                            size_type ctz = std::countr_zero(mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (mask);
                    }
                size_type j = _last() * MASK_SIZE;
                auto mask = m_data[_last()];
                while (mask) {
                    size_type ctz = std::countr_zero(mask);
                    call(j + ctz);
                    mask ^= mask_type(1) << ctz;
                }
            }
            template <typename Callback>
            void enumerate_zero(Callback &&call) const {
                for (size_type i = 0, j = 0; i != _last(); i++, j += MASK_SIZE)
                    if (~m_data[i]) {
                        auto mask = m_data[i];
                        do {
                            size_type ctz = std::countr_zero(~mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (~mask);
                    }
                size_type j = _last() * MASK_SIZE;
                auto mask = m_data[_last()] ^ _get_lead_mask(N % MASK_SIZE);
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
            for (size_type i = 0, j = 0; i != x.size(); i++)
                if (x.at(i)) out << (j++ ? ", " : "") << i;
            return out << "}";
        }
    }
    template <STATICBITSET::size_type N>
    using StaticBitset = STATICBITSET::Table<N>;
}

#endif