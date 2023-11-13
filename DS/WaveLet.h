/*
最后修改:
20231113
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_WAVELET__
#define __OY_WAVELET__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace WaveLet {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename MaskType = uint64_t, size_type MAX_NODE = 1 << 20>
        struct Table {
            static constexpr size_type block_size = sizeof(MaskType) << 3, mask_width = block_size / 32 + 4;
            struct node {
                MaskType m_bit;
                size_type m_sum;
            };
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_size, m_stride, m_alpha;
            static bool _bit(Tp val, size_type i) { return val >> i & Tp(1); }
            static size_type _count_one(node *cur, size_type i) { return cur[i >> mask_width].m_sum + std::popcount(cur[(i >> mask_width) + 1].m_bit & ((MaskType(1) << (i & (block_size - 1))) - 1)); }
            template <typename InitMapping = Ignore>
            Table(size_type length = 0, InitMapping mapping = InitMapping(), size_type alpha = sizeof(Tp) << 3) { resize(length, mapping, alpha); }
            template <typename Iterator>
            Table(Iterator first, Iterator last, size_type alpha = sizeof(Tp) << 3) { reset(first, last, alpha); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping(), size_type alpha = sizeof(Tp) << 3) {
                if (!(m_size = length)) return;
                m_alpha = alpha, m_stride = (m_size >> mask_width) + 2, m_sub = s_buffer + s_use_count, s_use_count += m_stride * m_alpha;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    std::vector<Tp> numbers(m_size);
                    for (size_type i = 0; i != m_size; i++) numbers[i] = mapping(i);
                    node *cur = m_sub;
                    for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                        for (size_type i = 0; i != m_size; i++)
                            if (_bit(numbers[i], j)) cur[((i + 1) >> mask_width) + 1].m_bit |= MaskType(1) << ((i + 1) & (block_size - 1));
                        for (size_type i = 1; i < m_stride; i++) cur[i].m_sum = cur[i - 1].m_sum + std::popcount(cur[i].m_bit);
                        std::stable_partition(numbers.begin(), numbers.end(), [&](size_type val) { return !_bit(val, j); });
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, size_type alpha = sizeof(Tp) << 3) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, alpha);
            }
            size_type count(size_type left, size_type right, Tp val) const {
                node *cur = m_sub;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1);
                    if (!_bit(val, j))
                        left -= a, right -= b;
                    else
                        left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b;
                }
                return right - left;
            }
            size_type count(size_type left, size_type right, Tp minimum, Tp maximum) const {
                node *cur = m_sub;
                size_type l1 = left, r1 = right + 1, l2 = left, r2 = right + 1, res = 0;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a1 = _count_one(cur, l1 + 1), b1 = _count_one(cur, r1 + 1), a2 = _count_one(cur, l2 + 1), b2 = _count_one(cur, r2 + 1), c1 = r1 - l1 - b1 + a1, c2 = r2 - l2 - b2 + a2;
                    if (!_bit(minimum, j))
                        l1 -= a1, r1 -= b1;
                    else
                        res -= c1, l1 = m_size - cur[m_stride - 1].m_sum + a1, r1 = m_size - cur[m_stride - 1].m_sum + b1;
                    if (!_bit(maximum, j))
                        l2 -= a2, r2 -= b2;
                    else
                        res += c2, l2 = m_size - cur[m_stride - 1].m_sum + a2, r2 = m_size - cur[m_stride - 1].m_sum + b2;
                }
                return r2 - l2 + res;
            }
            size_type rank(size_type left, size_type right, Tp val) const {
                node *cur = m_sub;
                size_type ans = 0;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1), c = right - left - b + a;
                    if (!_bit(val, j))
                        left -= a, right -= b;
                    else
                        ans += c, left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b;
                }
                return ans;
            }
            Tp minimum(size_type left, size_type right) const {
                node *cur = m_sub;
                Tp ans = 0;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1);
                    if (right - left - b + a)
                        left -= a, right -= b;
                    else
                        left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b, ans |= Tp(1) << j;
                }
                return ans;
            }
            Tp maximum(size_type left, size_type right) const {
                node *cur = m_sub;
                Tp ans = 0;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1);
                    if (a == b)
                        left -= a, right -= b;
                    else
                        left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b, ans |= Tp(1) << j;
                }
                return ans;
            }
            Tp quantile(size_type left, size_type right, size_type k) const {
                node *cur = m_sub;
                Tp ans = 0;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1), c = right - left - b + a;
                    if (k < c)
                        left -= a, right -= b;
                    else
                        left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b, k -= c, ans |= Tp(1) << j;
                }
                return ans;
            }
            Tp max_bitxor(size_type left, size_type right, Tp val) const {
                node *cur = m_sub;
                Tp ans = 0;
                right++;
                for (size_type j = m_alpha - 1; ~j; j--, cur += m_stride) {
                    size_type a = _count_one(cur, left + 1), b = _count_one(cur, right + 1), c = right - left - b + a;
                    if (val >> j & 1)
                        if (c)
                            left -= a, right -= b, ans |= Tp(1) << j;
                        else
                            left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b;
                    else if (a != b)
                        left = m_size - cur[m_stride - 1].m_sum + a, right = m_size - cur[m_stride - 1].m_sum + b, ans |= Tp(1) << j;
                    else
                        left -= a, right -= b;
                }
                return ans;
            }
        };
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        typename Table<Tp, MaskType, MAX_NODE>::node Table<Tp, MaskType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        size_type Table<Tp, MaskType, MAX_NODE>::s_use_count;
        template <typename Tp, typename MaskType = uint64_t, size_type MAX_NODE = 1 << 20>
        struct Tree {
            static Tp s_buffer[MAX_NODE];
            static size_type s_use_count;
            Table<size_type, MaskType, MAX_NODE> m_table;
            Tp *m_discretizer;
            size_type m_size, m_kind;
            size_type _find(const Tp &val) const { return std::lower_bound(m_discretizer, m_discretizer + m_kind, val) - m_discretizer; }
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_discretizer = s_buffer + s_use_count, s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    std::vector<Tp> items(m_size);
                    for (size_type i = 0; i < m_size; i++) items[i] = m_discretizer[i] = mapping(i);
                    std::sort(m_discretizer, m_discretizer + m_size);
                    m_kind = std::unique(m_discretizer, m_discretizer + m_size) - m_discretizer;
                    m_table.resize(
                        m_size, [&](size_type i) { return _find(items[i]); }, std::bit_width(m_kind - 1));
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type count(size_type left, size_type right, const Tp &val) const {
                size_type find = _find(val);
                return find < m_kind && m_discretizer[find] == val ? m_table.count(left, right, find) : 0;
            }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                size_type find1 = _find(minimum);
                if (find1 == m_kind) return 0;
                size_type find2 = _find(maximum);
                return find2 < m_kind && m_discretizer[find2] == maximum ? m_table.count(left, right, find1, find2) : m_table.count(left, right, find1, find2 - 1);
            }
            size_type rank(size_type left, size_type right, const Tp &val) const { return m_table.rank(left, right, _find(val)); }
            Tp minimum(size_type left, size_type right) const { return m_discretizer[m_table.minimum(left, right)]; }
            Tp maximum(size_type left, size_type right) const { return m_discretizer[m_table.maximum(left, right)]; }
            Tp quantile(size_type left, size_type right, size_type k) const { return m_discretizer[m_table.quantile(left, right, k)]; }
        };
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        Tp Tree<Tp, MaskType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        size_type Tree<Tp, MaskType, MAX_NODE>::s_use_count;
    }
    template <typename Tp, typename MaskType = uint64_t, WaveLet::size_type MAX_NODE = 1 << 20>
    using WaveLetTable = WaveLet::Table<Tp, MaskType, MAX_NODE>;
    template <typename Tp, typename MaskType = uint64_t, WaveLet::size_type MAX_NODE = 1 << 20>
    using WaveLetTree = WaveLet::Tree<Tp, MaskType, MAX_NODE>;
}

#endif