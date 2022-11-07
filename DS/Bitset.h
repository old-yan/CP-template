#ifndef __OY_BITSET__
#define __OY_BITSET__

#include <bit>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory.h>

namespace OY {
    enum _BITBLOCK_STATE {
        BITSET_DEFAULT = 0,
        BITSET_FLIPPED = 1,
        BITSET_ZERO = 2,
        BITSET_ONE = 3
    };
    template <uint32_t _Depth = 6>
    struct _BitBlock {
        mutable uint64_t m_mask[1 << _Depth];
        mutable _BITBLOCK_STATE m_state;
        uint32_t m_sum;
        void set_inside(uint32_t i, uint32_t l, uint32_t r) {
            uint64_t old = m_mask[i];
            m_mask[i] |= r < 63 ? (1ull << (r + 1)) - (1ull << l) : -(1ull << l);
            m_sum += std::__popcount(m_mask[i] ^ old);
        }
        void reset_inside(uint32_t i, uint32_t l, uint32_t r) {
            uint64_t old = m_mask[i];
            m_mask[i] &= r < 63 ? ~((1ull << (r + 1)) - (1ull << l)) : ~-(1ull << l);
            m_sum -= std::__popcount(m_mask[i] ^ old);
        }
        void flip_inside(uint32_t i, uint32_t l, uint32_t r) {
            uint64_t old = m_mask[i];
            m_mask[i] ^= r < 63 ? (1ull << (r + 1)) - (1ull << l) : -(1ull << l);
            m_sum += std::__popcount(m_mask[i]) - std::__popcount(old);
        }
        uint32_t count_inside(uint32_t i, uint32_t l, uint32_t r) const { return std::__popcount(m_mask[i] & (r < 63 ? (1ull << (r + 1)) - (1ull << l) : -(1ull << l))); }
        void push_down() const {
            if (m_state == BITSET_DEFAULT) return;
            if (m_state == BITSET_FLIPPED) {
                m_state = BITSET_DEFAULT;
                for (auto &a : m_mask) a = ~a;
            } else if (m_state == BITSET_ZERO) {
                m_state = BITSET_DEFAULT;
                memset(m_mask, 0, sizeof(m_mask));
            } else {
                m_state = BITSET_DEFAULT;
                memset(m_mask, -1, sizeof(m_mask));
            }
        }
        void update_sum() {
            m_sum = 0;
            for (auto &a : m_mask) m_sum += std::__popcount(a);
        }
        _BitBlock() : m_mask{0}, m_state(BITSET_DEFAULT), m_sum(0) {}
        int set() {
            m_state = BITSET_ONE;
            uint32_t old = m_sum;
            m_sum = 1 << (_Depth + 6);
            return m_sum - old;
        }
        int set(uint32_t __i) {
            if (m_state == BITSET_ONE) return 0;
            if (m_state == BITSET_DEFAULT) {
                if (m_mask[__i >> 6] >> (__i & 63) & 1) return 0;
            } else if (m_state == BITSET_FLIPPED) {
                if (!(m_mask[__i >> 6] >> (__i & 63) & 1)) return 0;
            } else {
                m_state = BITSET_DEFAULT;
                memset(m_mask, 0, sizeof(m_mask));
            }
            m_mask[__i >> 6] ^= 1ull << (__i & 63);
            m_sum++;
            return 1;
        }
        int set(uint32_t __left, uint32_t __right) {
            if (m_state == BITSET_ONE) return 0;
            uint32_t old = m_sum;
            push_down();
            if (uint32_t l = __left >> 6, r = __right >> 6; l == r)
                set_inside(l, __left & 63, __right & 63);
            else {
                set_inside(l, __left & 63, 63);
                for (uint32_t i = l + 1; i < r; i++) {
                    uint64_t old = m_mask[i];
                    m_mask[i] = -1ull;
                    m_sum += std::__popcount(m_mask[i] ^ old);
                }
                set_inside(r, 0, __right & 63);
            }
            return m_sum - old;
        }
        int reset() {
            m_state = BITSET_ZERO;
            uint32_t old = m_sum;
            m_sum = 0;
            return -old;
        }
        int reset(uint32_t __i) {
            if (m_state == BITSET_ZERO) return 0;
            if (m_state == BITSET_DEFAULT) {
                if (!(m_mask[__i >> 6] >> (__i & 63) & 1)) return 0;
            } else if (m_state == BITSET_FLIPPED) {
                if (m_mask[__i >> 6] >> (__i & 63) & 1) return 0;
            } else {
                m_state = BITSET_DEFAULT;
                memset(m_mask, -1, sizeof(m_mask));
            }
            m_mask[__i >> 6] ^= 1ull << (__i & 63);
            m_sum--;
            return -1;
        }
        int reset(uint32_t __left, uint32_t __right) {
            if (m_state == BITSET_ZERO) return 0;
            uint32_t old = m_sum;
            push_down();
            if (uint32_t l = __left >> 6, r = __right >> 6; l == r)
                reset_inside(l, __left & 63, __right & 63);
            else {
                reset_inside(l, __left & 63, 63);
                for (uint32_t i = l + 1; i < r; i++) {
                    m_sum -= std::__popcount(m_mask[i]);
                    m_mask[i] = 0;
                }
                reset_inside(r, 0, __right & 63);
            }
            return m_sum - old;
        }
        int flip() {
            if (m_state == BITSET_DEFAULT)
                m_state = BITSET_FLIPPED;
            else if (m_state == BITSET_FLIPPED)
                m_state = BITSET_DEFAULT;
            else if (m_state == BITSET_ONE)
                m_state = BITSET_ZERO;
            else
                m_state = BITSET_ONE;
            m_sum = (1 << (_Depth + 6)) - m_sum;
            return m_sum * 2 - (1 << (_Depth + 6));
        }
        int flip(uint32_t __i) {
            bool add;
            if (m_state == BITSET_FLIPPED) {
                m_mask[__i >> 6] ^= 1ull << (__i & 63);
                add = !(m_mask[__i >> 6] >> (__i & 63) & 1);
            } else {
                if (m_state != BITSET_DEFAULT) push_down();
                m_mask[__i >> 6] ^= 1ull << (__i & 63);
                add = m_mask[__i >> 6] >> (__i & 63) & 1;
            }
            if (add) {
                m_sum++;
                return 1;
            } else {
                m_sum--;
                return -1;
            }
        }
        int flip(uint32_t __left, uint32_t __right) {
            uint32_t old = m_sum;
            push_down();
            if (uint32_t l = __left >> 6, r = __right >> 6; l == r)
                flip_inside(l, __left & 63, __right & 63);
            else {
                flip_inside(l, __left & 63, 63);
                for (uint32_t i = l + 1; i < r; i++) {
                    m_sum += 64 - std::__popcount(m_mask[i]) * 2;
                    m_mask[i] = ~m_mask[i];
                }
                flip_inside(r, 0, __right & 63);
            }
            return m_sum - old;
        }
        uint32_t first() const {
            if (!m_sum) return -1;
            if (m_state == BITSET_DEFAULT) {
                uint32_t i = 0;
                while (!m_mask[i]) i++;
                return (i << 6) + std::__countr_zero(m_mask[i]);
            } else if (m_state == BITSET_FLIPPED) {
                uint32_t i = 0;
                while (!~m_mask[i]) i++;
                return (i << 6) + std::__countr_zero(~m_mask[i]);
            } else
                return 0;
        }
        uint32_t prev(uint32_t __i) const {
            if (m_state == BITSET_DEFAULT) {
                uint32_t i = __i >> 6;
                if (auto a = m_mask[i] & (1ull << (__i & 63)) - 1) return (i << 6) + 63 - std::__countl_zero(a);
                while (~--i && !m_mask[i]) {}
                return ~i ? (i << 6) + 63 - std::__countl_zero(m_mask[i]) : -1;
            } else if (m_state == BITSET_FLIPPED) {
                uint32_t i = __i >> _Depth;
                if (auto a = ~m_mask[i] & (1ull << (__i & 63)) - 1) return (i << 6) + 63 - std::__countl_zero(a);
                while (~--i && !~m_mask[i]) {}
                return ~i ? (i << 6) + 63 - std::__countl_zero(~m_mask[i]) : -1;
            } else if (m_state == BITSET_ONE)
                return __i - 1;
            else
                return -1;
        }
        uint32_t next(uint32_t __i) const {
            if (m_state == BITSET_DEFAULT) {
                uint32_t i = __i >> 6;
                if ((__i & 63) < 63)
                    if (auto a = m_mask[i] & -1ull << ((__i & 63) + 1)) return (i << 6) + std::__countr_zero(a);
                while (++i < 1 << _Depth && !m_mask[i]) {}
                return i < 1 << _Depth ? (i << 6) + std::__countr_zero(m_mask[i]) : -1;
            } else if (m_state == BITSET_FLIPPED) {
                uint32_t i = __i >> 6;
                if ((__i & 63) < 63)
                    if (auto a = ~m_mask[i] & -1ull << ((__i & 63) + 1)) return (i << 6) + std::__countr_zero(a);
                while (++i < 1 << _Depth && !~m_mask[i]) {}
                return i < 1 << _Depth ? (i << 6) + std::__countr_zero(~m_mask[i]) : -1;
            } else if (m_state == BITSET_ONE)
                return __i + 1 < 1 << (_Depth + 6) ? __i + 1 : -1;
            else
                return -1;
        }
        uint32_t last() const {
            if (!m_sum) return -1;
            if (m_state == BITSET_DEFAULT) {
                uint32_t i = (1 << _Depth) - 1;
                while (!m_mask[i]) i--;
                return (i << 6) + 63 - std::__countl_zero(m_mask[i]);
            } else if (m_state == BITSET_FLIPPED) {
                uint32_t i = (1 << _Depth) - 1;
                while (!~m_mask[i]) i--;
                return (i << 6) + 63 - std::__countl_zero(~m_mask[i]);
            } else
                return (1 << (_Depth + 6)) - 1;
        }
        uint32_t count() const { return m_sum; }
        uint32_t count(uint32_t __left, uint32_t __right) const {
            if (m_state == BITSET_ZERO)
                return 0;
            else if (m_state == BITSET_ONE)
                return __right - __left + 1;
            else {
                uint32_t l = __left >> 6, r = __right >> 6, sum = 0;
                if (l == r)
                    sum = count_inside(l, __left & 63, __right & 63);
                else {
                    sum = count_inside(l, __left & 63, 63);
                    for (uint32_t i = l + 1; i < r; i++) sum += std::__popcount(m_mask[i]);
                    sum += count_inside(r, 0, __right & 63);
                }
                return m_state == BITSET_DEFAULT ? sum : __right - __left + 1 - sum;
            }
        }
        bool at(uint32_t __i) const {
            if (m_state == BITSET_DEFAULT)
                return m_mask[__i >> 6] >> (__i & 63) & 1;
            else if (m_state == BITSET_FLIPPED)
                return !(m_mask[__i >> 6] >> (__i & 63) & 1);
            else
                return m_state == BITSET_ONE;
        }
        bool operator[](uint32_t __i) const { return at(__i); }
        bool all() const { return m_sum == 1 << (_Depth + 6); }
        bool any() const { return m_sum; }
        _BitBlock<_Depth> &operator&=(const _BitBlock<_Depth> &other) {
            push_down();
            other.push_down();
            for (uint32_t i = 0; i < 1 << _Depth; i++) m_mask[i] &= other.m_mask[i];
            update_sum();
            return *this;
        }
        _BitBlock<_Depth> &operator|=(const _BitBlock<_Depth> &other) {
            push_down();
            other.push_down();
            for (uint32_t i = 0; i < 1 << _Depth; i++) m_mask[i] |= other.m_mask[i];
            update_sum();
            return *this;
        }
        _BitBlock<_Depth> &operator^=(const _BitBlock<_Depth> &other) {
            push_down();
            other.push_down();
            for (uint32_t i = 0; i < 1 << _Depth; i++) m_mask[i] ^= other.m_mask[i];
            update_sum();
            return *this;
        }
        friend _BitBlock<_Depth> operator&(const _BitBlock<_Depth> &a, const _BitBlock<_Depth> &b) {
            _BitBlock<_Depth> res(a);
            a &= b;
            return a;
        }
        friend _BitBlock<_Depth> operator|(const _BitBlock<_Depth> &a, const _BitBlock<_Depth> &b) {
            _BitBlock<_Depth> res(a);
            a |= b;
            return a;
        }
        friend _BitBlock<_Depth> operator^(const _BitBlock<_Depth> &a, const _BitBlock<_Depth> &b) {
            _BitBlock<_Depth> res(a);
            a ^= b;
            return a;
        }
    };
    template <uint32_t _Depth = 6>
    struct Bitset {
        using bitset = Bitset<_Depth>;
        static constexpr uint32_t subbit = _Depth + 6, subsize = 1 << subbit, submask = subsize - 1;
        std::vector<_BitBlock<_Depth>> m_sub;
        uint32_t m_length, m_sum;
        void updateSum() {
            m_sum = 0;
            for (auto &sub : m_sub) m_sum += sub.m_sum;
        }
        Bitset(uint32_t __length = 0) { resize(__length); }
        void resize(uint32_t __length) {
            if (!(m_length = __length)) return;
            m_sub.clear();
            m_sub.resize(((__length - 1) >> subbit) + 1);
            m_sum = 0;
        }
        void set() {
            for (uint32_t i = 0; i + 1 < m_sub.size(); i++) m_sub[i].set();
            m_sub.back().set(0, (m_length - 1) & submask);
            m_sum = m_length;
        }
        void set(uint32_t __i) { m_sum += m_sub[__i >> subbit].set(__i & submask); }
        void set(uint32_t __left, uint32_t __right) {
            uint32_t l = __left >> subbit, r = __right >> subbit;
            if (l == r)
                m_sum += m_sub[l].set(__left & submask, __right & submask);
            else {
                m_sum += m_sub[l].set(__left & submask, submask);
                for (uint32_t i = l + 1; i < r; i++) m_sum += m_sub[i].set();
                m_sum += m_sub[r].set(0, __right & submask);
            }
        }
        void reset() {
            for (auto &sub : m_sub) sub.reset();
            m_sum = 0;
        }
        void reset(uint32_t __i) { m_sum += m_sub[__i >> subbit].reset(__i & submask); }
        void reset(uint32_t __left, uint32_t __right) {
            uint32_t l = __left >> subbit, r = __right >> subbit;
            if (l == r)
                m_sum += m_sub[l].reset(__left & submask, __right & submask);
            else {
                m_sum += m_sub[l].reset(__left & submask, submask);
                for (uint32_t i = l + 1; i < r; i++) m_sum += m_sub[i].reset();
                m_sum += m_sub[r].reset(0, __right & submask);
            }
        }
        void flip() {
            for (uint32_t i = 0; i + 1 < m_sub.size(); i++) m_sub[i].flip();
            m_sub.back().flip(0, (m_length - 1) & submask);
            m_sum = m_length - m_sum;
        }
        void flip(uint32_t __i) { m_sum += m_sub[__i >> subbit].flip(__i & submask); }
        void flip(uint32_t __left, uint32_t __right) {
            uint32_t l = __left >> subbit, r = __right >> subbit;
            if (l == r)
                m_sum += m_sub[l].flip(__left & submask, __right & submask);
            else {
                m_sum += m_sub[l].flip(__left & submask, submask);
                for (uint32_t i = l + 1; i < r; i++) m_sum += m_sub[i].flip();
                m_sum += m_sub[r].flip(0, __right & submask);
            }
        }
        uint32_t first() const {
            if (!m_sum) return -1;
            uint32_t i = 0;
            while (!m_sub[i].any()) i++;
            return (i << subbit) + m_sub[i].first();
        }
        uint32_t prev(uint32_t __i) const {
            uint32_t i = __i >> subbit;
            if (uint32_t j = m_sub[i].prev(__i & submask); ~j) return (i << subbit) + j;
            while (~--i && !m_sub[i].any()) {}
            return ~i ? (i << subbit) + m_sub[i].last() : -1;
        }
        uint32_t next(uint32_t __i) const {
            uint32_t i = __i >> subbit;
            if (uint32_t j = m_sub[i].next(__i & submask); ~j) return (i << subbit) + j;
            while (++i < m_sub.size() && !m_sub[i].any()) {}
            return i < m_sub.size() ? (i << subbit) + m_sub[i].first() : -1;
        }
        uint32_t last() const {
            if (!m_sum) return -1;
            uint32_t i = m_sub.size() - 1;
            while (!m_sub[i].any()) i--;
            return (i << subbit) + m_sub[i].last();
        }
        uint32_t count() const { return m_sum; }
        uint32_t count(uint32_t __left, uint32_t __right) const {
            uint32_t l = __left >> subbit, r = __right >> subbit;
            if (l == r)
                return m_sub[l].count(__left & submask, __right & submask);
            else {
                uint32_t sum = m_sub[l].count(__left & submask, submask);
                for (uint32_t i = l + 1; i < r; i++) sum += m_sub[i].count();
                return sum + m_sub[r].count(0, __right & submask);
            }
        }
        bool at(uint32_t __i) const { return m_sub[__i >> subbit].at(__i & submask); }
        bool operator[](uint32_t __i) const { return at(__i); }
        bool all() const { return count() == m_length; }
        bool any() const { return count(); }
        bitset &operator&=(const bitset &other) {
            assert(m_length == other.m_length);
            for (uint32_t i = 0; i < m_sub.size(); i++) m_sub[i] &= other.m_sub[i];
            updateSum();
            return *this;
        };
        bitset &operator|=(const bitset &other) {
            assert(m_length == other.m_length);
            for (uint32_t i = 0; i < m_sub.size(); i++) m_sub[i] |= other.m_sub[i];
            updateSum();
            return *this;
        };
        bitset &operator^=(const bitset &other) {
            assert(m_length == other.m_length);
            for (uint32_t i = 0; i < m_sub.size(); i++) m_sub[i] ^= other.m_sub[i];
            updateSum();
            return *this;
        };
        friend bitset operator&(const bitset &a, const bitset &b) {
            bitset res(a);
            res &= b;
            return res;
        }
        friend bitset operator|(const bitset &a, const bitset &b) {
            bitset res(a);
            res |= b;
            return res;
        }
        friend bitset operator^(const bitset &a, const bitset &b) {
            bitset res(a);
            res ^= b;
            return res;
        }
    };
}

#endif