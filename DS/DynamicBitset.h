/*
最后修改:
20240711
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICBITSET__
#define __OY_DYNAMICBITSET__

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace DYNAMICBITSET {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        class Table {
            std::unique_ptr<mask_type[]> m_data;
            size_type m_size;
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
                if (!~mask) return MASK_SIZE;
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
            static mask_type *_new(size_type len) { return new mask_type[(len + MASK_SIZE - 1) / MASK_SIZE]{}; }
            static mask_type *_malloc(size_type len) { return new mask_type[(len + MASK_SIZE - 1) / MASK_SIZE]; }
            void _sanitize() {
                m_data[(m_size - 1) / MASK_SIZE] &= _get_lead_mask(m_size % MASK_SIZE);
            }
            size_type _next_one(size_type x) const {
                if (x + 1 >= m_size) return -1;
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                mask_type a = m_data[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                if (i == last_bucket) return -1;
                while (++i != last_bucket && !m_data[i]) {}
                size_type ctz = std::countr_zero(m_data[i]);
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            size_type _next_zero(size_type x) const {
                if (x + 1 >= m_size) return -1;
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = (x + 1) / MASK_SIZE, j = (x + 1) % MASK_SIZE;
                if (i == last_bucket) {
                    mask_type a = ~m_data[i] & _get_mask(j, m_size % MASK_SIZE);
                    return a ? (i << MASK_WIDTH) + std::countr_zero(a) : -1;
                }
                mask_type a = ~m_data[i] & _get_trail_mask(j);
                if (a) return (i << MASK_WIDTH) + std::countr_zero(a);
                while (++i != last_bucket && !~m_data[i]) {}
                if (i != last_bucket) return (i << MASK_WIDTH) + std::countr_zero(~m_data[i]);
                size_type ctz = std::countr_zero(~m_data[i] & _get_lead_mask(m_size % MASK_SIZE));
                return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
            }
            void _left_shift(size_type x) {
                if (x) {
                    size_type last_bucket = (m_size - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = last_bucket; i > y; i--) m_data[i] = ((m_data[i - y] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[i - y - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_data[y] = (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        } else {
                            for (size_type i = last_bucket; i; i--) m_data[i] = ((m_data[i] & _get_lead_mask(MASK_SIZE - z)) << z) | ((m_data[i - 1] & _get_trail_mask(MASK_SIZE - z)) >> (MASK_SIZE - z));
                            m_data[0] = (m_data[0] & _get_lead_mask(MASK_SIZE - z)) << z;
                        }
                    else
                        for (size_type i = last_bucket; i >= y; i--) m_data[i] = m_data[i - y];
                    memset(m_data.get(), 0, y * sizeof(mask_type));
                    _sanitize();
                }
            }
            void _right_shift(size_type x) {
                if (x) {
                    size_type last_bucket = (m_size - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
                    if (z)
                        if (y) {
                            for (size_type i = 0; i + y < last_bucket; i++) m_data[i] = ((m_data[i + y] & _get_trail_mask(z)) >> z) | ((m_data[i + y + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_data[last_bucket - y] = (m_data[last_bucket] & _get_trail_mask(z)) >> z;
                        } else {
                            for (size_type i = 0; i < last_bucket; i++) m_data[i] = ((m_data[i] & _get_trail_mask(z)) >> z) | ((m_data[i + 1] & _get_lead_mask(z)) << (MASK_SIZE - z));
                            m_data[last_bucket] = (m_data[last_bucket] & _get_trail_mask(z)) >> z;
                        }
                    else {
                        for (size_type i = 0; i + y < last_bucket; i++) m_data[i] = m_data[i + y];
                        m_data[last_bucket - y] = m_data[last_bucket] & _get_lead_mask(m_size % MASK_SIZE);
                    }
                    memset(m_data.get() + last_bucket - y + 1, 0, y * sizeof(mask_type));
                }
            }
        public:
            Table() = default;
            explicit Table(size_type length) { resize(length); }
            Table(const Table &rhs) : m_data(_malloc(rhs.m_size)), m_size(rhs.m_size) {
                memcpy(m_data.get(), rhs.m_data.get(), ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
            }
            Table(Table &&rhs) noexcept : m_data(std::move(rhs.m_data)), m_size(rhs.m_size) {}
            void resize(size_type length) { m_data.reset(_new(m_size = length)); }
            void regard_as(size_type length) { m_size = length; }
            Table &operator=(const Table &rhs) {
                if (this == &rhs) return *this;
                if (bool(m_data) && m_size < rhs.m_size) m_data.release();
                if (!bool(m_data)) m_data.reset(_malloc(rhs.m_size));
                m_size = rhs.m_size;
                memcpy(m_data.get(), rhs.m_data.get(), ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
                return *this;
            }
            Table &operator=(Table &&rhs) noexcept {
                if (this == &rhs) return *this;
                m_data = std::move(rhs.m_data), m_size = rhs.m_size;
                return *this;
            }
            size_type size() const { return m_size; }
            void set() {
                memset(m_data.get(), -1, ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
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
                memset(m_data.get(), 0, ((m_size + MASK_SIZE - 1) / MASK_SIZE) * sizeof(mask_type));
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
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_data[i] = ~m_data[i];
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
            size_type count() const { return count(0, m_size - 1); }
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
            bool any() const { return any(0, m_size - 1); }
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
            bool all() const { return all(0, m_size - 1); }
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
                for (size_type end = (m_size + MASK_SIZE - 1) / MASK_SIZE; ++i != end;) {
                    size_type a = std::popcount(m_data[i]);
                    if (k < a) return (i << MASK_WIDTH) + _kth(m_data[i], k);
                    k -= a;
                }
                return -1;
            }
            size_type first_one() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = 0;
                while (i != last_bucket && !m_data[i]) i++;
                if (i != last_bucket)
                    return (i << MASK_WIDTH) + std::countr_zero(m_data[i]);
                else {
                    size_type ctz = std::countr_zero(m_data[i]);
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_one() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = last_bucket;
                size_type clz = std::countl_zero(m_data[i]);
                if (clz != MASK_SIZE) return (i << MASK_WIDTH) + MASK_SIZE - clz - 1;
                while (~--i && !m_data[i]) {}
                return ~i ? (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(m_data[i]) - 1 : -1;
            }
            size_type first_zero() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = 0;
                while (i != last_bucket && !~m_data[i]) i++;
                if (i != last_bucket)
                    return (i << MASK_WIDTH) + std::countr_zero(~m_data[i]);
                else {
                    size_type ctz = std::countr_zero(~m_data[i] & _get_lead_mask(m_size % MASK_SIZE));
                    return ctz == MASK_SIZE ? -1 : (i << MASK_WIDTH) + ctz;
                }
            }
            size_type last_zero() const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE, i = last_bucket;
                size_type clz = std::countl_zero(~m_data[i] & _get_lead_mask(m_size % MASK_SIZE));
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
            size_type next_one(size_type x) const { return _next_one(x); }
            size_type prev_zero(size_type x) const {
                if (!x) return -1;
                size_type i = (x - 1) / MASK_SIZE;
                mask_type a = (~m_data[i]) & _get_lead_mask(x % MASK_SIZE);
                if (a) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(a) - 1;
                while (~--i && !~m_data[i]) {};
                if (~i) return (i << MASK_WIDTH) + MASK_SIZE - std::countl_zero(~m_data[i]) - 1;
                return -1;
            }
            size_type next_zero(size_type x) const { return _next_zero(x); }
            template <typename Callback>
            void enumerate_one(Callback &&call) const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0, j = 0; i != last_bucket; i++, j += MASK_SIZE)
                    if (m_data[i]) {
                        auto mask = m_data[i];
                        do {
                            size_type ctz = std::countr_zero(mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (mask);
                    }
                size_type j = last_bucket * MASK_SIZE;
                auto mask = m_data[last_bucket];
                while (mask) {
                    size_type ctz = std::countr_zero(mask);
                    call(j + ctz);
                    mask ^= mask_type(1) << ctz;
                }
            }
            template <typename Callback>
            void enumerate_zero(Callback &&call) const {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0, j = 0; i != last_bucket; i++, j += MASK_SIZE)
                    if (~m_data[i]) {
                        auto mask = m_data[i];
                        do {
                            size_type ctz = std::countr_zero(~mask);
                            call(j + ctz);
                            mask ^= mask_type(1) << ctz;
                        } while (~mask);
                    }
                size_type j = last_bucket * MASK_SIZE;
                auto mask = m_data[last_bucket] ^ _get_lead_mask(m_size % MASK_SIZE);
                while (mask) {
                    size_type ctz = std::countr_zero(mask);
                    call(j + ctz);
                    mask ^= mask_type(1) << ctz;
                }
            }
            Table &operator|=(const Table &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_data[i] |= rhs.m_data[i];
                return *this;
            }
            Table &operator&=(const Table &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_data[i] &= rhs.m_data[i];
                return *this;
            }
            Table &operator^=(const Table &rhs) {
                size_type last_bucket = (m_size - 1) / MASK_SIZE;
                for (size_type i = 0; i <= last_bucket; i++) m_data[i] ^= rhs.m_data[i];
                return *this;
            }
            Table &operator>>=(size_type x) {
                if (x >= m_size)
                    reset();
                else
                    _right_shift(x);
                return *this;
            }
            Table &operator<<=(size_type x) {
                if (x >= m_size)
                    reset();
                else
                    _left_shift(x);
                return *this;
            }
            void bitor_lshift(size_type x, size_type range = -1) {
                if (!range || !x || x >= m_size) return;
                range = std::min(range, m_size - x);
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
                if (!x || x >= m_size) return;
                size_type last_bucket = (m_size - 1) / MASK_SIZE, y = x / MASK_SIZE, z = x % MASK_SIZE;
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
            friend Table operator|(const Table &lhs, const Table &rhs) {
                Table res(lhs);
                res |= rhs;
                return res;
            }
            friend Table operator&(const Table &lhs, const Table &rhs) {
                Table res(lhs);
                res &= rhs;
                return res;
            }
            friend Table operator^(const Table &lhs, const Table &rhs) {
                Table res(lhs);
                res ^= rhs;
                return res;
            }
            friend Table operator<<(const Table &o, size_type x) {
                Table res(o);
                res <<= x;
                return res;
            }
            friend Table operator>>(const Table &o, size_type x) {
                Table res(o);
                res >>= x;
                return res;
            }
        };
        template <typename Ostream>
        Ostream &operator<<(Ostream &out, const Table &x) {
            out << "{";
            for (size_type i = 0, j = 0; i != x.size(); i++)
                if (x.at(i)) out << (j++ ? ", " : "") << i;
            return out << "}";
        }
    }
    using DynamicBitset = DYNAMICBITSET::Table;
}

#endif