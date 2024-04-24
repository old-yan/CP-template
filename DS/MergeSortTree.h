/*
最后修改:
20240424
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MERGESORTTREE__
#define __OY_MERGESORTTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace MS {
        using size_type = uint32_t;
        template <typename Tp, typename Compare = std::less<Tp>>
        struct Tree {
            std::vector<Tp> m_sorted;
            std::vector<size_type> m_left;
            size_type m_size, m_depth;
            template <bool CountEqual>
            size_type _count(size_type left, size_type right, const Tp &x) const {
                size_type res{};
                right++;
                auto sorted = m_sorted.data();
                const size_type *_left = m_left.data();
                for (size_type i = 0, j = 1 << (m_depth - 1), cur = 0; i != m_depth && left != right; i++, j >>= 1) {
                    size_type to_left1 = left == cur ? 0 : _left[(i << m_depth) + left - 1], to_left2 = _left[(i << m_depth) + right - 1];
                    if constexpr (CountEqual)
                        if (Compare()(x, sorted[cur + j - 1]))
                            left = cur + to_left1, right = cur + to_left2;
                        else
                            cur += j, res += to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                    else if (!Compare()(sorted[cur + j - 1], x))
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
                m_depth = std::bit_width(m_size - 1);
                m_sorted.resize((m_depth + 1) << m_depth);
                m_left.resize((m_depth + 1) << m_depth);
                Tp *sorted = m_sorted.data();
                std::vector<Tp> items(1 << m_depth), buffer(1 << m_depth);
                std::vector<size_type> left_same(1 << m_depth);
                for (size_type i = 0; i != m_size; i++) items[i] = sorted[i] = mapping(i);
                std::sort(sorted, sorted + m_size, [&](const Tp &x, const Tp &y) { return Compare()(x, y); });
                Tp max = sorted[m_size - 1];
                for (size_type i = m_size; i != 1 << m_depth; i++) items[i] = sorted[i] = max;
                for (size_type i = 0; i != 1 << m_depth; i++) left_same[i] = i && !Compare()(sorted[i - 1], sorted[i]) ? left_same[i - 1] + 1 : 1;
                for (size_type j = 0; j != m_depth; j++) {
                    auto prev = items.data(), cur = buffer.data();
                    size_type *left = m_left.data() + (j << m_depth);
                    for (size_type l = 0, r = 1 << (m_depth - 1 - j), k = r; l != 1 << m_depth; l += k, r += k) {
                        Tp mid_val = sorted[r - 1];
                        size_type mid_same = std::min(k, left_same[r - 1]), cnt = 0;
                        for (size_type i = 0; i != k << 1; i++) {
                            if (Compare()(*prev, mid_val))
                                cur[l++] = *prev++, cnt++;
                            else if (!Compare()(mid_val, *prev) && mid_same)
                                cur[l++] = *prev++, mid_same--, cnt++;
                            else
                                cur[r++] = *prev++;
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
            Tp quantile(size_type left, size_type right, size_type k) const {
                right++;
                const size_type *_left = m_left.data();
                for (size_type i = 0, j = 1 << (m_depth - 1), cur = 0; i != m_depth; i++, j >>= 1) {
                    size_type to_left1 = left == cur ? 0 : _left[(i << m_depth) + left - 1], to_left2 = _left[(i << m_depth) + right - 1];
                    if (to_left1 + k < to_left2)
                        left = cur + to_left1, right = cur + to_left2;
                    else
                        cur += j, k -= to_left2 - to_left1, left += j - to_left1, right += j - to_left2;
                }
                return m_sorted[left];
            }
            size_type rank(size_type left, size_type right, const Tp &x) const { return _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const Tp &x) const { return _count<true>(left, right, x) - _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const { return _count<true>(left, right, maximum) - _count<false>(left, right, minimum); }
            Tp minimum(size_type left, size_type right) const { return quantile(left, right, 0); }
            Tp maximum(size_type left, size_type right) const { return quantile(left, right, right - left); }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, typename InitMapping, typename TreeType = MS::Tree<Tp, Compare>>
    auto make_MSTree(MS::size_type length, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Compare = std::less<void>, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = MS::Tree<Tp, Compare>>
    auto make_MSTree(Iterator first, Iterator last) -> TreeType { return TreeType(first, last); }
}

#endif