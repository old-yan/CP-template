/*
最后修改:
20240506
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_WAVELET__
#define __OY_WAVELET__

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace WaveLet {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        struct Ignore {};
        struct DefaultFilter {
            static constexpr bool get_left(size_type w) { return false; }
        };
        template <typename Tp>
        struct BitxorFilter {
            Tp m_val;
            Tp get_left(size_type w) const { return m_val >> w & 1; }
        };
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
        struct DefaultQuerier {
            template <typename Callback, typename Tr>
            void operator()(Callback &&call, Tr &&tr, size_type left, size_type right) const { call(tr.query(left, right)); }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Table {
            static constexpr size_type mask_size = sizeof(Tp) << 3;
            size_type m_size, m_alpha;
            std::vector<BitRank> m_ranks;
            std::vector<size_type> m_pos;
            std::vector<SumTable> m_sumer;
            Tp _mask() const { return (Tp(1) << m_alpha) - 1; }
            template <typename Filter = DefaultFilter>
            size_type _rank(size_type l, size_type r, Tp val, Filter &&filter) const {
                if (val >> m_alpha) return r - l;
                size_type ans{};
                Tp cur = val << (mask_size - m_alpha);
                for (size_type d = m_alpha - 1; cur && l != r; d--, cur <<= 1) {
                    size_type zl = m_ranks[d].rank0(l), zr = m_ranks[d].rank0(r);
                    ans += (val >> d & 1) * (filter.get_left(d) ? r - l - (zr - zl) : zr - zl);
                    if ((val >> d & 1) == filter.get_left(d))
                        l = zl, r = zr;
                    else
                        l += m_pos[d] - zl, r += m_pos[d] - zr;
                }
                return ans;
            }
            template <typename Filter = DefaultFilter>
            bool _any(size_type l, size_type r, Tp min, Tp max, Filter &&filter) const {
                size_type d = m_alpha - 1, l1, r1, l2, r2;
                for (;; d--) {
                    if (l == r) return false;
                    if (!~d) return true;
                    size_type zl = m_ranks[d].rank0(l), zr = m_ranks[d].rank0(r);
                    if ((min >> d) == (max >> d)) {
                        if ((min >> d & 1) == filter.get_left(d))
                            l = zl, r = zr;
                        else
                            l += m_pos[d] - zl, r += m_pos[d] - zr;
                    } else {
                        if (filter.get_left(d))
                            l1 = l + m_pos[d] - zl, r1 = r + m_pos[d] - zr, l2 = zl, r2 = zr;
                        else
                            l1 = zl, r1 = zr, l2 = l + m_pos[d] - zl, r2 = r + m_pos[d] - zr;
                        break;
                    }
                }
                if (!d) return l != r;
                Tp cur = min << (mask_size - d);
                for (size_type d2 = d - 1; cur && l1 != r1; d2--, cur <<= 1) {
                    size_type zl = m_ranks[d2].rank0(l1), zr = m_ranks[d2].rank0(r1);
                    if (!(min >> d2 & 1) && (filter.get_left(d2) ? (zr != zl) : (r1 - l1 != zr - zl))) return true;
                    if ((min >> d2 & 1) == filter.get_left(d2))
                        l1 = zl, r1 = zr;
                    else
                        l1 += m_pos[d2] - zl, r1 += m_pos[d2] - zr;
                }
                if (l1 != r1) return true;
                Tp cur2 = (~max) << (mask_size - d);
                for (size_type d2 = d - 1; cur2 && l2 != r2; d2--, cur2 <<= 1) {
                    size_type zl = m_ranks[d2].rank0(l2), zr = m_ranks[d2].rank0(r2);
                    if ((max >> d2 & 1) && (filter.get_left(d2) ? (r2 - l2 != zr - zl) : (zr != zl))) return true;
                    if ((max >> d2 & 1) == filter.get_left(d2))
                        l2 = zl, r2 = zr;
                    else
                        l2 += m_pos[d2] - zl, r2 += m_pos[d2] - zr;
                }
                return l2 != r2;
            }
            template <typename Filter = DefaultFilter>
            Tp _quantile(size_type l, size_type r, size_type k, Filter &&filter) const {
                Tp ans{};
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(l), zr = m_ranks[d].rank0(r), z = zr - zl;
                    if (filter.get_left(d))
                        if (k < r - l - z)
                            l += m_pos[d] - zl, r += m_pos[d] - zr;
                        else
                            k -= r - l - z, l = zl, r = zr, ans |= Tp(1) << d;
                    else if (k < z)
                        l = zl, r = zr;
                    else
                        l += m_pos[d] - zl, r += m_pos[d] - zr, k -= z, ans |= Tp(1) << d;
                }
                return ans;
            }
            template <typename Callback, typename Querier = DefaultQuerier, typename Filter = DefaultFilter>
            void _do_for_value_range(size_type l, size_type r, Tp floor, Tp ceil, Callback &&call, Querier &&q, Filter &&filter) const {
                auto handle = [&](size_type l1, size_type r1, size_type l2, size_type r2, size_type d) {
                    if (!d)
                        q(call, m_sumer[0], l1, r1 - 1), q(call, m_sumer[0], l2, r2 - 1);
                    else {
                        Tp cur = (floor << (mask_size - d));
                        size_type d1, d2;
                        for (d1 = d - 1; cur && l1 != r1; d1--, cur <<= 1) {
                            size_type zl = m_ranks[d1].rank0(l1), zr = m_ranks[d1].rank0(r1), z = zr - zl;
                            if (!(floor >> d1 & 1))
                                if (filter.get_left(d1)) {
                                    if (z) q(call, m_sumer[d1], zl, zr - 1);
                                } else if (z != r1 - l1)
                                    q(call, m_sumer[d1], l1 + m_pos[d1] - zl, r1 + m_pos[d1] - zr - 1);
                            if ((floor >> d1 & 1) == filter.get_left(d1))
                                l1 = zl, r1 = zr;
                            else
                                l1 += m_pos[d1] - zl, r1 += m_pos[d1] - zr;
                        }
                        Tp cur2 = ((~ceil) << (mask_size - d));
                        for (d2 = d - 1; cur2 && l2 != r2; d2--, cur2 <<= 1) {
                            size_type zl = m_ranks[d2].rank0(l2), zr = m_ranks[d2].rank0(r2), z = zr - zl;
                            if (ceil >> d2 & 1)
                                if (filter.get_left(d2)) {
                                    if (z != r2 - l2) q(call, m_sumer[d2], l2 + m_pos[d2] - zl, r2 + m_pos[d2] - zr - 1);
                                } else if (z)
                                    q(call, m_sumer[d2], zl, zr - 1);
                            if ((ceil >> d2 & 1) == filter.get_left(d2))
                                l2 = zl, r2 = zr;
                            else
                                l2 += m_pos[d2] - zl, r2 += m_pos[d2] - zr;
                        }
                        if (l1 != r1) q(call, m_sumer[d1 + 1], l1, r1 - 1);
                        if (l2 != r2) q(call, m_sumer[d2 + 1], l2, r2 - 1);
                    }
                };
                for (size_type d = m_alpha - 1; ~d && l != r; d--) {
                    size_type zl = m_ranks[d].rank0(l), zr = m_ranks[d].rank0(r), z = zr - zl;
                    if ((floor >> d & 1) == (ceil >> d & 1))
                        if ((floor >> d & 1) == filter.get_left(d))
                            l = zl, r = zr;
                        else
                            l += m_pos[d] - zl, r += m_pos[d] - zr;
                    else if (filter.get_left(d))
                        return handle(l + m_pos[d] - zl, r + m_pos[d] - zr, zl, zr, d);
                    else
                        return handle(zl, zr, l + m_pos[d] - zl, r + m_pos[d] - zr, d);
                }
                if (l != r) q(call, m_sumer[0], l, r - 1);
            }
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping, size_type alpha = 0) { resize(length, mapping, alpha); }
            template <typename Iterator>
            Table(Iterator first, Iterator last, size_type alpha = 0) { reset(first, last, alpha); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping mapping, size_type alpha = 0, TableMapping table_mapping = TableMapping()) {
                static_assert(std::is_unsigned<Tp>::value, "Tp Must Be Unsigned Type");
                if (!(m_size = length)) return;
                std::vector<Tp> numbers(m_size);
                for (size_type i = 0; i != m_size; i++) numbers[i] = mapping(i);
                m_alpha = alpha ? alpha : std::max<size_type>(1, std::bit_width(*std::max_element(numbers.begin(), numbers.end())));
                m_ranks.resize(m_alpha), m_pos.resize(m_alpha);
                m_sumer.resize(m_alpha);
                for (size_type d = m_alpha - 1; ~d; d--) {
                    m_ranks[d].resize(m_size);
                    for (size_type i = 0; i != m_size; i++) m_ranks[d].set(i, numbers[i] >> d & 1);
                    m_ranks[d].prepare();
                    m_pos[d] = std::stable_partition(numbers.begin(), numbers.end(), [&](size_type val) { return !(val >> d & 1); }) - numbers.begin();
                    if constexpr (std::is_same<TableMapping, Ignore>::value)
                        m_sumer[d].reset(numbers.begin(), numbers.end());
                    else
                        m_sumer[d].resize(m_size, [&](size_type i) { return table_mapping(numbers[i]); });
                }
            }
            template <typename InitMapping, typename TableMapping>
            void resize_mapping_with_index(size_type length, InitMapping mapping, size_type alpha, TableMapping table_mapping) {
                static_assert(std::is_unsigned<Tp>::value, "Tp Must Be Unsigned Type");
                if (!(m_size = length)) return;
                struct pair {
                    Tp m_val;
                    size_type m_index;
                };
                std::vector<pair> numbers(m_size);
                for (size_type i = 0; i != m_size; i++) numbers[i] = {mapping(i), i};
                m_alpha = alpha ? alpha : std::max<size_type>(1, std::bit_width(std::max_element(numbers.begin(), numbers.end(), [](pair &x, pair &y) { return x.m_val < y.m_val; })->m_val));
                m_ranks.resize(m_alpha), m_pos.resize(m_alpha);
                m_sumer.resize(m_alpha);
                for (size_type d = m_alpha - 1; ~d; d--) {
                    m_ranks[d].resize(m_size);
                    for (size_type i = 0; i != m_size; i++) m_ranks[d].set(i, numbers[i].m_val >> d & 1);
                    m_ranks[d].prepare();
                    m_pos[d] = std::stable_partition(numbers.begin(), numbers.end(), [&](pair &p) { return !(p.m_val >> d & 1); }) - numbers.begin();
                    m_sumer[d].resize(m_size, [&](size_type i) { return table_mapping(numbers[i].m_val, numbers[i].m_index); });
                }
            }
            template <typename Iterator, typename TableMapping = Ignore>
            void reset(Iterator first, Iterator last, size_type alpha = 0, TableMapping table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, alpha, table_mapping);
            }
            template <typename Iterator, typename TableMapping>
            void reset_mapping_with_index(Iterator first, Iterator last, size_type alpha, TableMapping table_mapping) {
                resize_mapping_with_index(
                    last - first, [&](size_type i) { return *(first + i); }, alpha, table_mapping);
            }
            size_type count(size_type left, size_type right, Tp val) const {
                right++;
                for (size_type d = m_alpha - 1; ~d && left != right; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (!(val >> d & 1))
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr;
                }
                return right - left;
            }
            size_type rank(size_type left, size_type right, Tp val) const { return _rank(left, right + 1, val, {}); }
            size_type rank_bitxor(size_type left, size_type right, Tp xor_by, Tp val) const {
                if ((val >> m_alpha) != (xor_by >> m_alpha)) return val < xor_by ? 0 : right - left + 1;
                return _rank(left, right + 1, val & _mask(), BitxorFilter<Tp>{xor_by & _mask()});
            }
            size_type count(size_type left, size_type right, Tp minimum, Tp maximum) const { return (~maximum ? rank(left, right, maximum + 1) : right - left + 1) - rank(left, right, minimum); }
            size_type count_bitxor(size_type left, size_type right, Tp xor_by, Tp minimum, Tp maximum) const { return (~maximum ? rank_bitxor(left, right, xor_by, maximum + 1) : right - left + 1) - rank_bitxor(left, right, xor_by, minimum); }
            bool any(size_type left, size_type right, Tp min, Tp max) const {
                max = std::min(max, _mask());
                if (min > max) return false;
                return _any(left, right + 1, min, max, {});
            }
            bool any_bitxor(size_type left, size_type right, Tp xor_by, Tp min, Tp max) const {
                if ((xor_by >> m_alpha) < (min >> m_alpha) || (xor_by >> m_alpha) > (max >> m_alpha)) return false;
                Tp _min = (xor_by >> m_alpha) == (min >> m_alpha) ? (min & _mask()) : 0;
                Tp _max = (xor_by >> m_alpha) == (max >> m_alpha) ? (max & _mask()) : _mask();
                return _any(left, right + 1, _min, _max, BitxorFilter<Tp>{xor_by});
            }
            Tp quantile(size_type left, size_type right, size_type k) const { return _quantile(left, right + 1, k, {}); }
            Tp quantile_bitxor(size_type left, size_type right, Tp xor_by, size_type k) const { return _quantile(left, right + 1, k, BitxorFilter<Tp>{xor_by}); }
            Tp minimum(size_type left, size_type right) const { return quantile(left, right, 0); }
            Tp min_bitxor(size_type left, size_type right, Tp xor_by) const { return quantile_bitxor(left, right, xor_by, 0); }
            Tp maximum(size_type left, size_type right) const { return quantile(left, right, right - left); }
            Tp max_bitxor(size_type left, size_type right, Tp xor_by) const { return quantile_bitxor(left, right, xor_by, right - left); }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_rank_range(size_type left, size_type right, size_type rk1, size_type rk2, Callback &&call, Querier &&q = Querier()) const {
                right++;
                auto handle = [&](size_type l1, size_type r1, size_type l2, size_type r2, size_type k1, size_type k2, size_type d) {
                    for (; ~d; d--) {
                        size_type one_l = m_ranks[d].rank1(l1), one_r = m_ranks[d].rank1(r1), one = one_r - one_l, zl = m_ranks[d].rank0(l2), zr = m_ranks[d].rank0(r2), z = zr - zl;
                        if (k1 < one)
                            l1 = m_pos[d] + one_l, r1 = m_pos[d] + one_r;
                        else {
                            l1 -= one_l, r1 -= one_r, k1 -= one;
                            if (one) q(call, m_sumer[d], m_pos[d] + one_l, m_pos[d] + one_r - 1);
                        }
                        if (k2 < z)
                            l2 = zl, r2 = zr;
                        else {
                            l2 += m_pos[d] - zl, r2 += m_pos[d] - zr, k2 -= z;
                            if (z) q(call, m_sumer[d], zl, zr - 1);
                        }
                    }
                    if (k1) q(call, m_sumer[0], r1 - k1, r1 - 1);
                    if (k2) q(call, m_sumer[0], l2, l2 + k2 - 1);
                };
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (rk2 < z)
                        left = zl, right = zr;
                    else if (rk1 >= z)
                        left += m_pos[d] - zl, right += m_pos[d] - zr, rk1 -= z, rk2 -= z;
                    else
                        return handle(zl, zr, left + m_pos[d] - zl, right + m_pos[d] - zr, z - rk1, rk2 - z + 1, d - 1);
                }
                if (left != right) q(call, m_sumer[0], left + rk1, left + rk2);
            }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_value_range(size_type left, size_type right, Tp floor, Tp ceil, Callback &&call, Querier &&q = Querier()) const {
                ceil = std::min(ceil, _mask());
                if (floor > ceil) return;
                _do_for_value_range(left, right + 1, floor, ceil, call, q, {});
            }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_value_range_bitxor(size_type left, size_type right, Tp xor_by, Tp floor, Tp ceil, Callback &&call, Querier &&q = Querier()) const {
                if ((xor_by >> m_alpha) < (floor >> m_alpha) || (xor_by >> m_alpha) > (ceil >> m_alpha)) return;
                Tp _floor = (xor_by >> m_alpha) == (floor >> m_alpha) ? (floor & _mask()) : 0;
                Tp _ceil = (xor_by >> m_alpha) == (ceil >> m_alpha) ? (ceil & _mask()) : _mask();
                _do_for_value_range(left, right + 1, _floor, _ceil, call, q, BitxorFilter<Tp>{xor_by});
            }
            template <typename Callback>
            void do_in_table(size_type pos, Callback &&call) {
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(pos), zr = m_ranks[d].rank0(pos + 1);
                    call(m_sumer[d], pos = zl == zr ? pos + m_pos[d] - zl : zl);
                }
            }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Tree {
            struct pair {
                Tp m_val;
                size_type m_index;
                bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                bool operator==(const pair &rhs) const { return m_val == rhs.m_val; }
                operator Tp() const { return m_val; }
            };
            Table<size_type, SumTable> m_table;
            std::vector<Tp> m_discretizer;
            size_type m_size;
            size_type _find(const Tp &val) const { return std::lower_bound(m_discretizer.begin(), m_discretizer.end(), val) - m_discretizer.begin(); }
            size_type _find2(const Tp &val) const { return std::upper_bound(m_discretizer.begin(), m_discretizer.end(), val) - m_discretizer.begin() - 1; }
            Tree() = default;
            template <typename InitMapping, typename Compare = std::less<Tp>>
            Tree(size_type length, InitMapping mapping, Compare &&comp = Compare()) { resize(length, mapping, {}, comp); }
            template <typename Iterator, typename Compare = std::less<Tp>>
            Tree(Iterator first, Iterator last, Compare &&comp = Compare()) { reset(first, last, {}, comp); }
            template <typename InitMapping, typename TableMapping = Ignore, typename Compare = std::less<Tp>>
            void resize(size_type length, InitMapping mapping, TableMapping table_mapping = TableMapping(), Compare &&comp = Compare()) {
                if (!(m_size = length)) return;
                std::vector<pair> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = {mapping(i), i};
                std::sort(items.begin(), items.end(), comp);
                std::vector<size_type> id(m_size);
                size_type cnt = 0;
                for (size_type i = 0; i != m_size; i++) {
                    if (i && comp(items[i - 1].m_val, items[i].m_val)) cnt++;
                    id[items[i].m_index] = cnt;
                }
                m_discretizer.clear(), m_discretizer.reserve(cnt + 1);
                std::unique_copy(items.begin(), items.end(), std::back_insert_iterator<std::vector<Tp>>(m_discretizer));
                if constexpr (std::is_same<TableMapping, Ignore>::value)
                    m_table.reset(id.begin(), id.end(), std::bit_width(cnt + 1), [&](size_type val) { return m_discretizer[val]; });
                else
                    m_table.reset(id.begin(), id.end(), std::bit_width(cnt + 1), [&](size_type val) { return table_mapping(m_discretizer[val]); });
            }
            template <typename InitMapping, typename TableMapping, typename Compare = std::less<Tp>>
            void resize_mapping_with_index(size_type length, InitMapping mapping, TableMapping table_mapping, Compare &&comp = Compare()) {
                if (!(m_size = length)) return;
                std::vector<pair> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = {mapping(i), i};
                std::sort(items.begin(), items.end(), comp);
                std::vector<size_type> id(m_size);
                size_type cnt = 0;
                for (size_type i = 0; i != m_size; i++) {
                    if (i && comp(items[i - 1].m_val, items[i].m_val)) cnt++;
                    id[items[i].m_index] = cnt;
                }
                m_discretizer.clear(), m_discretizer.reserve(cnt + 1);
                std::unique_copy(items.begin(), items.end(), std::back_insert_iterator<std::vector<Tp>>(m_discretizer));
                m_table.reset_mapping_with_index(id.begin(), id.end(), std::bit_width(cnt + 1), [&](size_type val, size_type i) { return table_mapping(m_discretizer[val], i); });
            }
            template <typename Iterator, typename TableMapping = Ignore, typename Compare = std::less<Tp>>
            void reset(Iterator first, Iterator last, TableMapping table_mapping = TableMapping(), Compare &&comp = Compare()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping, comp);
            }
            template <typename Iterator, typename TableMapping, typename Compare = std::less<Tp>>
            void reset_mapping_with_index(Iterator first, Iterator last, TableMapping table_mapping, Compare &&comp = Compare()) {
                resize_mapping_with_index(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping, comp);
            }
            size_type count(size_type left, size_type right, const Tp &val) const {
                size_type find = _find(val);
                return find < m_discretizer.size() && m_discretizer[find] == val ? m_table.count(left, right, find) : 0;
            }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                size_type find1 = _find(minimum), find2 = _find2(maximum);
                if (find1 == find2 + 1) return 0;
                return m_table.count(left, right, find1, find2);
            }
            bool any(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                size_type find1 = _find(minimum), find2 = _find2(maximum);
                return find1 != find2 + 1 && m_table.any(left, right, find1, find2);
            }
            size_type rank(size_type left, size_type right, const Tp &val) const { return m_table.rank(left, right, _find(val)); }
            Tp minimum(size_type left, size_type right) const { return m_discretizer[m_table.minimum(left, right)]; }
            Tp maximum(size_type left, size_type right) const { return m_discretizer[m_table.maximum(left, right)]; }
            Tp quantile(size_type left, size_type right, size_type k) const { return m_discretizer[m_table.quantile(left, right, k)]; }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_rank_range(size_type left, size_type right, size_type rk1, size_type rk2, Callback &&call, Querier &&q = Querier()) const { m_table.do_for_rank_range(left, right, rk1, rk2, call, q); }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_value_range(size_type left, size_type right, const Tp &floor, const Tp &ceil, Callback &&call, Querier &&q = Querier()) const {
                size_type find1 = _find(floor), find2 = _find2(ceil);
                if (find1 != find2 + 1) m_table.do_for_value_range(left, right, find1, find2, call, q);
            }
        };
    }
}

#endif