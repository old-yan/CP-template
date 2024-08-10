/*
最后修改:
20240810
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
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace CAT {
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
            size_type m_size, m_depth;
            void _update(size_type i) {
                node *sub = m_sub[0].data();
                for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1) {
                    node *cur = m_sub[j].data();
                    size_type l = i & -(1 << (j + 1));
                    if (i >> j & 1) {
                        size_type j = i, end = std::min(l + k, m_size);
                        cur[j].set(j == l + (k >> 1) ? sub[j].get() : node::op(cur[j - 1].get(), sub[j].get()));
                        while (++j != end) cur[j].set(node::op(cur[j - 1].get(), sub[j].get()));
                    } else {
                        if (m_size <= l + k / 2) continue;
                        size_type j = i + 1;
                        cur[j - 1].set(j == l + (k >> 1) ? sub[j - 1].get() : node::op(sub[j - 1].get(), cur[j].get()));
                        while (--j != l) cur[j - 1].set(node::op(sub[j - 1].get(), cur[j].get()));
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
                for (size_type i = 0; i != m_depth; i++) m_sub[i].resize(m_size);
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    node *sub = m_sub[0].data();
                    for (size_type i = 0; i != m_size; i++) sub[i].set(mapping(i));
                    for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                        node *cur = m_sub[j].data();
                        for (l = 0; l + k <= m_size; l += k) {
                            size_type i = l + (k >> 1);
                            cur[i - 1].set(sub[i - 1].get());
                            while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                            i = l + (k >> 1);
                            cur[i].set(sub[i].get());
                            while (++i != l + k) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
                        }
                        if (l != m_size && (l + (k >> 1) < m_size)) {
                            size_type i = l + (k >> 1);
                            cur[i - 1].set(sub[i - 1].get());
                            while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                            i = l + (k >> 1);
                            cur[i].set(sub[i].get());
                            while (++i != m_size) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
                        }
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const value_type &inc) { m_sub[0][i].set(node::op(inc, m_sub[i].get())), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[0][i].set(val), _update(i); }
            value_type query(size_type i) const { return m_sub[0][i].get(); }
            value_type query(size_type left, size_type right) const {
                if (left == right) return m_sub[0][left].get();
                size_type d = std::bit_width(left ^ right) - 1;
                return node::op(m_sub[d][left].get(), m_sub[d][right].get());
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left].get();
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ (m_size - 1));
                while (d && left < m_size) {
                    size_type split = (left & -(1 << (d - 1))) | (1 << (d - 1));
                    if (m_size <= split)
                        while (--d && (left >> (d - 1) & 1)) {}
                    else {
                        value_type a = node::op(val, m_sub[d - 1][left].get());
                        if (judge(a))
                            val = a, --d, left = split;
                        else
                            while (--d && (left >> (d - 1) & 1)) {}
                    }
                }
                if (left < m_size && judge(node::op(val, m_sub[0][left].get()))) left++;
                return std::min(left, m_size) - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right].get();
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = node::op(m_sub[d - 1][right].get(), val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(node::op(m_sub[0][right].get(), val))) right--;
                return right + 1;
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
    template <typename Tp, CAT::size_type MAX_LEVEL = 32, typename Operation, typename InitMapping = CAT::Ignore, typename TreeType = CAT::Table<CAT::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(CAT::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <CAT::size_type MAX_LEVEL = 32, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = CAT::Table<CAT::CustomNode<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, CAT::size_type MAX_LEVEL = 32>
    using CatMaxTable = CAT::Table<CAT::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, CAT::size_type MAX_LEVEL = 32>
    using CatMinTable = CAT::Table<CAT::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
    template <typename Tp, CAT::size_type MAX_LEVEL = 32>
    using CatSumTable = CAT::Table<CAT::CustomNode<Tp, std::plus<Tp>>, MAX_LEVEL>;
}

#endif