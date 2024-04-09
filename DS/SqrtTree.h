/*
最后修改:
20240313
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

#include "CatTree.h"

namespace OY {
    namespace Sqrt {
        using size_type = uint32_t;
        static constexpr bool random_data = false;
        struct Ignore {};
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
        template <typename Node, size_type MAX_NODE, typename InterTable = Cat::Table<Node, MAX_NODE>>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            InterTable m_inter_table;
            std::vector<node> m_data, m_prefix, m_suffix;
            size_type m_size, m_depth, m_mask;
            template <typename Judger>
            size_type _max_right(size_type left, size_type end, Judger &&judge) const {
                value_type val = m_data[left].get();
                if (judge(val))
                    while (++left != end) {
                        value_type a = node::op(val, m_data[left].get());
                        if (!judge(a)) break;
                        val = a;
                    }
                return left - 1;
            }
            template <typename Judger>
            size_type _max_right2(size_type left, size_type end, Judger &&judge) const {
                size_type low = left, high = end;
                while (low != high) {
                    size_type mid = (low + high) / 2;
                    if (judge(m_prefix[mid].get()))
                        low = mid + 1;
                    else
                        high = mid;
                }
                return low - 1;
            }
            template <typename Judger>
            size_type _min_left(size_type end, size_type right, Judger &&judge) const {
                value_type val = m_data[right].get();
                if (judge(val))
                    while (--right != end) {
                        value_type a = node::op(m_data[right].get(), val);
                        if (!judge(a)) break;
                        val = a;
                    }
                return right + 1;
            }
            template <typename Judger>
            size_type _min_left2(size_type end, size_type right, Judger &&judge) const {
                size_type low = end, high = right;
                while (low != high) {
                    size_type mid = (low + high + 1) / 2;
                    if (judge(m_suffix[mid].get()))
                        high = mid - 1;
                    else
                        low = mid;
                }
                return low + 1;
            }
            template <typename InitMapping = Ignore>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = (std::bit_width(m_size) - 1) / 2;
                if constexpr (!random_data)
                    if (m_size >= 32) m_depth = std::bit_width<size_type>(std::bit_width(m_size / std::bit_width(m_size)) - 1);
                m_mask = (1 << m_depth) - 1;
                m_data.resize(length);
                if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value)
                    for (size_type i = 0; i != length; i++) m_data[i].set(mapping(i));
                m_prefix = m_suffix = m_data;
                for (size_type i = 1; i != length; i++)
                    if (i & m_mask) m_prefix[i].set(node::op(m_prefix[i - 1].get(), m_prefix[i].get()));
                for (size_type i = length - 1; i; i--)
                    if (i & m_mask) m_suffix[i - 1].set(node::op(m_suffix[i - 1].get(), m_suffix[i].get()));
                m_inter_table.resize((m_size + (1 << m_depth) - 1) / (1 << m_depth), [&](size_type i) { return m_suffix[i << m_depth].get(); });
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) {
                m_data[i].set(node::op(inc, m_data[i].get()));
                for (size_type j = i, k = std::min((i | m_mask) + 1, m_size); j != k; j++)
                    m_prefix[j].set((j & m_mask) ? node::op(m_prefix[j - 1].get(), m_data[j].get()) : m_data[j].get());
                m_suffix[i].set((i + 1 == m_size || !(i + 1 & m_mask)) ? m_data[i].get() : node::op(m_data[i].get(), m_suffix[i + 1].get()));
                for (size_type j = i - 1, k = (i | m_mask) - (1 << m_depth); j != k; j--)
                    m_suffix[j].set(node::op(m_suffix[j + 1].get(), m_data[j].get()));
                m_inter_table.modify(i >> m_depth, m_suffix[i & ~m_mask].get());
            }
            void modify(size_type i, const value_type &val) {
                m_data[i].set(val);
                for (size_type j = i, k = std::min((i | m_mask) + 1, m_size); j != k; j++)
                    m_prefix[j].set((j & m_mask) ? node::op(m_prefix[j - 1].get(), m_data[j].get()) : m_data[j].get());
                m_suffix[i].set((i + 1 == m_size || !(i + 1 & m_mask)) ? m_data[i].get() : node::op(m_data[i].get(), m_suffix[i + 1].get()));
                for (size_type j = i - 1, k = (i | m_mask) - (1 << m_depth); j != k; j--)
                    m_suffix[j].set(node::op(m_suffix[j + 1].get(), m_data[j].get()));
                m_inter_table.modify(i >> m_depth, m_suffix[i & ~m_mask].get());
            }
            value_type query(size_type i) const { return m_data[i].get(); }
            value_type query(size_type left, size_type right) const {
                size_type l = left >> m_depth, r = right >> m_depth;
                if (l == r) {
                    value_type res = m_data[left].get();
                    for (size_type i = left + 1; i <= right; i++) res = node::op(res, m_data[i].get());
                    return res;
                } else if (l + 1 == r)
                    return node::op(m_suffix[left].get(), m_prefix[right].get());
                else
                    return node::op(node::op(m_suffix[left].get(), m_inter_table.query(l + 1, r - 1)), m_prefix[right].get());
            }
            value_type query_all() const { return m_inter_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_suffix[left].get();
                if (!judge(val)) return _max_right(left, std::min(m_size, (left | m_mask) + 1), judge);
                size_type l = left >> m_depth;
                if (l + 1 == m_inter_table.m_size) return m_size - 1;
                size_type r = m_inter_table.max_right(l + 1, [&](const value_type &x) { return judge(node::op(val, x)); });
                if (r + 1 == m_inter_table.m_size) return m_size - 1;
                if (r > l) val = node::op(val, m_inter_table.query(l + 1, r));
                return _max_right2((r + 1) << m_depth, std::min(m_size, (r + 2) << m_depth), [&](const value_type &x) { return judge(node::op(val, x)); });
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_prefix[right].get();
                if (!judge(val)) return _min_left((right | m_mask) - (1 << m_depth), right, judge);
                size_type r = right >> m_depth;
                if (!r) return 0;
                size_type l = m_inter_table.min_left(r - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
                if (!l) return 0;
                if (l < r) val = node::op(m_inter_table.query(l, r - 1), val);
                return _min_left2(((l - 1) << m_depth) - 1, (l << m_depth) - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
            }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE, typename InterTable>
        Ostream &operator<<(Ostream &out, const Table<Node, MAX_NODE, InterTable> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Sqrt::size_type MAX_NODE = 1 << 20, typename Operation, typename InitMapping = Sqrt::Ignore, typename Node = Sqrt::CustomNode<Tp, Operation>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Sqrt::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Sqrt::size_type MAX_NODE = 1 << 20, typename InitMapping = Sqrt::Ignore, typename Node = Sqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Sqrt::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, Sqrt::size_type MAX_NODE = 1 << 20, typename InitMapping = Sqrt::Ignore, typename Node = Sqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Sqrt::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <Sqrt::size_type MAX_NODE = 1 << 20, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = Sqrt::CustomNode<Tp, Operation>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <Sqrt::size_type MAX_NODE = 1 << 20, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = Sqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <Sqrt::size_type MAX_NODE = 1 << 20, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = Sqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename TreeType = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>>
    auto make_SqrtTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, Sqrt::size_type MAX_NODE = 1 << 20, typename Node = Sqrt::BaseNode<Tp, std::less<Tp>>>
    using SqrtMaxTable = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>;
    template <typename Tp, Sqrt::size_type MAX_NODE = 1 << 20, typename Node = Sqrt::BaseNode<Tp, std::greater<Tp>>>
    using SqrtMinTable = Sqrt::Table<Node, MAX_NODE, Cat::Table<Node, MAX_NODE>>;
}

#endif