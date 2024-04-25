/*
最后修改:
20240425
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
#include <vector>

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
        template <typename Node, size_type MAX_LEVEL = 32>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            std::vector<node> m_sub[MAX_LEVEL];
            size_type m_size;
            void _update(size_type idx) {
                size_type depth = std::bit_width(m_size - 1);
                for (size_type i = 1; i < depth; i++) {
                    node *pre = m_sub[i - 1].data(), *cur = m_sub[i].data();
                    for (size_type j1 = idx < 1 << i ? 0 : (idx - (1 << i) + 1), j2 = j1 + (1 << (i - 1)), k = std::min(idx + 1, m_size - (1 << i) + 1); j1 != k; j1++, j2++) cur[j1].set(node::op(pre[j1].get(), pre[j2].get()));
                }
            }
            template <typename InitMapping = Ignore>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                size_type depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                for (size_type i = 0; i != depth; i++) m_sub[i].resize(m_size - (1 << i) + 1);
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    node *sub = m_sub[0].data();
                    for (size_type i = 0; i != m_size; i++) sub[i].set(mapping(i));
                    for (size_type i = 1; i != depth; i++) {
                        node *pre = m_sub[i - 1].data(), *cur = m_sub[i].data();
                        for (size_type j1 = 0, j2 = 1 << (i - 1), k = m_size - (1 << i) + 1; j1 != k; j1++, j2++) cur[j1].set(node::op(pre[j1].get(), pre[j2].get()));
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) { m_sub[0][i].set(node::op(inc, m_sub[0][i].get())), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[0][i].set(val), _update(i); }
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
        template <typename Ostream, typename Node, size_type MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<Node, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, ST::size_type MAX_LEVEL = 32, typename Operation, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(ST::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, ST::size_type MAX_LEVEL = 32, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_STTable(ST::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, ST::size_type MAX_LEVEL = 32, typename InitMapping = ST::Ignore, typename TreeType = ST::Table<ST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_STTable(ST::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <ST::size_type MAX_LEVEL = 32, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <ST::size_type MAX_LEVEL = 32, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <ST::size_type MAX_LEVEL = 32, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, ST::size_type MAX_LEVEL = 32>
    using STMaxTable = ST::Table<ST::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, ST::size_type MAX_LEVEL = 32>
    using STMinTable = ST::Table<ST::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
}

#endif