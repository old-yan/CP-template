/*
最后修改:
20240420
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
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RollbackCAT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType, typename Compare = std::less<ValueType>>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return Compare()(x, y) ? y : x; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename Node, size_t MAX_LEVEL = 32>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            std::vector<node> m_sub[MAX_LEVEL];
            size_type m_size, m_depth;
            void _deepen(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                node *sub = m_sub[0].data();
                sub[m_size].set(val);
                if (m_depth) {
                    for (size_type i = 1, j = 2; i != m_depth; i++, j <<= 1) {
                        if (m_size & j) {
                            m_sub[i].resize(m_size + 1);
                            node *cur = m_sub[i].data();
                            if (m_size & (j - 1))
                                cur[m_size].set(node::op(cur[m_size - 1].get(), val));
                            else {
                                cur[m_size].set(val), cur[m_size - 1].set(sub[m_size - 1].get());
                                size_type k = (m_size - 1) & -(size_type(1) << i);
                                for (size_type idx = m_size - 1; idx != k; idx--) cur[idx - 1].set(node::op(sub[idx - 1].get(), cur[idx].get()));
                            }
                        }
                    }
                    m_sub[m_depth].resize(m_size + 1);
                    node *cur = m_sub[m_depth].data();
                    cur[m_size].set(val);
                    size_type i = m_size;
                    cur[i - 1].set(sub[i - 1].get());
                    while (--i) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                }
                m_size++, m_depth++;
            }
            void _extend(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                node *sub = m_sub[0].data();
                sub[m_size].set(val);
                if (m_size)
                    for (size_type i = 1, j = 2; i != m_depth; i++, j <<= 1)
                        if (m_size & j) {
                            m_sub[i].resize(m_size + 1);
                            node *cur = m_sub[i].data();
                            if (m_size & (j - 1))
                                cur[m_size].set(node::op(cur[m_size - 1].get(), val));
                            else {
                                cur[m_size].set(val), cur[m_size - 1].set(sub[m_size - 1].get());
                                size_type k = (m_size - 1) & -(size_type(1) << i);
                                for (size_type idx = m_size - 1; idx != k; idx--) cur[idx - 1].set(node::op(sub[idx - 1].get(), cur[idx].get()));
                            }
                        }
                m_size++;
            }
            void _update(size_type i) {
                if (m_depth) {
                    node *sub = m_sub[0].data();
                    for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1)
                        if (i < m_sub[j].size()) {
                            node *cur = m_sub[j].data();
                            size_type l = i & -(1 << (j + 1));
                            if (i >> j & 1) {
                                size_type j = i, end = std::min(l + k, m_size);
                                cur[j].set(j == l + (k >> 1) ? sub[j].get() : node::op(cur[j - 1].get(), sub[j].get()));
                                while (++j != end) cur[j].set(node::op(cur[j - 1].get(), sub[j].get()));
                            } else {
                                size_type j = i + 1;
                                cur[j - 1].set(j == l + (k >> 1) ? sub[j - 1].get() : node::op(sub[j - 1].get(), cur[j].get()));
                                while (--j != l) cur[j - 1].set(node::op(sub[j - 1].get(), cur[j].get()));
                            }
                        }
                }
            }
            Table() : m_size(0), m_depth(0) {}
            template <typename InitMapping = Ignore>
            Table(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return m_depth = 0, m_sub[0].clear();
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub[0].resize(m_size);
                node *sub = m_sub[0].data();
                if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value)
                    for (size_type i = 0; i != m_size; i++) sub[i].set(mapping(i));
                else
                    for (size_type i = 0; i != m_size; i++) sub[i].set(value_type{});
                for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                    m_sub[j].resize(m_size);
                    node *cur = m_sub[j].data();
                    for (l = 0; l + k <= m_size; l += k) {
                        size_type i = l + (k >> 1);
                        cur[i - 1].set(sub[i - 1].get());
                        while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                        i = l + (k >> 1);
                        cur[i].set(sub[i].get());
                        while (++i != l + k) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
                    }
                    if (l != m_size)
                        if (l + (k >> 1) >= m_size) {
                            size_type i = m_size;
                            cur[i - 1].set(sub[i - 1].get());
                            while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                        } else {
                            size_type i = l + (k >> 1);
                            cur[i - 1].set(sub[i - 1].get());
                            while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                            i = l + (k >> 1);
                            cur[i].set(sub[i].get());
                            while (++i != m_size) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
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
            void add(size_type i, const value_type &inc) { m_sub[0][i].set(node::op(inc, m_sub[0][i].get())), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[0][i].set(val), _update(i); }
            void push_back(const value_type &val) {
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
            value_type query(size_type i) const { return m_sub[0][i].get(); }
            value_type query(size_type left, size_type right) const {
                if (left == right) return query(left);
                size_type d = std::bit_width(left ^ right) - 1;
                return node::op(m_sub[d][left].get(), m_sub[d][right].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left].get();
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ ((1 << m_depth) - 1));
                while (d && left < m_size) {
                    value_type a = node::op(val, m_sub[d - 1][left].get());
                    if (judge(a))
                        val = a, --d, left = (left & -(1 << d)) | (1 << d);
                    else
                        while (--d && (left >> (d - 1) & 1)) {}
                }
                if (left < m_size && judge(node::op(val, m_sub[0][left].get()))) left++;
                return std::min(left, m_size) - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right].get();
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = node::op(m_sub[d - 1][right].get(), val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(node::op(m_sub[0][right].get(), val))) right--;
                return right + 1;
            }
        };
        template <typename Ostream, typename Node, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<Node, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL, typename Operation, typename InitMapping = RollbackCAT::Ignore, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(RollbackCAT::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, size_t MAX_LEVEL, typename InitMapping = RollbackCAT::Ignore, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_CatTree(RollbackCAT::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, size_t MAX_LEVEL, typename InitMapping = RollbackCAT::Ignore, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_CatTree(RollbackCAT::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <size_t MAX_LEVEL, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <size_t MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <size_t MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 32>
    using RollbackCatMaxTable = RollbackCAT::Table<RollbackCAT::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 32>
    using RollbackCatMinTable = RollbackCAT::Table<RollbackCAT::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 32>
    using RollbackCatSumTable = RollbackCAT::Table<RollbackCAT::CustomNode<Tp, std::plus<Tp>>, MAX_LEVEL>;
}

#endif