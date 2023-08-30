/*
最后修改:
20230828
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

#include "TEST/mystd.h"

namespace OY {
    namespace WaveLet {
        using size_type = size_t;
        struct NoInit {};
        template <typename Tp, typename MaskType = uint64_t, size_type MAX_NODE = 1 << 20>
        struct Table {
            static constexpr size_type block_size = sizeof(MaskType) << 3, mask_width = block_size / 32 + 4;
            static size_type s_buffer[MAX_NODE], s_use_count;
            static MaskType s_mask_buffer[MAX_NODE];
            MaskType *m_bits;
            size_type *m_sum, m_size, m_stride, m_alpha;
            static bool _bit(Tp val, size_type i) { return val >> i & Tp(1); }
            static size_type _count_one(MaskType *cur_bits, size_type *cur_sum, size_type i) { return cur_sum[i >> mask_width] + std::popcount(cur_bits[(i >> mask_width) + 1] & ((MaskType(1) << (i & (block_size - 1))) - 1)); }
            template <typename InitMapping = NoInit>
            Table(size_type length = 0, InitMapping mapping = InitMapping(), size_type alpha = sizeof(Tp) << 3) { resize(length, mapping, alpha); }
            template <typename Iterator>
            Table(Iterator first, Iterator last, size_type alpha = sizeof(Tp) << 3) { reset(first, last, alpha); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping(), size_type alpha = sizeof(Tp) << 3) {
                if (!(m_size = length)) return;
                m_alpha = alpha, m_stride = (m_size >> 6) + 2, m_sum = s_buffer + s_use_count, m_bits = s_mask_buffer + s_use_count;
                s_use_count += m_stride * m_alpha;
                if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                    std::vector<Tp> numbers(m_size);
                    for (size_type i = 0; i < m_size; i++) numbers[i] = mapping(i);
                    MaskType *cur_bits = m_bits;
                    size_type *cur_sum = m_sum;
                    for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                        for (size_type i = 0; i < m_size; i++)
                            if (_bit(numbers[i], j)) cur_bits[((i + 1) >> mask_width) + 1] |= MaskType(1) << ((i + 1) & (block_size - 1));
                        for (size_type i = 1; i < m_stride; i++) cur_sum[i] = cur_sum[i - 1] + std::popcount(cur_bits[i]);
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
                MaskType *cur_bits = m_bits;
                size_type *cur_sum = m_sum;
                for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                    size_type a = _count_one(cur_bits, cur_sum, left + 1), b = _count_one(cur_bits, cur_sum, right + 2);
                    if (!_bit(val, j))
                        left -= a, right -= b;
                    else
                        left = m_size - cur_sum[m_stride - 1] + a, right = m_size - cur_sum[m_stride] + b - 1;
                }
                return right - left + 1;
            }
            size_type rank(size_type left, size_type right, Tp val) const {
                MaskType *cur_bits = m_bits;
                size_type *cur_sum = m_sum;
                size_type ans = 0;
                for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                    size_type a = _count_one(cur_bits, cur_sum, left + 1), b = _count_one(cur_bits, cur_sum, right + 2), c = right - left + 1 - b + a;
                    if (!_bit(val, j))
                        left -= a, right -= b;
                    else
                        ans += c, left = m_size - cur_sum[m_stride - 1] + a, right = m_size - cur_sum[m_stride - 1] + b - 1;
                }
                return ans;
            }
            Tp minimum(size_type left, size_type right) const {
                MaskType *cur_bits = m_bits;
                size_type *cur_sum = m_sum;
                Tp ans = 0;
                for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                    size_type a = _count_one(cur_bits, cur_sum, left + 1), b = _count_one(cur_bits, cur_sum, right + 2);
                    if (right - left + 1 - b + a)
                        left -= a, right -= b;
                    else
                        left = m_size - cur_sum[m_stride - 1] + a, right = m_size - cur_sum[m_stride - 1] + b - 1, ans |= Tp(1) << j;
                }
                return ans;
            }
            Tp maximum(size_type left, size_type right) const {
                MaskType *cur_bits = m_bits;
                size_type *cur_sum = m_sum;
                Tp ans = 0;
                for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                    size_type a = _count_one(cur_bits, cur_sum, left + 1), b = _count_one(cur_bits, cur_sum, right + 2);
                    if (a == b)
                        left -= a, right -= b;
                    else
                        left = m_size - cur_sum[m_stride - 1] + a, right = m_size - cur_sum[m_stride - 1] + b - 1, ans |= Tp(1) << j;
                }
                return ans;
            }
            Tp quantile(size_type left, size_type right, size_type k) const {
                MaskType *cur_bits = m_bits;
                size_type *cur_sum = m_sum;
                Tp ans = 0;
                for (size_type j = m_alpha - 1; ~j; j--, cur_bits += m_stride, cur_sum += m_stride) {
                    size_type a = _count_one(cur_bits, cur_sum, left + 1), b = _count_one(cur_bits, cur_sum, right + 2), c = right - left + 1 - b + a;
                    if (k < c)
                        left -= a, right -= b;
                    else
                        left = m_size - cur_sum[m_stride - 1] + a, right = m_size - cur_sum[m_stride - 1] + b - 1, k -= c, ans |= Tp(1) << j;
                }
                return ans;
            }
        };
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        size_type Table<Tp, MaskType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, typename MaskType, size_type MAX_NODE>
        MaskType Table<Tp, MaskType, MAX_NODE>::s_mask_buffer[MAX_NODE];
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
            template <typename InitMapping = NoInit>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_discretizer = s_buffer + s_use_count, s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, NoInit>::value) {
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
            size_type count(size_type left, size_type right, const Tp &val) const { return m_table.count(left, right, _find(val)); }
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