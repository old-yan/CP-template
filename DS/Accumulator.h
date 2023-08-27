/*
最后修改:
20230827
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

#include "TEST/mystd.h"

namespace OY {
    namespace Accumulator {
        using size_type = uint32_t;
        struct NoInit {};
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
        template <typename Node, bool Prefix, bool Suffix, size_type MAX_NODE = 1 << 22>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_raw, *m_prefix, *m_suffix;
            size_type m_size;
            template <typename InitMapping = NoInit>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_raw = s_buffer + s_use_count, s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, NoInit>::value)
                    for (size_type i = 0; i < m_size; i++) m_raw[i].set(mapping(i));
                if constexpr (Prefix) {
                    m_prefix = s_buffer + s_use_count, s_use_count += m_size;
                    if constexpr (!std::is_same<InitMapping, NoInit>::value)
                        for (size_type i = 0; i < m_size; i++) m_prefix[i].set(i ? node::op(m_prefix[i - 1].get(), m_raw[i].get()) : m_raw[i].get());
                }
                if constexpr (Suffix) {
                    m_suffix = s_buffer + s_use_count, s_use_count += m_size;
                    if constexpr (!std::is_same<InitMapping, NoInit>::value)
                        for (size_type i = m_size - 1; ~i; i--) m_suffix[i].set(i + 1 < m_size ? node::op(m_raw[i].get(), m_suffix[i + 1].get()) : m_raw[i].get());
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                m_raw[i].set(val);
                if constexpr (Prefix)
                    for (size_type j = i; j < m_size; j++) m_prefix[j].set(j ? node::op(m_prefix[j - 1].get(), m_raw[i].get()) : m_raw[i].get());
                if constexpr (Suffix)
                    for (size_type j = i; ~j; j--) m_suffix[j].set(j + 1 < m_size ? node::op(m_raw[i].get(), m_suffix[j + 1].get()) : m_raw[i].get());
            }
            void add(size_type i, const value_type &modify) {
                m_raw[i].set(node::op(m_raw[i].get(), modify));
                if constexpr (Prefix)
                    for (size_type j = i; j < m_size; j++) m_prefix[j].set(node::op(m_prefix[j].get(), modify));
                if constexpr (Suffix)
                    for (size_type j = i; ~j; j--) m_suffix[j].set(node::op(m_suffix[j].get(), modify));
            }
            value_type prefix(size_type i) const {
                static_assert(Prefix, "Prefix Tag Must Be True");
                return m_prefix[i].get();
            }
            value_type suffix(size_type i) const {
                static_assert(Suffix, "Prefix Tag Must Be True");
                return m_suffix[i].get();
            }
            value_type query(size_type i) const { return m_raw[i].get(); }
            value_type query(size_type left, size_type right) const {
                value_type res = m_raw[left].get();
                for (size_type i = left + 1; i <= right; i++) res = node::op(res, m_raw[i].get());
                return res;
            }
            value_type query_all() const {
                if constexpr (Prefix)
                    return m_prefix[m_size - 1].get();
                else if constexpr (Suffix)
                    return m_suffix[0].get();
                else {
                    value_type res = m_raw[0].get();
                    for (size_type i = 1; i < m_size; i++) res = node::op(res, m_raw[i].get());
                    return res;
                }
            }
            template <typename Judge>
            size_type max_right(size_type left, Judge judge) const {
                value_type val = m_raw[left].get();
                if (!judge(val)) return left - 1;
                while (++left < m_size) {
                    value_type a = node::op(val, m_raw[left].get());
                    if (!judge(a)) break;
                    val = a;
                }
                return left - 1;
            }
            template <typename Judge>
            size_type min_left(size_type right, Judge judge) const {
                value_type val = m_raw[right].get();
                if (!judge(val)) return right + 1;
                while (~--right) {
                    value_type a = node::op(m_raw[right].get(), val);
                    if (!judge(a)) break;
                    val = a;
                }
                return right + 1;
            }
        };
        template <typename Ostream, typename Node, bool Prefix, bool Suffix, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, Prefix, Suffix, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
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
    template <typename Tp, bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = Accumulator::NoInit, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, Operation>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Accumulator::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename InitMapping = Accumulator::NoInit, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Accumulator::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename InitMapping = Accumulator::NoInit, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Accumulator::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, Operation>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, Accumulator::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Accumulator::Table<Accumulator::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix, MAX_NODE>>
    auto make_Accumulator(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Node, Accumulator::size_type MAX_NODE>
    using PrefixTable = Accumulator::Table<Node, true, true, MAX_NODE>;
}

#endif
