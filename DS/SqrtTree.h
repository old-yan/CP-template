/*
最后修改:
20230824
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SQRTTREE__
#define __OY_SQRTTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "Accumulator.h"
#include "CatTree.h"
#include "STtable.h"
#include "ZkwTree.h"

namespace OY {
    namespace Sqrt {
        using size_type = uint32_t;
        struct NoInit {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename = void>
        struct Has_prefix : std::false_type {};
        template <typename Tp>
        struct Has_prefix<Tp, void_t<decltype(std::declval<Tp>().prefix(std::declval<size_type>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_suffix : std::false_type {};
        template <typename Tp>
        struct Has_suffix<Tp, void_t<decltype(std::declval<Tp>().suffix(std::declval<size_type>()))>> : std::true_type {};
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
        template <typename Node, template <typename, size_type> typename InnerTable, template <typename, size_type> typename InterTable, size_type MAX_NODE>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            using inner_table_type = InnerTable<Node, MAX_NODE>;
            using inter_table_type = InterTable<Node, MAX_NODE>;
            std::vector<inner_table_type> m_inner_table;
            inter_table_type m_inter_table;
            size_type m_size, m_depth, m_mask;
            value_type _prefix_of(size_type i, size_type j) const {
                if constexpr (Has_prefix<InnerTable<node, MAX_NODE>>::value)
                    return m_inner_table[i].prefix(j);
                else
                    return m_inner_table[i].query(0, j);
            }
            value_type _suffix_of(size_type i, size_type j) const {
                if constexpr (Has_suffix<InnerTable<node, MAX_NODE>>::value)
                    return m_inner_table[i].suffix(j);
                else
                    return m_inner_table[i].query(j, m_inner_table[i].m_size - 1);
            }
            template <typename InitMapping = NoInit>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = m_size <= 2 ? 1 : (std::bit_width(m_size - 1) >> 1);
                m_mask = (1 << m_depth) - 1;
                m_inner_table.reserve((m_size + (1 << m_depth) - 1) / (1 << m_depth));
                if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                    for (size_type i = 0; i < m_size; i += 1 << m_depth) m_inner_table.emplace_back(std::min<size_type>(1 << m_depth, m_size - i), [&](size_type j) { return mapping(i + j); });
                    m_inter_table.resize((m_size + (1 << m_depth) - 1) / (1 << m_depth), [&](size_type i) { return m_inner_table[i].query_all(); });
                } else {
                    for (size_type i = 0; i < m_size; i += 1 << m_depth) m_inner_table.emplace_back(std::min<size_type>(1 << m_depth, m_size - i));
                    m_inter_table.resize((m_size + (1 << m_depth) - 1) / (1 << m_depth));
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) {
                m_inner_table[i >> m_depth].add(i & m_mask, inc);
                m_inter_table.modify(i >> m_depth, m_inner_table[i >> m_depth].query_all());
            }
            void modify(size_type i, const value_type &val) {
                m_inner_table[i >> m_depth].modify(i & m_mask, val);
                m_inter_table.modify(i >> m_depth, m_inner_table[i >> m_depth].query_all());
            }
            value_type query(size_type i) const { return m_inner_table[i >> m_depth].query(i & m_mask); }
            value_type query(size_type left, size_type right) const {
                size_type l = left >> m_depth, r = right >> m_depth;
                if (l == r)
                    return m_inner_table[l].query(left & m_mask, right & m_mask);
                else if (l + 1 == r)
                    return node::op(_suffix_of(l, left & m_mask), _prefix_of(r, right & m_mask));
                else
                    return node::op(node::op(_suffix_of(l, left & m_mask), m_inter_table.query(l + 1, r - 1)), _prefix_of(r, right & m_mask));
            }
            value_type query_all() const { return m_inter_table.query_all(); }
            template <typename Judge>
            size_type max_right(size_type left, Judge judge) const {
                size_type l = left >> m_depth;
                size_type l2 = m_inner_table[l].max_right(left & m_mask, judge);
                if (l + 1 == m_inner_table.size() || l2 != m_inner_table[l].m_size - 1) return (l << m_depth) + l2;
                value_type val = _suffix_of(l, left & m_mask);
                size_type r = m_inter_table.max_right(l + 1, [&](const value_type &x) { return judge(node::op(val, x)); });
                if (r + 1 == m_inner_table.size()) return m_size - 1;
                if (r > l) val = node::op(val, m_inter_table.query(l + 1, r));
                size_type r2 = m_inner_table[r + 1].max_right(0, [&](const value_type &x) { return judge(node::op(val, x)); });
                return ((r + 1) << m_depth) + r2;
            }
            template <typename Judge>
            size_type min_left(size_type right, Judge judge) const {
                size_type r = right >> m_depth;
                size_type r2 = m_inner_table[r].min_left(right & m_mask, judge);
                if (!r || r2) return (r << m_depth) + r2;
                value_type val = _prefix_of(r, right & m_mask);
                size_type l = m_inter_table.min_left(r - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
                if (!l) return 0;
                if (l < r) val = node::op(m_inter_table.query(l, r - 1), val);
                size_type l2 = m_inner_table[l - 1].min_left((1 << m_depth) - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
                return ((l - 1) << m_depth) + l2;
            }
        };
        template <typename Ostream, typename Node, template <typename, size_type> typename InnerTable, template <typename, size_type> typename InterTable, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, InnerTable, InterTable, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename Operation, typename InitMapping = Sqrt::NoInit, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, Operation>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Sqrt::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename InitMapping = Sqrt::NoInit, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Sqrt::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename InitMapping = Sqrt::NoInit, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Sqrt::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, Operation>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Sqrt::Table<Sqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, InnerTable, InterTable, MAX_NODE>>
    auto make_SqrtTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10>
    using SqrtMaxTable = Sqrt::Table<Sqrt::BaseNode<Tp, std::less<Tp>>, InnerTable, InterTable, MAX_NODE>;
    template <typename Tp, template <typename, Sqrt::size_type> typename InnerTable = PrefixTable, template <typename, Sqrt::size_type> typename InterTable = ST::Table, Sqrt::size_type MAX_NODE = 1 << 10>
    using SqrtMinTable = Sqrt::Table<Sqrt::BaseNode<Tp, std::greater<Tp>>, InnerTable, InterTable, MAX_NODE>;
}

#endif