/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ACCUMULATOR__
#define __OY_ACCUMULATOR__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace ACC {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            value_type m_val;
            void set(const value_type &val) { m_val = val; }
            const value_type &get() const { return m_val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            void set(const value_type &val) { m_val = val; }
            const value_type &get() const { return m_val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename Node, bool Prefix, bool Suffix>
        struct AccNode {
            Node m_val;
        };
        template <typename Node>
        struct AccNode<Node, true, false> {
            Node m_val, m_prefix;
        };
        template <typename Node>
        struct AccNode<Node, false, true> {
            Node m_val, m_suffix;
        };
        template <typename Node>
        struct AccNode<Node, true, true> {
            Node m_val, m_prefix, m_suffix;
        };
        template <typename Node, bool Prefix, bool Suffix, size_type MAX_NODE = 1 << 22>
        struct Table {
            using node_base = Node;
            using node = AccNode<Node, Prefix, Suffix>;
            using value_type = typename Node::value_type;
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
                m_sub = s_buffer + s_use_count, s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, Ignore>::value)
                    for (size_type i = 0; i != m_size; i++) m_sub[i].m_val.set(mapping(i));
                if constexpr (Prefix && !std::is_same<InitMapping, Ignore>::value)
                    for (size_type i = 0; i != m_size; i++) m_sub[i].m_prefix.set(i ? node_base::op(m_sub[i - 1].m_prefix.get(), m_sub[i].m_val.get()) : m_sub[i].m_val.get());
                if constexpr (Suffix && !std::is_same<InitMapping, Ignore>::value)
                    for (size_type i = m_size - 1; ~i; i--) m_sub[i].m_suffix.set(i + 1 < m_size ? node_base::op(m_sub[i].m_val.get(), m_sub[i + 1].m_suffix.get()) : m_sub[i].m_val.get());
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                m_sub[i].m_val.set(val);
                if constexpr (Prefix)
                    for (size_type j = i; j != m_size; j++) m_sub[j].m_prefix.set(j ? node_base::op(m_sub[j - 1].m_prefix.get(), m_sub[i].m_val.get()) : m_sub[i].m_val.get());
                if constexpr (Suffix)
                    for (size_type j = i; ~j; j--) m_sub[j].m_suffix.set(j + 1 < m_size ? node_base::op(m_sub[i].m_val.get(), m_sub[j + 1].m_suffix.get()) : m_sub[i].m_val.get());
            }
            void add(size_type i, const value_type &modify) {
                m_sub[i].m_val.set(node_base::op(m_sub[i].m_val.get(), modify));
                if constexpr (Prefix)
                    for (size_type j = i; j != m_size; j++) m_sub[j].m_prefix.set(node_base::op(m_sub[j].m_prefix.get(), modify));
                if constexpr (Suffix)
                    for (size_type j = i; ~j; j--) m_sub[j].m_suffix.set(node_base::op(m_sub[j].m_suffix.get(), modify));
            }
            value_type prefix(size_type i) const {
                static_assert(Prefix, "Prefix Tag Must Be True");
                return m_sub[i].m_prefix.get();
            }
            value_type suffix(size_type i) const {
                static_assert(Suffix, "Prefix Tag Must Be True");
                return m_sub[i].m_suffix.get();
            }
            value_type query(size_type i) const { return m_sub[i].m_val.get(); }
            value_type query(size_type left, size_type right) const {
                value_type res = m_sub[left].m_val.get();
                for (size_type i = left + 1; i <= right; i++) res = node_base::op(res, m_sub[i].m_val.get());
                return res;
            }
            value_type query_all() const {
                if constexpr (Prefix)
                    return m_sub[m_size - 1].m_prefix.get();
                else if constexpr (Suffix)
                    return m_sub[0].m_suffix.get();
                else {
                    value_type res = m_sub[0].m_val.get();
                    for (size_type i = 1; i != m_size; i++) res = node_base::op(res, m_sub[i].m_val.get());
                    return res;
                }
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger judge) const {
                value_type val = m_sub[left].m_val.get();
                if (!judge(val)) return left - 1;
                while (++left != m_size) {
                    value_type a = node_base::op(val, m_sub[left].m_val.get());
                    if (!judge(a)) break;
                    val = a;
                }
                return left - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger judge) const {
                value_type val = m_sub[right].m_val.get();
                if (!judge(val)) return right + 1;
                while (~--right) {
                    value_type a = node_base::op(m_sub[right].m_val.get(), val);
                    if (!judge(a)) break;
                    val = a;
                }
                return right + 1;
            }
        };
        template <typename Ostream, typename Node, bool Prefix, bool Suffix, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, Prefix, Suffix, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, bool Prefix, bool Suffix, size_type MAX_NODE>
        typename Table<Node, Prefix, Suffix, MAX_NODE>::node Table<Node, Prefix, Suffix, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, bool Prefix, bool Suffix, size_type MAX_NODE>
        size_type Table<Node, Prefix, Suffix, MAX_NODE>::s_use_count;
    }
    template <typename Tp, bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = ACC::Ignore, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Operation>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(ACC::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename InitMapping = ACC::Ignore, typename TreeType = ACC::Table<ACC::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(ACC::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node_base::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename InitMapping = ACC::Ignore, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(ACC::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node_base::s_op = op, TreeType(length, mapping); }
    template <bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Operation>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node_base::s_op = op, TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, ACC::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node_base::s_op = op, TreeType(first, last); }
    template <typename Node, ACC::size_type MAX_NODE>
    using PrefixTable = ACC::Table<Node, true, true, MAX_NODE>;
}

#endif
