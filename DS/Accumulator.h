/*
最后修改:
20240318
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
#include <vector>

namespace OY {
    namespace ACC {
        using size_type = uint32_t;
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
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            size_type m_size;
            std::vector<node> m_data, m_prefix, m_suffix;
            void _update(size_type i) {
                if constexpr (Prefix) {
                    m_prefix[i].set(i ? node::op(m_prefix[i - 1].get(), m_data[i].get()) : m_data[i].get());
                    for (size_type j = i + 1; j != m_size; j++) m_prefix[j].set(node::op(m_prefix[j - 1].get(), m_data[j].get()));
                }
                if constexpr (Suffix) {
                    m_suffix[i].set(i + 1 < m_size ? node::op(m_data[i].get(), m_suffix[i + 1].get()) : m_data[i].get());
                    for (size_type j = i - 1; ~j; j--) m_suffix[j].set(node::op(m_data[j].get(), m_suffix[j + 1].get()));
                }
            }
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_data.resize(m_size);
                for (size_type i = 0; i != m_size; i++) m_data[i].set(mapping(i));
                if constexpr (Prefix) {
                    m_prefix.resize(m_size);
                    m_prefix[0].set(m_data[0].get());
                    for (size_type i = 1; i != m_size; i++) m_prefix[i].set(node::op(m_prefix[i - 1].get(), m_data[i].get()));
                }
                if constexpr (Suffix) {
                    m_suffix.resize(m_size);
                    m_suffix[m_size - 1].set(m_data[m_size - 1].get());
                    for (size_type i = m_size - 2; ~i; i--) m_suffix[i].set(node::op(m_data[i].get(), m_suffix[i + 1].get()));
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) { m_data[i].set(val), _update(i); }
            void add(size_type i, const value_type &modify) { m_data[i].set(node::op(modify, m_data[i].get())), _update(i); }
            value_type prefix(size_type i) const {
                static_assert(Prefix, "Prefix Tag Must Be True");
                return m_prefix[i].get();
            }
            value_type suffix(size_type i) const {
                static_assert(Suffix, "Prefix Tag Must Be True");
                return m_suffix[i].get();
            }
            value_type query(size_type i) const { return m_data[i].get(); }
            value_type query(size_type left, size_type right) const {
                value_type res = m_data[left].get();
                for (size_type i = left + 1; i <= right; i++) res = node::op(res, m_data[i].get());
                return res;
            }
            value_type query_all() const {
                if constexpr (Prefix)
                    return m_prefix[m_size - 1].get();
                else if constexpr (Suffix)
                    return m_suffix[0].get();
                else {
                    value_type res = m_data[0].get();
                    for (size_type i = 1; i != m_size; i++) res = node::op(res, m_data[i].get());
                    return res;
                }
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_data[left].get();
                if (!judge(val)) return left - 1;
                while (++left != m_size) {
                    value_type a = node::op(val, m_data[left].get());
                    if (!judge(a)) break;
                    val = a;
                }
                return left - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_data[right].get();
                if (!judge(val)) return right + 1;
                while (~--right) {
                    value_type a = node::op(m_data[right].get(), val);
                    if (!judge(a)) break;
                    val = a;
                }
                return right + 1;
            }
        };
        template <typename Ostream, typename Node, bool Prefix, bool Suffix>
        Ostream &operator<<(Ostream &out, const Table<Node, Prefix, Suffix> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, bool Prefix = true, bool Suffix = true, typename Operation, typename InitMapping, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Operation>, Prefix, Suffix>>
    auto make_Accumulator(ACC::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, typename InitMapping, typename TreeType = ACC::Table<ACC::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix>>
    auto make_Accumulator(ACC::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Prefix = true, bool Suffix = true, typename InitMapping, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix>>
    auto make_Accumulator(ACC::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <bool Prefix = true, bool Suffix = true, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Operation>, Prefix, Suffix>>
    auto make_Accumulator(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Prefix, Suffix>>
    auto make_Accumulator(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <bool Prefix = true, bool Suffix = true, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::CustomNode<Tp, Tp (*)(Tp, Tp)>, Prefix, Suffix>>
    auto make_Accumulator(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Node>
    using PrefixTable = ACC::Table<Node, true, true>;
}

#endif
