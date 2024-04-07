/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STTABLE__
#define __OY_STTABLE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace ST {
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
        template <typename Node, size_type MAX_NODE = 1 << 22>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_size;
            template <typename InitMapping = Ignore>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                size_type depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub = s_buffer + s_use_count, s_use_count += m_size * depth;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_size; i++) m_sub[i].set(mapping(i));
                    for (size_type j = 1; j < depth; j++)
                        for (size_type k = m_size * j, l = k - m_size, r = l + (1 << (j - 1)), end = m_size * (j + 1) - (1 << j) + 1; k != end;) m_sub[k++].set(node::op(m_sub[l++].get(), m_sub[r++].get()));
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) {
                m_sub[i].set(node::op(m_sub[i].get(), inc));
                size_type depth = std::bit_width(m_size - 1);
                for (size_type j = 1; j < depth; j++)
                    for (size_type k = i < 1 << j ? m_size * j : m_size * j + i - (1 << j) + 1, l = k - m_size, r = l + (1 << (j - 1)), end = std::min(m_size * j + i + 1, m_size * (j + 1) - (1 << j) + 1); k != end;) m_sub[k++].set(node::op(m_sub[l++].get(), m_sub[r++].get()));
            }
            void modify(size_type i, const value_type &val) {
                m_sub[i].set(val);
                size_type depth = std::bit_width(m_size - 1);
                for (size_type j = 1; j < depth; j++)
                    for (size_type k = i < 1 << j ? m_size * j : m_size * j + i - (1 << j) + 1, l = k - m_size, r = l + (1 << (j - 1)), end = std::min(m_size * j + i + 1, m_size * (j + 1) - (1 << j) + 1); k != end;) m_sub[k++].set(node::op(m_sub[l++].get(), m_sub[r++].get()));
            }
            value_type query(size_type i) const { return m_sub[i].get(); }
            value_type query(size_type left, size_type right) const {
                size_type d = std::bit_width((right - left) >> 1);
                return node::op(m_sub[m_size * d + left].get(), m_sub[m_size * d + right - (1 << d) + 1].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[left].get();
                if (!judge(val)) return left - 1;
                size_type d = std::bit_width(m_size - left - 1);
                if (d) {
                    value_type a = node::op(val, m_sub[m_size * (d - 1) + left + 1].get());
                    if (judge(a))
                        val = a, d = std::bit_width(m_size - left - 1 - (1 << (d - 1))), left = m_size - (1 << d);
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = node::op(val, m_sub[m_size * (d - 1) + left + 1].get());
                    if (judge(a)) val = a, left += 1 << (d - 1);
                }
                return left;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[right].get();
                if (!judge(val)) return right + 1;
                size_type d = std::bit_width(right);
                if (d) {
                    value_type a = node::op(m_sub[m_size * (d - 1) + right - (1 << (d - 1))].get(), val);
                    if (judge(a))
                        val = a, d = std::bit_width(right - (1 << (d - 1))), right = (1 << d) - 1;
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = node::op(m_sub[m_size * (d - 1) + right - (1 << (d - 1))].get(), val);
                    if (judge(a)) val = a, right -= 1 << (d - 1);
                }
                return right;
            }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, size_type MAX_NODE>
        typename Table<Node, MAX_NODE>::node Table<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Table<Node, MAX_NODE>::s_use_count;
    }
    template <typename Tp, ST::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_STTable(ST::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, ST::size_type MAX_NODE = 1 << 22, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_STTable(ST::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, ST::size_type MAX_NODE = 1 << 22, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_STTable(ST::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <ST::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_STTable(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <ST::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_STTable(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <ST::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_STTable(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, ST::size_type MAX_NODE = 1 << 22>
    using STMaxTable = ST::Table<ST::BaseNode<Tp, std::less<Tp>>, MAX_NODE>;
    template <typename Tp, ST::size_type MAX_NODE = 1 << 22>
    using STMinTable = ST::Table<ST::BaseNode<Tp, std::greater<Tp>>, MAX_NODE>;
}

#endif