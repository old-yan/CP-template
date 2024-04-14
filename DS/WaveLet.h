/*
最后修改:
20240415
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
        using mask_type = uint64_t;
        struct Ignore {};
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        struct BitRank {
            std::vector<mask_type> m_bits;
            std::vector<size_type> m_sum;
            void resize(size_type length) { m_bits.assign((length >> MASK_WIDTH) + 1, 0), m_sum.resize(m_bits.size()); }
            void set(size_type i, mask_type val) { m_bits[i >> MASK_WIDTH] |= val << (i & (MASK_SIZE - 1)); }
            void prepare() {
                for (size_type i = 1; i != m_bits.size(); i++) m_sum[i] = m_sum[i - 1] + std::popcount(m_bits[i - 1]);
            }
            size_type rank1(size_type i) const { return m_sum[i >> MASK_WIDTH] + std::popcount(m_bits[i >> MASK_WIDTH] & ((mask_type(1) << (i & (MASK_SIZE - 1))) - 1)); }
            size_type rank1(size_type l, size_type r) const { return rank1(r) - rank1(l); }
            size_type rank0(size_type i) const { return i - rank1(i); }
            size_type rank0(size_type l, size_type r) const { return rank0(r) - rank0(l); }
        };
        struct VoidTable {
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping) {}
            size_type query(size_type left, size_type right) const { return right - left + 1; }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Table {
            size_type m_size, m_alpha;
            std::vector<BitRank> m_ranks;
            std::vector<size_type> m_pos;
            std::vector<SumTable> m_summer;
            Table() = default;
            template <typename InitMapping, typename TableMapping = Ignore>
            Table(size_type length, InitMapping &&mapping, size_type alpha = 0, TableMapping &&table_mapping = TableMapping()) { resize(length, mapping, alpha, table_mapping); }
            template <typename Iterator, typename TableMapping = Ignore>
            Table(Iterator first, Iterator last, size_type alpha = 0, TableMapping &&table_mapping = TableMapping()) { reset(first, last, alpha, table_mapping); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping &&mapping, size_type alpha = 0, TableMapping &&table_mapping = TableMapping()) {
                static_assert(std::is_unsigned<Tp>::value, "Tp Must Be Unsigned Type");
                if (!(m_size = length)) return;
                std::vector<Tp> numbers(m_size);
                for (size_type i = 0; i != m_size; i++) numbers[i] = mapping(i);
                m_alpha = alpha ? alpha : std::max<uint32_t>(1, std::bit_width(*std::max_element(numbers.begin(), numbers.end())));
                m_ranks.resize(m_alpha), m_pos.resize(m_alpha);
                m_summer.resize(m_alpha);
                for (size_type d = m_alpha - 1; ~d; d--) {
                    m_ranks[d].resize(m_size);
                    for (size_type i = 0; i != m_size; i++) m_ranks[d].set(i, numbers[i] >> d & 1);
                    m_ranks[d].prepare();
                    m_pos[d] = std::stable_partition(numbers.begin(), numbers.end(), [&](size_type val) { return !(val >> d & 1); }) - numbers.begin();
                    if constexpr (std::is_same<typename std::decay<TableMapping>::type, Ignore>::value)
                        m_summer[d].reset(numbers.begin(), numbers.end());
                    else
                        m_summer[d].resize(m_size, [&](size_type i) { return table_mapping(numbers[i]); });
                }
            }
            template <typename Iterator, typename TableMapping = Ignore>
            void reset(Iterator first, Iterator last, size_type alpha = 0, TableMapping &&table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, alpha, table_mapping);
            }
            size_type count(size_type left, size_type right, Tp val) const {
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (!(val >> d & 1))
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr;
                }
                return right - left;
            }
            size_type count(size_type left, size_type right, Tp minimum, Tp maximum) const {
                size_type l1 = left, r1 = right + 1, l2 = left, r2 = right + 1, res = 0;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl1 = m_ranks[d].rank0(l1), zr1 = m_ranks[d].rank0(r1), zl2 = m_ranks[d].rank0(l2), zr2 = m_ranks[d].rank0(r2);
                    if (!(minimum >> d & 1))
                        l1 = zl1, r1 = zr1;
                    else
                        res -= zr1 - zl1, l1 += m_pos[d] - zl1, r1 += m_pos[d] - zr1;
                    if (!(maximum >> d & 1))
                        l2 = zl2, r2 = zr2;
                    else
                        res += zr2 - zl2, l2 += m_pos[d] - zl2, r2 += m_pos[d] - zr2;
                }
                return r2 - l2 + res;
            }
            size_type rank(size_type left, size_type right, Tp val) const {
                size_type ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (!(val >> d & 1))
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans += zr - zl;
                }
                return ans;
            }
            Tp minimum(size_type left, size_type right) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (zl != zr)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp maximum(size_type left, size_type right) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (zr - zl == right - left)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp quantile(size_type left, size_type right, size_type k) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (k < z)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, k -= z, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp max_bitxor(size_type left, size_type right, Tp val) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (val >> d & 1)
                        if (z)
                            left = zl, right = zr, ans |= Tp(1) << d;
                        else
                            left += m_pos[d] - zl, right += m_pos[d] - zr;
                    else if (right - left - z)
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                    else
                        left = zl, right = zr;
                }
                return ans;
            }
            template <typename Callback>
            void do_for_ksmallest(size_type left, size_type right, size_type k, Callback &&call) const {
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (k < z)
                        left = zl, right = zr;
                    else {
                        left += m_pos[d] - zl, right += m_pos[d] - zr, k -= z;
                        if (z) call(m_summer[d].query(zl, zr - 1));
                    }
                }
                if (k) call(m_summer[0].query(left, left + k - 1));
            }
            template <typename Callback>
            void do_for_klargest(size_type left, size_type right, size_type k, Callback &&call) const {
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type one_l = m_ranks[d].rank1(left), one_r = m_ranks[d].rank1(right), one = one_r - one_l;
                    if (k < one)
                        left = m_pos[d] + one_l, right = m_pos[d] + one_r;
                    else {
                        left -= one_l, right -= one_r, k -= one;
                        if (one) call(m_summer[d].query(m_pos[d] + one_l, m_pos[d] + one_r - 1));
                    }
                }
                if (k) call(m_summer[0].query(right - k, right - 1));
            }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Tree {
            Table<size_type, SumTable> m_table;
            std::vector<Tp> m_discretizer;
            size_type m_size;
            size_type _find(const Tp &val) const { return std::lower_bound(m_discretizer.begin(), m_discretizer.end(), val) - m_discretizer.begin(); }
            Tree() = default;
            template <typename InitMapping, typename TableMapping = Ignore>
            Tree(size_type length, InitMapping &&mapping, TableMapping &&table_mapping = TableMapping()) { resize(length, mapping, table_mapping); }
            template <typename Iterator, typename TableMapping = Ignore>
            Tree(Iterator first, Iterator last, TableMapping &&table_mapping = TableMapping()) { reset(first, last, table_mapping); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping &&mapping, TableMapping &&table_mapping = TableMapping()) {
                if (!(m_size = length)) return;
                std::vector<Tp> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = mapping(i);
                m_discretizer = items;
                std::sort(m_discretizer.begin(), m_discretizer.end());
                m_discretizer.erase(std::unique(m_discretizer.begin(), m_discretizer.end()), m_discretizer.end());
                if constexpr (std::is_same<typename std::decay<TableMapping>::type, Ignore>::value)
                    m_table.resize(
                        m_size, [&](size_type i) { return _find(items[i]); }, std::bit_width(m_discretizer.size()), [&](size_type val) { return m_discretizer[val]; });
                else
                    m_table.resize(
                        m_size, [&](size_type i) { return _find(items[i]); }, std::bit_width(m_discretizer.size()), [&](size_type val) { return table_mapping(m_discretizer[val]); });
            }
            template <typename Iterator, typename TableMapping>
            void reset(Iterator first, Iterator last, TableMapping &&table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping);
            }
            size_type count(size_type left, size_type right, const Tp &val) const {
                size_type find = _find(val);
                return find < m_discretizer.size() && m_discretizer[find] == val ? m_table.count(left, right, find) : 0;
            }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                size_type find1 = _find(minimum);
                if (find1 == m_discretizer.size()) return 0;
                size_type find2 = _find(maximum);
                return find2 < m_discretizer.size() && m_discretizer[find2] == maximum ? m_table.count(left, right, find1, find2) : m_table.count(left, right, find1, find2 - 1);
            }
            size_type rank(size_type left, size_type right, const Tp &val) const { return m_table.rank(left, right, _find(val)); }
            Tp minimum(size_type left, size_type right) const { return m_discretizer[m_table.minimum(left, right)]; }
            Tp maximum(size_type left, size_type right) const { return m_discretizer[m_table.maximum(left, right)]; }
            Tp quantile(size_type left, size_type right, size_type k) const { return m_discretizer[m_table.quantile(left, right, k)]; }
            template <typename Callback>
            void do_for_ksmallest(size_type left, size_type right, size_type k, Callback &&call) const { m_table.do_for_ksmallest(left, right, k, call); }
            template <typename Callback>
            void do_for_klargest(size_type left, size_type right, size_type k, Callback &&call) const { m_table.do_for_klargest(left, right, k, call); }
        };
    }
}

#endif