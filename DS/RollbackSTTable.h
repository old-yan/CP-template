/*
最后修改:
20240421
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKSTTABLE__
#define __OY_ROLLBACKSTTABLE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RollbackST {
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
            void _update(size_type i) {
                if (m_depth) {
                    node *pre = m_sub[0].data();
                    for (size_type j = 1, k = 2; j != m_depth; j++, k <<= 1) {
                        node *cur = m_sub[j].data();
                        for (size_type l1 = i < 1 << j ? 0 : i - (1 << j) + 1, l2 = std::min(i + 1, m_size - (1 << j) + 1); l1 != l2; l1++) cur[l1].set(node::op(pre[l1].get(), pre[l1 + (k >> 1)].get()));
                        pre = cur;
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
                m_size = length, m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub[0].resize(m_size);
                node *sub = m_sub[0].data();
                if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value)
                    for (size_type i = 0; i != m_size; i++) sub[i].set(mapping(i));
                else
                    for (size_type i = 0; i != m_size; i++) sub[i].set(value_type{});
                for (size_type j = 1; j != m_depth; j++) {
                    m_sub[j].resize(m_size - (1 << j) + 1);
                    node *cur = m_sub[j].data(), *pre = m_sub[j - 1].data();
                    for (size_type k = 0, l = 1 << (j - 1), end = m_sub[j].size(); k != end; k++, l++) cur[k].set(node::op(pre[k].get(), pre[l].get()));
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type capacity) {
                size_type depth = capacity <= 1 ? 1 : std::bit_width(capacity - 1);
                m_sub[0].reserve(capacity);
                for (size_type i = 1; i != depth; i++) m_sub[i].reserve(capacity - (1 << i) + 1);
            }
            size_type size() const { return m_size; }
            bool empty() const { return !m_size; }
            void add(size_type i, const value_type &inc) { m_sub[0][i].set(node::op(inc, m_sub[0][i].get())), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[0][i].set(val), _update(i); }
            void push_back(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                m_sub[0][m_size].set(val);
                if (!m_size)
                    m_depth++;
                else {
                    node *pre = m_sub[0].data();
                    for (size_type i = 1, j = 1, index = m_size - 1; i != m_depth; i++, j <<= 1, index -= j) {
                        m_sub[i].resize(index + 1);
                        node *cur = m_sub[i].data();
                        cur[index].set(node::op(pre[index].get(), pre[index + j].get())), pre = cur;
                    }
                    if (!(m_size & (m_size + 1))) {
                        m_sub[m_depth].resize(1);
                        m_sub[m_depth][0].set(node::op(pre[0].get(), pre[1 << (m_depth - 1)].get()));
                        m_depth++;
                    }
                }
                m_size++;
            }
            void pop_back() {
                for (size_type i = 0; i != m_depth; i++) m_sub[i].pop_back();
                if (!(m_size & (m_size - 1))) m_depth--;
                m_size--;
            }
            value_type query(size_type i) const { return m_sub[0][i].get(); }
            value_type query(size_type left, size_type right) const {
                size_type d = std::bit_width((right - left) >> 1);
                return node::op(m_sub[d][left].get(), m_sub[d][right - (1 << d) + 1].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left].get();
                if (!judge(val)) return left - 1;
                size_type d = std::bit_width(m_size - left - 1);
                if (d) {
                    value_type a = node::op(val, m_sub[d - 1][left + 1].get());
                    if (judge(a))
                        val = a, d = std::bit_width(m_size - left - 1 - (1 << (d - 1))), left = m_size - (1 << d);
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = node::op(val, m_sub[d - 1][left + 1].get());
                    if (judge(a)) val = a, left += 1 << (d - 1);
                }
                return left;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right].get();
                if (!judge(val)) return right + 1;
                size_type d = std::bit_width(right);
                if (d) {
                    value_type a = node::op(m_sub[d - 1][right - (1 << (d - 1))].get(), val);
                    if (judge(a))
                        val = a, d = std::bit_width(right - (1 << (d - 1))), right = (1 << d) - 1;
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = node::op(m_sub[d - 1][right - (1 << (d - 1))].get(), val);
                    if (judge(a)) val = a, right -= 1 << (d - 1);
                }
                return right;
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
    template <typename Tp, size_t MAX_LEVEL, typename Operation, typename InitMapping = RollbackST::Ignore, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(RollbackST::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, size_t MAX_LEVEL, typename InitMapping = RollbackST::Ignore, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_STTable(RollbackST::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, size_t MAX_LEVEL, typename InitMapping = RollbackST::Ignore, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_STTable(RollbackST::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <size_t MAX_LEVEL, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <size_t MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <size_t MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackST::Table<RollbackST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 32>
    using RollbackSTMaxTable = RollbackST::Table<RollbackST::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 32>
    using RollbackSTMinTable = RollbackST::Table<RollbackST::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
}

#endif