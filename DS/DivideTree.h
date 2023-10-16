/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DIVIDETREE__
#define __OY_DIVIDETREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace DIVIDE {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType, typename Compare = std::less<ValueType>>
        struct BaseNode {
            using value_type = ValueType;
            static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename Node, size_type MAX_NODE>
        struct Tree {
            using node = Node;
            using value_type = typename node::value_type;
            static node s_buffer[MAX_NODE];
            static size_type s_left[MAX_NODE], s_use_count;
            node *m_sorted;
            size_type *m_left, m_size, m_depth;
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size - 1), m_sorted = s_buffer + s_use_count, m_left = s_left + s_use_count, s_use_count += (m_depth + 1) << m_depth;
                std::vector<node> items(1 << m_depth), buffer(1 << m_depth);
                std::vector<size_type> left_same(1 << m_depth);
                for (size_type i = 0; i != m_size; i++) m_sorted[i].set(mapping(i)), items[i].set(m_sorted[i].get());
                std::sort(m_sorted, m_sorted + m_size, [&](const node &x, const node &y) { return node::comp(x.get(), y.get()); });
                value_type max = m_sorted[m_size - 1].get();
                for (size_type i = m_size; i != 1 << m_depth; i++) items[i].set(max), m_sorted[i].set(max);
                for (size_type i = 0; i != 1 << m_depth; i++) left_same[i] = i && !node::comp(m_sorted[i - 1].get(), m_sorted[i].get()) ? left_same[i - 1] + 1 : 1;
                for (size_type j = 0; j != m_depth; j++) {
                    auto prev = items.begin(), cur = buffer.begin();
                    size_type *left = m_left + (j << m_depth);
                    for (size_type l = 0, r = 1 << (m_depth - 1 - j), k = r; l != 1 << m_depth; l += k, r += k) {
                        value_type mid_val = m_sorted[r - 1].get();
                        size_type mid_same = std::min(k, left_same[r - 1]), cnt = 0;
                        for (size_type i = 0; i != k << 1; i++) {
                            if (node::comp(prev->get(), mid_val))
                                (cur + l++)->set(prev++->get()), cnt++;
                            else if (!node::comp(mid_val, prev->get()) && mid_same)
                                (cur + l++)->set(prev++->get()), mid_same--, cnt++;
                            else
                                (cur + r++)->set(prev++->get());
                            *left++ = cnt;
                        }
                    }
                    std::swap(items, buffer);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            value_type quantile(size_type left, size_type right, size_type k) const {
                for (size_type i = 0, j = 1 << (m_depth - 1 - i), cur = 0; i < m_depth; i++, j >>= 1) {
                    size_type to_left1 = left == cur ? 0 : m_left[(i << m_depth) + left - 1], to_left2 = m_left[(i << m_depth) + right];
                    if (to_left1 + k < to_left2)
                        left = cur + to_left1, right = cur + to_left2 - 1;
                    else
                        cur += j, k -= to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                }
                return m_sorted[left].get();
            }
        };
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_left[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
    }
    template <typename Tp, typename Compare = std::less<Tp>, DIVIDE::size_type MAX_NODE = 1 << 22, typename InitMapping = DIVIDE::Ignore, typename TreeType = DIVIDE::Tree<DIVIDE::BaseNode<Tp, Compare>, MAX_NODE>>
    auto make_DivideTree(DIVIDE::size_type length, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Compare = std::less<void>, DIVIDE::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = DIVIDE::Tree<DIVIDE::BaseNode<Tp, Compare>, MAX_NODE>>
    auto make_DivideTree(Iterator first, Iterator last) -> TreeType { return TreeType(first, last); }
    template <typename Node, DIVIDE::size_type MAX_NODE = 1 << 22>
    using DivideTree = DIVIDE::Tree<Node, MAX_NODE>;
}

#endif