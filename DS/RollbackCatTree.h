/*
最后修改:
20240905
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKCATTREE__
#define __OY_ROLLBACKCATTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace RollbackCAT {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename SemiGroup, size_t MAX_LEVEL = 30>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub[MAX_LEVEL];
            size_type m_size, m_depth;
            void _deepen(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                auto sub = m_sub[0].data();
                sub[m_size] = val;
                if (m_depth) {
                    for (size_type i = 1, j = 2; i != m_depth; i++, j <<= 1) {
                        if (m_size & j) {
                            m_sub[i].resize(m_size + 1);
                            auto cur = m_sub[i].data();
                            if (m_size & (j - 1))
                                cur[m_size] = group::op(cur[m_size - 1], val);
                            else {
                                cur[m_size] = val, cur[m_size - 1] = sub[m_size - 1];
                                size_type k = (m_size - 1) & -(size_type(1) << i);
                                for (size_type idx = m_size - 1; idx != k; idx--) cur[idx - 1] = group::op(sub[idx - 1], cur[idx]);
                            }
                        }
                    }
                    m_sub[m_depth].resize(m_size + 1);
                    auto cur = m_sub[m_depth].data();
                    cur[m_size] = val;
                    size_type i = m_size;
                    cur[i - 1] = sub[i - 1];
                    while (--i) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                }
                m_size++, m_depth++;
            }
            void _extend(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                auto sub = m_sub[0].data();
                sub[m_size] = val;
                if (m_size)
                    for (size_type i = 1, j = 2; i != m_depth; i++, j <<= 1)
                        if (m_size & j) {
                            m_sub[i].resize(m_size + 1);
                            auto cur = m_sub[i].data();
                            if (m_size & (j - 1))
                                cur[m_size] = group::op(cur[m_size - 1], val);
                            else {
                                cur[m_size] = val, cur[m_size - 1] = sub[m_size - 1];
                                size_type k = (m_size - 1) & -(size_type(1) << i);
                                for (size_type idx = m_size - 1; idx != k; idx--) cur[idx - 1] = group::op(sub[idx - 1], cur[idx]);
                            }
                        }
                m_size++;
            }
            void _update(size_type i) {
                if (m_depth) {
                    auto sub = m_sub[0].data();
                    for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1)
                        if (i < m_sub[j].size()) {
                            auto cur = m_sub[j].data();
                            size_type l = i & -(1 << (j + 1));
                            if (i >> j & 1) {
                                size_type j = i, end = std::min(l + k, m_size);
                                cur[j] = (j == l + (k >> 1)) ? sub[j] : group::op(cur[j - 1], sub[j]);
                                while (++j != end) cur[j] = group::op(cur[j - 1], sub[j]);
                            } else {
                                size_type j = i + 1;
                                cur[j - 1] = (j == l + (k >> 1)) ? sub[j - 1] : group::op(sub[j - 1], cur[j]);
                                while (--j != l) cur[j - 1] = group::op(sub[j - 1], cur[j]);
                            }
                        }
                }
            }
        public:
            Table() : m_size(0), m_depth(0) {}
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return m_depth = 0, m_sub[0].clear();
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub[0].resize(m_size);
                auto sub = m_sub[0].data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                    m_sub[j].resize(m_size);
                    auto cur = m_sub[j].data();
                    for (l = 0; l + k <= m_size; l += k) {
                        size_type i = l + (k >> 1);
                        cur[i - 1] = sub[i - 1];
                        while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        i = l + (k >> 1);
                        cur[i] = sub[i];
                        while (++i != l + k) cur[i] = group::op(cur[i - 1], sub[i]);
                    }
                    if (l != m_size)
                        if (l + (k >> 1) >= m_size) {
                            size_type i = m_size;
                            cur[i - 1] = sub[i - 1];
                            while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        } else {
                            size_type i = l + (k >> 1);
                            cur[i - 1] = sub[i - 1];
                            while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                            i = l + (k >> 1);
                            cur[i] = sub[i];
                            while (++i != m_size) cur[i] = group::op(cur[i - 1], sub[i]);
                        }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type capacity) {
                size_type depth = capacity <= 1 ? 1 : std::bit_width(capacity - 1);
                m_sub[0].reserve(capacity);
                for (size_type i = 1; i != depth; i++) m_sub[i].reserve(((capacity - 1) >> i & 1) ? capacity : ((capacity - 1) & -(1 << i)));
            }
            size_type size() const { return m_size; }
            bool empty() const { return !m_size; }
            void modify(size_type i, value_type val) { m_sub[0][i] = val, _update(i); }
            void push_back(value_type val) {
                if (std::popcount(m_size) == 1)
                    _deepen(val);
                else
                    _extend(val);
            }
            void pop_back() {
                m_sub[0].pop_back();
                if (m_depth)
                    for (size_type i = 1, j = 2; i != m_depth; i++, j <<= 1)
                        if ((m_size - 1) & j)
                            if ((m_size - 1) & (j - 1))
                                m_sub[i].pop_back();
                            else
                                m_sub[i].resize(m_size - j - 1);
                if (std::popcount(--m_size) == 1) m_depth--;
            }
            value_type query(size_type i) const { return m_sub[0][i]; }
            value_type query(size_type left, size_type right) const {
                if (left == right) return query(left);
                size_type d = std::bit_width(left ^ right) - 1;
                return group::op(m_sub[d][left], m_sub[d][right]);
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left];
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ (m_size - 1));
                while (d && left < m_size) {
                    size_type split = (left & -(1 << (d - 1))) | (1 << (d - 1));
                    if (m_size <= split)
                        while (--d && (left >> (d - 1) & 1)) {}
                    else {
                        value_type a = group::op(val, m_sub[d - 1][left]);
                        if (judge(a))
                            val = a, --d, left = split;
                        else
                            while (--d && (left >> (d - 1) & 1)) {}
                    }
                }
                if (left < m_size && judge(group::op(val, m_sub[0][left]))) left++;
                return std::min(left, m_size) - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right];
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = group::op(m_sub[d - 1][right], val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(group::op(m_sub[0][right], val))) right--;
                return right + 1;
            }
        };
        template <typename Ostream, typename SemiGroup, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_RollbackCatTree(RollbackCAT::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_RollbackCatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatMaxTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, RollbackCAT::ChoiceByCompare<Tp, std::less<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatMinTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, RollbackCAT::ChoiceByCompare<Tp, std::greater<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatGcdTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, RollbackCAT::FpTransfer<Tp, std::gcd<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatLcmTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, RollbackCAT::FpTransfer<Tp, std::lcm<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatBitAndTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, std::bit_and<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatBitOrTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, std::bit_or<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatBitXorTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, std::bit_xor<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackCatSumTable = RollbackCAT::Table<RollbackCAT::BaseSemiGroup<Tp, std::plus<Tp>>, MAX_LEVEL>;
}

#endif