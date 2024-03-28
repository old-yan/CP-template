/*
最后修改:
20240326
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
            template <bool CountEqual>
            size_type _count(size_type left, size_type right, const value_type &x) const {
                size_type res{};
                right++;
                for (size_type i = 0, j = 1 << (m_depth - 1), cur = 0; i != m_depth && left != right; i++, j >>= 1) {
                    size_type to_left1 = left == cur ? 0 : m_left[(i << m_depth) + left - 1], to_left2 = m_left[(i << m_depth) + right - 1];
                    if constexpr (CountEqual)
                        if (node::comp(x, m_sorted[cur + j - 1].get()))
                            left = cur + to_left1, right = cur + to_left2;
                        else
                            cur += j, res += to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                    else if (!node::comp(m_sorted[cur + j - 1].get(), x))
                        left = cur + to_left1, right = cur + to_left2;
                    else
                        cur += j, res += to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                }
                return res;
            }
            Tree() = default;
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
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
                right++;
                for (size_type i = 0, j = 1 << (m_depth - 1), cur = 0; i != m_depth; i++, j >>= 1) {
                    size_type to_left1 = left == cur ? 0 : m_left[(i << m_depth) + left - 1], to_left2 = m_left[(i << m_depth) + right - 1];
                    if (to_left1 + k < to_left2)
                        left = cur + to_left1, right = cur + to_left2;
                    else
                        cur += j, k -= to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                }
                return m_sorted[left].get();
            }
            size_type rank(size_type left, size_type right, const value_type &x) const { return _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const value_type &x) const { return _count<true>(left, right, x) - _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const value_type &minimum, const value_type &maximum) const { return _count<true>(left, right, maximum) - _count<false>(left, right, minimum); }
            value_type minimum(size_type left, size_type right) const { return quantile(left, right, 0); }
            value_type maximum(size_type left, size_type right) const { return quantile(left, right, right - left); }
        };
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_left[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
    }
    template <typename Tp, typename Compare = std::less<Tp>, DIVIDE::size_type MAX_NODE = 1 << 22, typename InitMapping, typename TreeType = DIVIDE::Tree<DIVIDE::BaseNode<Tp, Compare>, MAX_NODE>>
    auto make_DivideTree(DIVIDE::size_type length, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Compare = std::less<void>, DIVIDE::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = DIVIDE::Tree<DIVIDE::BaseNode<Tp, Compare>, MAX_NODE>>
    auto make_DivideTree(Iterator first, Iterator last) -> TreeType { return TreeType(first, last); }
    template <typename Tp, typename Compare = std::less<Tp>, DIVIDE::size_type MAX_NODE = 1 << 22>
    using DivideTree = DIVIDE::Tree<DIVIDE::BaseNode<Tp, Compare>, MAX_NODE>;
}

#endif