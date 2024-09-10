/*
最后修改:
20240513
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MERGESORTTREE__
#define __OY_MERGESORTTREE__

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace MS {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        struct Ignore {};
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        struct BitRank {
            std::vector<mask_type> m_bits;
            std::vector<size_type> m_sum;
            void resize(size_type length) { m_bits.assign((length >> MASK_WIDTH) + 1, 0), m_sum.resize(m_bits.size()); }
            void set(size_type i) { m_bits[i >> MASK_WIDTH] |= mask_type(1) << (i & (MASK_SIZE - 1)); }
            void set(size_type left, size_type right) {
                size_type l = left >> MASK_WIDTH, r = right >> MASK_WIDTH;
                if (l == r)
                    m_bits[l] |= (mask_type(1) << (right & (MASK_SIZE - 1))) - (mask_type(1) << (left & (MASK_SIZE - 1)));
                else {
                    m_bits[l] |= -(mask_type(1) << (left & (MASK_SIZE - 1)));
                    std::fill(m_bits.data() + l + 1, m_bits.data() + r, -1);
                    m_bits[r] |= (mask_type(1) << (right & (MASK_SIZE - 1))) - 1;
                }
            }
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
        template <typename Tp, typename TableMapping, typename SumTable, size_t MAX_LEVEL = 30>
        struct TableMappingProxy1 {
            static constexpr bool is_ignore = false;
            using value_type = decltype(std::declval<TableMapping>()(std::declval<Tp>()));
            TableMapping &m_table_mapping;
            std::vector<Tp> m_data;
            std::vector<value_type> m_buf[MAX_LEVEL];
            template <typename InitMapping>
            void init(size_type depth, size_type size, InitMapping mapping) {
                m_data.resize(size);
                for (size_type i = 0; i != size; i++) m_data[i] = mapping(i);
                for (size_type i = 0; i != depth; i++) m_buf[i].resize(size);
            }
            auto operator()(size_type index) const -> decltype(m_table_mapping(std::declval<Tp>())) { return m_table_mapping(m_data[index]); }
        };
        template <typename Tp, typename SumTable, size_t MAX_LEVEL>
        struct TableMappingProxy1<Tp, Ignore, SumTable, MAX_LEVEL> {
            static constexpr bool is_ignore = false;
            using value_type = Tp;
            Ignore &_;
            std::vector<Tp> m_data;
            std::vector<value_type> m_buf[MAX_LEVEL];
            template <typename InitMapping>
            void init(size_type depth, size_type size, InitMapping mapping) {
                m_data.resize(size);
                for (size_type i = 0; i != size; i++) m_data[i] = mapping(i);
                for (size_type i = 0; i != depth; i++) m_buf[i].resize(size);
            }
            const Tp &operator()(size_type index) const { return m_data[index]; }
        };
        template <typename Tp, size_t MAX_LEVEL>
        struct TableMappingProxy1<Tp, Ignore, VoidTable, MAX_LEVEL> {
            static constexpr bool is_ignore = true;
            Ignore m_table_mapping;
            template <typename InitMapping>
            void init(size_type depth, size_type size, InitMapping mapping) {}
        };
        template <typename Tp, typename TableMapping, size_t MAX_LEVEL = 30>
        struct TableMappingProxy2 {
            static constexpr bool is_ignore = false;
            using value_type = decltype(std::declval<TableMapping>()(std::declval<Tp>(), 0));
            TableMapping &m_table_mapping;
            std::vector<Tp> m_data;
            std::vector<value_type> m_buf[MAX_LEVEL];
            template <typename InitMapping>
            void init(size_type depth, size_type size, InitMapping mapping) {
                m_data.resize(size);
                for (size_type i = 0; i != size; i++) m_data[i] = mapping(i);
                for (size_type i = 0; i != depth; i++) m_buf[i].resize(size);
            }
            auto operator()(size_type index) const -> decltype(m_table_mapping(std::declval<Tp>(), index)) { return m_table_mapping(m_data[index], index); }
        };
        struct DefaultQuerier {
            template <typename Callback, typename Tr>
            void operator()(Callback &&call, Tr &&tr, size_type left, size_type right) const { call(tr.query(left, right)); }
            template <typename Callback, typename Tr>
            void operator()(Callback &&call, Tr &&tr, size_type pos) const { call(tr.query(pos)); }
        };
        template <typename Tp, typename Compare = std::less<Tp>, typename SumTable = VoidTable>
        struct Tree {
            std::vector<Tp> m_sorted;
            std::vector<BitRank> m_left;
            std::vector<SumTable> m_sumer;
            size_type m_size, m_depth;
            template <typename Proxy>
            void _dfs(size_type l, size_type r, size_type level, size_type *ids, size_type *buf, Proxy &&proxy) {
                using proxy_decay = typename std::decay<Proxy>::type;
                if (r == l + 1) {
                    if constexpr (!proxy_decay::is_ignore)
                        if (level != m_depth) proxy.m_buf[level][l] = proxy(ids[l]);
                    return;
                }
                size_type mid = (l + r) / 2;
                _dfs(l, mid, level + 1, ids, buf, proxy), _dfs(mid, r, level + 1, ids, buf, proxy);
                if constexpr (!proxy_decay::is_ignore)
                    for (size_type i = l; i != r; i++) proxy.m_buf[level][i] = proxy(ids[i]);
                size_type i1 = l, i2 = mid, i = 0;
                while (i1 != mid && i2 != r)
                    if (ids[i1] < ids[i2]) {
                        m_left[level].set(l + i);
                        buf[i++] = ids[i1++];
                    } else
                        buf[i++] = ids[i2++];
                m_left[level].set(l + i, l + i + (mid - i1));
                std::move_backward(ids + i1, ids + mid, ids + r), std::move(buf, buf + i, ids + l);
            }
            template <bool CountEqual>
            size_type _count(size_type left, size_type right, const Tp &x) const {
                size_type res{};
                right++;
                auto sorted = m_sorted.data();
                for (size_type i = 0, j = m_size, pos = 0; i != m_depth && j / 2 && left != right; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(left), one2 = m_left[i].rank1(right), one = one2 - one1;
                    if constexpr (CountEqual)
                        if (Compare()(x, sorted[pos + j / 2 - 1]))
                            left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                        else
                            res += one, left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2;
                    else if (!Compare()(sorted[pos + j / 2 - 1], x))
                        left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                    else
                        res += one, left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2;
                }
                if (left != right)
                    if constexpr (CountEqual)
                        res += !Compare()(x, sorted[left]);
                    else
                        res += Compare()(sorted[left], x);
                return res;
            }
            Tree() = default;
            template <typename InitMapping, typename TableMapping = Ignore>
            Tree(size_type length, InitMapping mapping, TableMapping table_mapping = TableMapping()) { resize(length, mapping, table_mapping); }
            template <typename Iterator, typename TableMapping = Ignore>
            Tree(Iterator first, Iterator last, TableMapping table_mapping = TableMapping()) { reset(first, last, table_mapping); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping mapping, TableMapping table_mapping = TableMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size - 1);
                m_left.resize(m_depth);
                for (size_type i = 0; i != m_depth; i++) m_left[i].resize(m_size);
                struct pair {
                    Tp m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                    operator size_type() const { return m_index; }
                };
                std::vector<pair> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = {mapping(i), i};
                TableMappingProxy1<Tp, TableMapping, SumTable> proxy{table_mapping};
                proxy.init(m_depth, m_size, [&](size_type i) { return items[i].m_val; });
                std::sort(items.begin(), items.end());
                m_sorted.resize(m_size);
                for (size_type i = 0; i != m_size; i++) m_sorted[i] = items[i].m_val;
                std::vector<size_type> ids(items.begin(), items.end()), buf_vec(m_size);
                _dfs(0, m_size, 0, ids.data(), buf_vec.data(), proxy);
                for (size_type i = 0; i != m_depth; i++) m_left[i].prepare();
                if constexpr (!TableMappingProxy1<Tp, TableMapping, SumTable>::is_ignore) {
                    m_sumer.resize(m_depth);
                    for (size_type i = 0; i != m_depth; i++) m_sumer[i].reset(proxy.m_buf[i].begin(), proxy.m_buf[i].end());
                }
            }
            template <typename InitMapping, typename TableMapping>
            void resize_mapping_with_index(size_type length, InitMapping mapping, TableMapping table_mapping) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size);
                m_left.resize(m_depth);
                for (size_type i = 0; i != m_depth; i++) m_left[i].resize(m_size);
                struct pair {
                    Tp m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                    operator size_type() const { return m_index; }
                };
                std::vector<pair> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = {mapping(i), i};
                TableMappingProxy2<Tp, TableMapping> proxy{table_mapping};
                proxy.init(m_depth, m_size, [&](size_type i) { return items[i].m_val; });
                std::sort(items.begin(), items.end());
                m_sorted.resize(m_size);
                for (size_type i = 0; i != m_size; i++) m_sorted[i] = items[i].m_val;
                std::vector<size_type> ids(items.begin(), items.end()), buf_vec(m_size);
                _dfs(0, m_size, 0, ids.data(), buf_vec.data(), proxy);
                for (size_type i = 0; i != m_depth; i++) m_left[i].prepare();
                m_sumer.resize(m_depth);
                for (size_type i = 0; i != m_depth; i++) m_sumer[i].reset(proxy.m_buf[i].begin(), proxy.m_buf[i].end());
            }
            template <typename Iterator, typename TableMapping = Ignore>
            void reset(Iterator first, Iterator last, TableMapping table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping);
            }
            template <typename Iterator, typename TableMapping>
            void reset_mapping_with_index(Iterator first, Iterator last, TableMapping table_mapping) {
                resize_mapping_with_index(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping);
            }
            Tp quantile(size_type left, size_type right, size_type k) const {
                right++;
                for (size_type i = 0, j = m_size, pos = 0; i != m_depth; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(left), one2 = m_left[i].rank1(right), one = one2 - one1;
                    if (k < one)
                        left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                    else
                        k -= one, left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2;
                }
                return m_sorted[left];
            }
            size_type rank(size_type left, size_type right, const Tp &x) const { return _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const Tp &x) const { return _count<true>(left, right, x) - _count<false>(left, right, x); }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const { return _count<true>(left, right, maximum) - _count<false>(left, right, minimum); }
            bool any(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                right++;
                auto sorted = m_sorted.data();
                size_type i = 0, j = m_size, pos = 0, l1, r1, l2, r2;
                for (; j / 2 && left != right; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(left), one2 = m_left[i].rank1(right);
                    if (!Compare()(sorted[pos + j / 2 - 1], maximum))
                        left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                    else if (Compare()(sorted[pos + j / 2 - 1], minimum))
                        left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2;
                    else {
                        l1 = pos + one1 - base, r1 = pos + one2 - base, l2 = left + j / 2 + base - one1, r2 = right + j / 2 + base - one2;
                        break;
                    }
                }
                if (j == 1 || left == right) return left != right && !Compare()(sorted[left], minimum) && !Compare()(maximum, sorted[left]);
                for (size_type pos1 = pos, j1 = j / 2, i1 = i + 1; j1 / 2 && l1 != r1; i1++) {
                    size_type base = m_left[i1].rank1(pos1), one1 = m_left[i1].rank1(l1), one2 = m_left[i1].rank1(r1);
                    if (!Compare()(sorted[pos1 + j1 / 2 - 1], minimum)) {
                        if (one2 - one1 != r1 - l1) return true;
                        l1 = pos1 + one1 - base, r1 = pos1 + one2 - base, j1 /= 2;
                    } else
                        l1 += j1 / 2 + base - one1, r1 += j1 / 2 + base - one2, pos1 += j1 / 2, j1 = (j1 + 1) / 2;
                }
                for (size_type pos2 = pos + j / 2, j2 = (j + 1) / 2, i2 = i + 1; j2 / 2 && l2 != r2; i2++) {
                    size_type base = m_left[i2].rank1(pos2), one1 = m_left[i2].rank1(l2), one2 = m_left[i2].rank1(r2);
                    if (Compare()(maximum, sorted[pos2 + j2 / 2 - 1]))
                        l2 = pos2 + one1 - base, r2 = pos2 + one2 - base, j2 /= 2;
                    else {
                        if (one2 != one1) return true;
                        l2 += j2 / 2 + base - one1, r2 += j2 / 2 + base - one2, pos2 += j2 / 2, j2 = (j2 + 1) / 2;
                    }
                }
                return (l1 != r1 && !Compare()(sorted[l1], minimum)) || (l2 != r2 && !Compare()(maximum, sorted[l2]));
            }
            Tp minimum(size_type left, size_type right) const { return quantile(left, right, 0); }
            Tp maximum(size_type left, size_type right) const { return quantile(left, right, right - left); }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_rank_range(size_type left, size_type right, size_type rk1, size_type rk2, Callback &&call, Querier &&q = Querier()) const {
                right++;
                auto handle = [&](size_type l1, size_type r1, size_type pos1, size_type j1, size_type l2, size_type r2, size_type pos2, size_type j2, size_type k1, size_type k2, size_type d) {
                    for (size_type i = d; i != m_depth && l1 != r1; i++) {
                        size_type base = m_left[i].rank1(pos1), one1 = m_left[i].rank1(l1), one2 = m_left[i].rank1(r1), z = r1 - l1 - one2 + one1;
                        if (k1 < z)
                            l1 += j1 / 2 + base - one1, r1 += j1 / 2 + base - one2, pos1 += j1 / 2, j1 = (j1 + 1) / 2;
                        else {
                            if (z) q(call, m_sumer[i], l1 + j1 / 2 + base - one1, r1 + j1 / 2 + base - one2 - 1);
                            l1 = pos1 + one1 - base, r1 = pos1 + one2 - base, j1 /= 2, k1 -= z;
                        }
                    }
                    for (size_type i = d; i != m_depth && l2 != r2; i++) {
                        size_type base = m_left[i].rank1(pos2), one1 = m_left[i].rank1(l2), one2 = m_left[i].rank1(r2), one = one2 - one1;
                        if (k2 < one)
                            l2 = pos2 + one1 - base, r2 = pos2 + one2 - base, j2 /= 2;
                        else {
                            if (one) q(call, m_sumer[i], pos2 + one1 - base, pos2 + one2 - base - 1);
                            l2 += j2 / 2 + base - one1, r2 += j2 / 2 + base - one2, pos2 += j2 / 2, j2 = (j2 + 1) / 2, k2 -= one;
                        }
                    }
                    if (k1) q(call, m_sumer[m_depth - 1], r1 - k1, r1 - 1);
                    if (k2) q(call, m_sumer[m_depth - 1], l2, l2 + k2 - 1);
                };
                for (size_type i = 0, j = m_size, pos = 0; i != m_depth && left != right; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(left), one2 = m_left[i].rank1(right), one = one2 - one1;
                    if (rk2 < one)
                        left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                    else if (rk1 >= one)
                        left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2, rk1 -= one, rk2 -= one;
                    else
                        return handle(pos + one1 - base, pos + one2 - base, pos, j / 2, left + j / 2 + base - one1, right + j / 2 + base - one2, pos + j / 2, (j + 1) / 2, one - rk1, rk2 - one + 1, i + 1);
                }
                if (left != right) q(call, m_sumer[m_depth - 1], left);
            }
            template <typename Callback, typename Querier = DefaultQuerier>
            void do_for_value_range(size_type left, size_type right, const Tp &minimum, const Tp &maximum, Callback &&call, Querier &&q = Querier()) const {
                right++;
                auto sorted = m_sorted.data();
                auto handle = [&](size_type l1, size_type r1, size_type pos1, size_type j1, size_type l2, size_type r2, size_type pos2, size_type j2, size_type d) {
                    for (size_type i = d; i != m_depth && l1 != r1; i++) {
                        size_type base = m_left[i].rank1(pos1), one1 = m_left[i].rank1(l1), one2 = m_left[i].rank1(r1), z = r1 - l1 - one2 + one1;
                        if (Compare()(sorted[pos1 + j1 / 2], minimum))
                            l1 += j1 / 2 + base - one1, r1 += j1 / 2 + base - one2, pos1 += j1 / 2, j1 = (j1 + 1) / 2;
                        else {
                            if (z) q(call, m_sumer[i], l1 + j1 / 2 + base - one1, r1 + j1 / 2 + base - one2 - 1);
                            l1 = pos1 + one1 - base, r1 = pos1 + one2 - base, j1 /= 2;
                        }
                    }
                    for (size_type i = d; i != m_depth && l2 != r2; i++) {
                        size_type base = m_left[i].rank1(pos2), one1 = m_left[i].rank1(l2), one2 = m_left[i].rank1(r2), one = one2 - one1;
                        if (Compare()(maximum, sorted[pos2 + j2 / 2]))
                            l2 = pos2 + one1 - base, r2 = pos2 + one2 - base, j2 /= 2;
                        else {
                            if (one) q(call, m_sumer[i], pos2 + one1 - base, pos2 + one2 - base - 1);
                            l2 += j2 / 2 + base - one1, r2 += j2 / 2 + base - one2, pos2 += j2 / 2, j2 = (j2 + 1) / 2;
                        }
                    }
                    if (l1 != r1 && !Compare()(sorted[l1], minimum)) q(call, m_sumer[m_depth - 1], l1);
                    if (l2 != r2 && !Compare()(maximum, sorted[l2])) q(call, m_sumer[m_depth - 1], l2);
                };
                for (size_type i = 0, j = m_size, pos = 0; i != m_depth && left != right; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(left), one2 = m_left[i].rank1(right), one = one2 - one1;
                    if (Compare()(maximum, sorted[pos + j / 2]))
                        left = pos + one1 - base, right = pos + one2 - base, j /= 2;
                    else if (Compare()(sorted[pos + j / 2], minimum))
                        left += j / 2 + base - one1, right += j / 2 + base - one2, pos += j / 2, j = (j + 1) / 2;
                    else
                        return handle(pos + one1 - base, pos + one2 - base, pos, j / 2, left + j / 2 + base - one1, right + j / 2 + base - one2, pos + j / 2, (j + 1) / 2, i + 1);
                }
                if (left != right && !Compare()(sorted[left], minimum) && !Compare()(maximum, sorted[left])) q(call, m_sumer[m_depth - 1], left);
            }
            template <typename Callback>
            void do_in_table(size_type index, Callback &&call) {
                for (size_type i = 0, j = m_size, pos = 0; i != m_depth; i++) {
                    size_type base = m_left[i].rank1(pos), one1 = m_left[i].rank1(index), one2 = m_left[i].rank1(index + 1);
                    if (one1 != one2)
                        index = pos + one1 - base, j /= 2;
                    else
                        index += j / 2 + base - one1, pos += j / 2, j = (j + 1) / 2;
                    call(m_sumer[i], index);
                }
            }
        };
    }
}

#endif