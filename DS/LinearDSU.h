/*
最后修改:
20240424
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LINEARDISJOINTUNION__
#define __OY_LINEARDISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LDSU {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <bool MaintainGroupSize>
        class Table {
            std::vector<mask_type> m_masks;
            mutable std::vector<size_type> m_tail, m_group_size;
            size_type m_size, m_group_cnt;
            size_type _find(size_type q) const { return m_masks[q] ? q : (m_tail[q] = _find(m_tail[q])); }
        public:
            Table(size_type n = 0) { resize(n); }
            size_type size() const { return m_size; }
            void resize(size_type n) {
                if (!(m_size = n)) return;
                m_group_cnt = m_size;
                m_masks.assign((m_size + MASK_SIZE - 1) >> MASK_WIDTH, -1), m_tail.resize((m_size + MASK_SIZE - 1) >> MASK_WIDTH);
                if constexpr (MaintainGroupSize) m_group_size.assign(m_size, 1);
                for (size_t i = 0; i != m_tail.size(); i++) m_tail[i] = i;
            }
            size_type find_head(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                size_type tail = find_tail(i);
                return tail - m_group_size[tail] + 1;
            }
            size_type find_tail(size_type i) const {
                size_type quot = i >> MASK_WIDTH, rem = i & (MASK_SIZE - 1);
                if (m_masks[quot] >> rem & mask_type(1))
                    return i;
                else if (mask_type mask = m_masks[quot] & -(mask_type(1) << rem))
                    return (quot << MASK_WIDTH) ^ std::countr_zero(mask);
                else {
                    m_tail[quot] = _find(m_tail[quot]);
                    return (m_tail[quot] << MASK_WIDTH) ^ std::countr_zero(m_masks[m_tail[quot]]);
                }
            }
            size_type find_prev(size_type i) const { return find_head(i) - 1; }
            size_type find_next(size_type i) const { return find_tail(i) + 1; }
            template <bool IsTail = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsTail)
                    return m_group_size[i];
                else
                    return m_group_size[find_tail(i)];
            }
            bool unite_after(size_type i) {
                size_type quot = i >> MASK_WIDTH, rem = i & (MASK_SIZE - 1);
                if (m_masks[quot] >> rem & mask_type(1)) {
                    m_masks[quot] &= ~(mask_type(1) << rem);
                    if (rem + 1 == MASK_SIZE) m_tail[quot] = m_masks[quot + 1] ? quot + 1 : m_tail[quot + 1];
                    if constexpr (MaintainGroupSize) m_group_size[find_tail(i)] += m_group_size[i];
                    m_group_cnt--;
                    return true;
                } else
                    return false;
            }
            bool in_same_group(size_type left, size_type right) const { return find_tail(left) >= right; }
            bool is_head(size_type i) const { return !i || is_tail(i - 1); }
            bool is_tail(size_type i) const { return m_masks[i >> MASK_WIDTH] >> (i & (MASK_SIZE - 1)) & mask_type(1); }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i != m_size; i = find_tail(i) + 1) ret.push_back(i);
                return ret;
            }
            std::vector<size_type> tails() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                size_type i = find_tail(0);
                do ret.push_back(i);
                while ((i = find_tail(i + 1)) != m_size);
                return ret;
            }
        };
        template <typename Ostream, bool MaintainGroupSize>
        Ostream &operator<<(Ostream &out, const Table<MaintainGroupSize> &x) {
            out << '[';
            for (size_type l = 0, r; l != x.size(); l = r + 1) {
                r = x.find_tail(l);
                if (l) out << ", ";
                out << "[" << l << ", " << r << "]";
            }
            return out << ']';
        }
    }
}
#endif