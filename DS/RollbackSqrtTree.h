/*
最后修改:
20240421
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKSQRTTREE__
#define __OY_ROLLBACKSQRTTREE__

#include <algorithm>
#include <cstdint>
#include <functional>

#include "RollbackCatTree.h"

namespace OY {
    namespace RollbackSqrt {
        using size_type = uint32_t;
        using RollbackCAT::BaseNode;
        using RollbackCAT::CustomNode;
        using RollbackCAT::Ignore;
        template <size_type BlockSize = 16>
        struct StaticController {
            void reserve(size_type capacity) {}
            static constexpr bool is_first(size_type i) { return i % BlockSize == 0; }
            static constexpr size_type block_id(size_type i) { return i / BlockSize; }
            static constexpr size_type block_first(size_type i) { return i / BlockSize * BlockSize; }
            static constexpr size_type block_size() { return BlockSize; }
            static constexpr size_type block_count(size_type length) { return length ? (length - 1) / BlockSize : 0; }
        };
        template <size_type DefaultDepth = 5>
        struct RandomController {
            size_type m_mask = (1 << DefaultDepth) - 1, m_depth = DefaultDepth;
            void reserve(size_type capacity) { m_depth = (std::bit_width(capacity) - 1) / 2, m_mask = (size_type(1) << m_depth) - 1; }
            bool is_first(size_type i) const { return !(i & m_mask); }
            size_type block_id(size_type i) const { return i >> m_depth; }
            size_type block_first(size_type i) const { return i & ~m_mask; }
            size_type block_size() const { return m_mask + 1; }
            size_type block_count(size_type length) const { return length ? (length - 1) >> m_depth : 0; }
        };
        template <size_type DefaultDepth = 5>
        struct NonRandomController {
            size_type m_mask = (1 << DefaultDepth) - 1, m_depth = DefaultDepth;
            void reserve(size_type capacity) { m_depth = capacity >= 32 ? std::bit_width<size_type>(std::bit_width(capacity / std::bit_width(capacity)) - 1) : (std::bit_width(capacity) - 1) / 2, m_mask = (size_type(1) << m_depth) - 1; }
            bool is_first(size_type i) const { return !(i & m_mask); }
            size_type block_id(size_type i) const { return i >> m_depth; }
            size_type block_first(size_type i) const { return i & ~m_mask; }
            size_type block_size() const { return m_mask + 1; }
            size_type block_count(size_type length) const { return length ? (length - 1) >> m_depth : 0; }
        };
        template <typename Node, typename Controller = StaticController<16>, size_t MAX_LEVEL = 32>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            using inner_table = RollbackCAT::Table<node, MAX_LEVEL>;
            inner_table m_table;
            std::vector<node> m_data, m_prefix, m_suffix;
            size_type m_size{};
            Controller m_ctrl;
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
            void _update(size_type i) {
                size_type cur = m_ctrl.block_first(i), nxt = std::min(cur + m_ctrl.block_size(), m_size);
                m_prefix[i].set(i == cur ? m_data[i].get() : node::op(m_prefix[i - 1].get(), m_data[i].get()));
                for (size_type j = i + 1; j != nxt; j++) m_prefix[j].set(node::op(m_prefix[j - 1].get(), m_data[j].get()));
                if (nxt != m_size) {
                    m_suffix[i].set(i == nxt - 1 ? m_data[i].get() : node::op(m_data[i].get(), m_suffix[i + 1].get()));
                    for (size_type j = i - 1; j != cur - 1; j--) m_suffix[j].set(node::op(m_data[j].get(), m_suffix[j + 1].get()));
                    m_table.modify(m_ctrl.block_id(i), m_suffix[cur].get());
                }
            }
            Table() = default;
            template <typename InitMapping = Ignore>
            Table(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                m_ctrl.reserve(m_size = length);
                m_data.resize(length);
                if constexpr (!std::is_same<InitMapping, Ignore>::value)
                    for (size_type i = 0; i != m_size; i++) m_data[i].set(mapping(i));
                else
                    for (size_type i = 0; i != m_size; i++) m_data[i].set(value_type{});
                m_prefix = m_suffix = m_data;
                for (size_type i = 1; i != m_size; i++)
                    if (!m_ctrl.is_first(i)) m_prefix[i].set(node::op(m_prefix[i - 1].get(), m_prefix[i].get()));
                m_suffix.resize(m_ctrl.block_count(m_size) * m_ctrl.block_size());
                if (!m_suffix.empty())
                    for (size_type i = m_suffix.size(); i; i--)
                        if (!m_ctrl.is_first(i)) m_suffix[i - 1].set(node::op(m_suffix[i - 1].get(), m_suffix[i].get()));
                m_table.resize(m_ctrl.block_count(m_size), [&](size_type i) { return m_suffix[i * m_ctrl.block_size()].get(); });
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type capacity) {
                if (capacity) m_ctrl.reserve(capacity), m_data.reserve(capacity), m_prefix.reserve(capacity), m_suffix.reserve(capacity), m_table.reserve(m_ctrl.block_count(capacity));
            }
            size_type size() const { return m_size; }
            bool empty() const { return !m_size; }
            void add(size_type i, const value_type &inc) { m_data[i].set(node::op(inc, m_data[i].get())), _update(i); }
            void modify(size_type i, const value_type &val) { m_data[i].set(val), _update(i); }
            void push_back(const value_type &val) {
                m_data.resize(m_size + 1), m_prefix.resize(m_size + 1);
                m_data[m_size].set(val);
                size_type cur = m_ctrl.block_first(m_size);
                m_prefix[m_size].set(m_size == cur ? val : node::op(m_prefix[m_size - 1].get(), val));
                if (m_size && m_size == cur) {
                    m_suffix.resize(m_size);
                    m_suffix[m_size - 1].set(m_data[m_size - 1].get());
                    for (size_type i = m_size - 1, j = m_size - m_ctrl.block_size(); i != j; i--) m_suffix[i - 1].set(node::op(m_data[i - 1].get(), m_suffix[i].get()));
                    m_table.push_back(m_suffix[m_size - m_ctrl.block_size()].get());
                }
                m_size++;
            }
            void pop_back() {
                m_data.pop_back(), m_prefix.pop_back();
                size_type cur = m_ctrl.block_first(--m_size);
                if (m_size && m_size == cur) {
                    m_suffix.resize(m_size - m_ctrl.block_size());
                    m_table.pop_back();
                }
            }
            value_type query(size_type i) const { return m_data[i].get(); }
            value_type query(size_type left, size_type right) const {
                size_type l = m_ctrl.block_id(left), r = m_ctrl.block_id(right);
                if (l == r) {
                    value_type res = m_data[left].get();
#ifndef __clang__
#pragma GCC unroll 64
#endif
                    for (size_type i = left + 1; i <= right; i++) res = node::op(res, m_data[i].get());
                    return res;
                } else if (l + 1 == r)
                    return node::op(m_suffix[left].get(), m_prefix[right].get());
                else
                    return node::op(node::op(m_suffix[left].get(), m_table.query(l + 1, r - 1)), m_prefix[right].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                if (left < m_suffix.size()) {
                    value_type val = m_suffix[left].get();
                    if (!judge(val)) return _max_right(left, std::min(m_size, m_ctrl.block_first(left) + m_ctrl.block_size()), judge);
                    size_type l = m_ctrl.block_id(left);
                    if (l + 1 < m_table.size()) {
                        size_type r = m_table.max_right(l + 1, [&](const value_type &x) { return judge(node::op(val, x)); });
                        if (r > l) val = node::op(val, m_table.query(l + 1, r));
                        l = r;
                    }
                    return _max_right2((l + 1) * m_ctrl.block_size(), std::min(m_size, (l + 2) * m_ctrl.block_size()), [&](const value_type &x) { return judge(node::op(val, x)); });
                } else
                    return _max_right(left, m_size, judge);
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_prefix[right].get();
                if (!judge(val)) return _min_left(m_ctrl.block_first(right) - 1, right, judge);
                size_type r = m_ctrl.block_id(right);
                if (!r) return 0;
                size_type l = m_table.min_left(r - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
                if (!l) return 0;
                if (l < r) val = node::op(m_table.query(l, r - 1), val);
                return _min_left2((l - 1) * m_ctrl.block_size() - 1, l * m_ctrl.block_size() - 1, [&](const value_type &x) { return judge(node::op(x, val)); });
            }
        };
        template <typename Ostream, typename Node, typename Controller, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<Node, Controller, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename Operation, typename InitMapping = RollbackSqrt::Ignore, typename Node = RollbackSqrt::CustomNode<Tp, Operation>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(RollbackSqrt::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename InitMapping = RollbackSqrt::Ignore, typename Node = RollbackSqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(RollbackSqrt::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename InitMapping = RollbackSqrt::Ignore, typename Node = RollbackSqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(RollbackSqrt::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = RollbackSqrt::CustomNode<Tp, Operation>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = RollbackSqrt::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Node = RollbackSqrt::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename TreeType = RollbackSqrt::Table<Node, Controller, MAX_LEVEL>>
    auto make_RollbackSqrtTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32>
    using RollbackSqrtMaxTable = RollbackSqrt::Table<RollbackSqrt::BaseNode<Tp, std::less<Tp>>, Controller, MAX_LEVEL>;
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32>
    using RollbackSqrtMinTable = RollbackSqrt::Table<RollbackSqrt::BaseNode<Tp, std::greater<Tp>>, Controller, MAX_LEVEL>;
    template <typename Tp, typename Controller = RollbackSqrt::RandomController<>, RollbackSqrt::size_type MAX_LEVEL = 32>
    using RollbackSqrtSumTable = RollbackSqrt::Table<RollbackSqrt::CustomNode<Tp, std::plus<Tp>>, Controller, MAX_LEVEL>;
}

#endif