/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CATTREE__
#define __OY_CATTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace Cat {
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
            size_type m_size, m_depth;
            void _update(size_type i) const {
                for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1) {
                    node *cur = m_sub + m_size * j;
                    size_type l = i & -(1 << (j + 1));
                    if (i >> j & 1) {
                        size_type j = i, end = std::min(l + k, m_size);
                        cur[j].set(j == l + (k >> 1) ? m_sub[j].get() : node::op(cur[j - 1].get(), m_sub[j].get()));
                        while (++j != end) cur[j].set(node::op(cur[j - 1].get(), m_sub[j].get()));
                    } else {
                        size_type j = i + 1;
                        cur[j - 1].set(j == l + (k >> 1) ? m_sub[j - 1].get() : node::op(m_sub[j - 1].get(), cur[j].get()));
                        while (--j != l) cur[j - 1].set(node::op(m_sub[j - 1].get(), cur[j].get()));
                    }
                }
            }
            template <typename InitMapping = Ignore>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub = s_buffer + s_use_count, s_use_count += m_size * m_depth;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i != m_size; i++) m_sub[i].set(mapping(i));
                    for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                        node *cur = m_sub + m_size * j;
                        for (l = 0; l + k <= m_size; l += k) {
                            size_type i = l + (k >> 1);
                            cur[i - 1].set(m_sub[i - 1].get());
                            while (--i != l) cur[i - 1].set(node::op(m_sub[i - 1].get(), cur[i].get()));
                            i = l + (k >> 1);
                            cur[i].set(m_sub[i].get());
                            while (++i != l + k) cur[i].set(node::op(cur[i - 1].get(), m_sub[i].get()));
                        }
                        if (l != m_size)
                            if (l + (k >> 1) >= m_size) {
                                size_type i = m_size;
                                cur[i - 1].set(m_sub[i - 1].get());
                                while (--i != l) cur[i - 1].set(node::op(m_sub[i - 1].get(), cur[i].get()));
                            } else {
                                size_type i = l + (k >> 1);
                                cur[i - 1].set(m_sub[i - 1].get());
                                while (--i != l) cur[i - 1].set(node::op(m_sub[i - 1].get(), cur[i].get()));
                                i = l + (k >> 1);
                                cur[i].set(m_sub[i].get());
                                while (++i != m_size) cur[i].set(node::op(cur[i - 1].get(), m_sub[i].get()));
                            }
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) { m_sub[i].set(node::op(m_sub[i].get(), inc)), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[i].set(val), _update(i); }
            value_type query(size_type i) const { return m_sub[i].get(); }
            value_type query(size_type left, size_type right) const {
                if (left == right) return m_sub[left].get();
                size_type d = std::bit_width(left ^ right) - 1;
                return node::op(m_sub[m_size * d + left].get(), m_sub[m_size * d + right].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[left].get();
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ ((1 << m_depth) - 1));
                while (d && left < m_size) {
                    value_type a = node::op(val, m_sub[m_size * (d - 1) + left].get());
                    if (judge(a))
                        val = a, --d, left = (left & -(1 << d)) | (1 << d);
                    else
                        while (--d && (left >> (d - 1) & 1)) {}
                }
                if (left < m_size && judge(node::op(val, m_sub[left].get()))) left++;
                return std::min(left, m_size) - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[right].get();
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = node::op(m_sub[m_size * (d - 1) + right].get(), val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(node::op(m_sub[right].get(), val))) right--;
                return right + 1;
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
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = Cat::Ignore, typename TreeType = Cat::Table<Cat::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_CatTree(Cat::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22, typename InitMapping = Cat::Ignore, typename TreeType = Cat::Table<Cat::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_CatTree(Cat::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22, typename InitMapping = Cat::Ignore, typename TreeType = Cat::Table<Cat::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_CatTree(Cat::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <Cat::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Cat::Table<Cat::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_CatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <Cat::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Cat::Table<Cat::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_CatTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <Cat::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Cat::Table<Cat::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_CatTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22>
    using CatMaxTable = Cat::Table<Cat::BaseNode<Tp, std::less<Tp>>, MAX_NODE>;
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22>
    using CatMinTable = Cat::Table<Cat::BaseNode<Tp, std::greater<Tp>>, MAX_NODE>;
    template <typename Tp, Cat::size_type MAX_NODE = 1 << 22>
    using CatSumTable = Cat::Table<Cat::CustomNode<Tp, std::plus<Tp>>, MAX_NODE>;
}

#endif