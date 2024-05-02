/*
最后修改:
20240502
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SORTFHQTREAP__
#define __OY_SORTFHQTREAP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace SortFHQ {
        using size_type = uint32_t;
        using priority_type = uint32_t;
        std::mt19937 fhq_rand;
        struct Ignore {};
        template <typename KeyType, typename InfoType, typename Compare, bool MaintainReverse>
        struct NodeWrapper {
            template <typename Node>
            struct type {
                using key_type = KeyType;
                key_type m_key;
                InfoType m_info, m_sum;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
                const InfoType &get_sum() const { return m_sum; }
                const InfoType &get_sum_rev() const { return m_sum; }
                void pushup(Node *lchild, Node *rchild) { m_sum = lchild->is_null() ? (rchild->is_null() ? m_info : m_info + rchild->m_sum) : (rchild->is_null() ? lchild->m_sum + m_info : lchild->m_sum + m_info + rchild->m_sum); }
            };
        };
        template <typename KeyType, typename InfoType, typename Compare>
        struct NodeWrapper<KeyType, InfoType, Compare, true> {
            template <typename Node>
            struct type {
                using key_type = KeyType;
                key_type m_key;
                InfoType m_info, m_sum, m_sum_rev;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
                const InfoType &get_sum() const { return m_sum; }
                const InfoType &get_sum_rev() const { return m_sum_rev; }
                void pushup(Node *lchild, Node *rchild) {
                    if (lchild->is_null())
                        if (rchild->is_null())
                            m_sum = m_sum_rev = m_info;
                        else
                            m_sum = m_info + rchild->m_sum, m_sum_rev = rchild->m_sum_rev + m_info;
                    else if (rchild->is_null())
                        m_sum = lchild->m_sum + m_info, m_sum_rev = m_info + lchild->m_sum_rev;
                    else
                        m_sum = lchild->m_sum + m_info + rchild->m_sum, m_sum_rev = rchild->m_sum_rev + m_info + lchild->m_sum_rev;
                }
            };
        };
        template <typename KeyType, typename Compare>
        struct NodeWrapper<KeyType, void, Compare, false> {
            template <typename Node>
            struct type {
                using key_type = KeyType;
                key_type m_key;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
            };
        };
        template <typename InfoType>
        struct InfoTable {
            size_type m_size, m_capacity;
            std::vector<InfoType> m_data;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping, const InfoType &default_info) {
                m_size = length;
                for (m_capacity = 1; m_capacity < length; m_capacity *= 2) {}
                m_data.resize(m_capacity * 2);
                for (size_type i = 0; i != length; i++) m_data[m_capacity + i] = mapping(i);
                for (size_type i = length; i != m_capacity; i++) m_data[m_capacity + i] = default_info;
                for (size_type l = m_capacity, r = m_capacity * 2; r /= 2, l /= 2;)
                    for (size_type i = l; i != r; i++) m_data[i] = m_data[i * 2] + m_data[i * 2 + 1];
            }
            void modify(size_type i, const InfoType &info) {
                m_data[i += m_capacity] = info;
                while (i /= 2) m_data[i] = m_data[i * 2] + m_data[i * 2 + 1];
            }
            const InfoType &query(size_type i) const { return m_data[m_capacity + i]; }
            InfoType query(size_type left, size_type right) const {
                if (left == right) return query(left);
                InfoType resl = m_data[left += m_capacity], resr = m_data[right += m_capacity];
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) resl = resl + m_data[left ^ 1];
                    if (right & 1) resr = m_data[right ^ 1] + resr;
                }
                return resl + resr;
            }
            InfoType query_all() const { return m_data[1]; }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                InfoType val = m_data[left += m_capacity];
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    InfoType a = val + m_data[left >>= ctz];
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; len >>= 1) {
                            InfoType a = val + m_data[left <<= 1];
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                InfoType val = m_data[right += m_capacity];
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    InfoType a = m_data[(right >>= ctz) - 1] + val;
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; len >>= 1) {
                            InfoType a = m_data[(right <<= 1) - 1] + val;
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
        };
        template <>
        struct InfoTable<void> {};
        enum MaintainType {
            MAINTAIN_NONE = 0,
            MAINTAIN_RANGE = 1,
            MAINTAIN_RANGE_REVERSE = 2
        };
        template <typename KeyType, typename InfoType, typename Compare, MaintainType Maintain = MAINTAIN_RANGE_REVERSE>
        struct Tree {
            struct node : NodeWrapper<KeyType, InfoType, Compare, Maintain == MAINTAIN_RANGE_REVERSE>::template type<node> {
                size_type m_lc, m_rc, m_sz;
                priority_type m_rd;
                bool is_null() const { return !m_rd; }
            };
            using info_type = typename std::conditional<std::is_void<InfoType>::value, Ignore, InfoType>::type;
            template <bool Reversed, bool LeftAdd>
            struct InfoGetter {
                static size_type lchild(const node *p) {
                    if constexpr (Reversed)
                        return p->m_rc;
                    else
                        return p->m_lc;
                }
                static size_type rchild(const node *p) {
                    if constexpr (Reversed)
                        return p->m_lc;
                    else
                        return p->m_rc;
                }
                static info_type add(const info_type &x, const info_type &y) {
                    if constexpr (LeftAdd)
                        return x + y;
                    else
                        return y + x;
                }
                static info_type get(const node *p) {
                    if constexpr (Reversed)
                        return p->get_sum_rev();
                    else
                        return p->get_sum();
                }
            };
            size_type m_length;
            info_type m_default_info;
            std::vector<bool> m_reversed;
            std::vector<size_type> m_trees, m_bit, m_prev, m_next;
            std::vector<node> m_nodes;
            InfoTable<InfoType> m_table;
            static size_type _lowbit(size_type x) { return x & -x; }
            void _update_size(size_type x) { m_nodes[x].m_sz = m_nodes[m_nodes[x].m_lc].m_sz + m_nodes[m_nodes[x].m_rc].m_sz + 1; }
            void _pushup(size_type x) {
                if constexpr (!std::is_void<InfoType>::value) m_nodes[x].pushup(&m_nodes[m_nodes[x].m_lc], &m_nodes[m_nodes[x].m_rc]);
            }
            void _split_by_rank(size_type rt, size_type *x, size_type *y, size_type k) {
                if (!k) return (void)(*x = 0, *y = rt);
                size_type lsz = m_nodes[m_nodes[rt].m_lc].m_sz;
                if (k <= lsz)
                    *y = rt, _split_by_rank(m_nodes[rt].m_lc, x, &m_nodes[rt].m_lc, k);
                else
                    *x = rt, _split_by_rank(m_nodes[rt].m_rc, &m_nodes[rt].m_rc, y, k - lsz - 1);
                _update_size(rt), _pushup(rt);
            }
            void _split_by_rank(size_type &tr1, size_type &tr2, size_type k) { _split_by_rank(tr1, &tr1, &tr2, k); }
            template <typename Less>
            void _split_by_key(size_type rt, size_type *x, size_type *y, Less &&less) {
                if (!rt) return (void)(*x = *y = 0);
                if (less(m_nodes[rt].get()))
                    *y = rt, _split_by_key(m_nodes[rt].m_lc, x, &m_nodes[rt].m_lc, less);
                else
                    *x = rt, _split_by_key(m_nodes[rt].m_rc, &m_nodes[rt].m_rc, y, less);
                _update_size(rt), _pushup(rt);
            }
            void _join(size_type *rt, size_type x, size_type y) {
                if (!x || !y) return (void)(*rt = x | y);
                if (m_nodes[x].m_rd > m_nodes[y].m_rd)
                    _join(&m_nodes[ *rt = x].m_rc, m_nodes[x].m_rc, y);
                else
                    _join(&m_nodes[ *rt = y].m_lc, x, m_nodes[y].m_lc);
                _update_size(*rt), _pushup(*rt);
            }
            void _merge(size_type *rt, size_type x, size_type y) {
                if (!x || !y) return (void)(*rt = x | y);
                if (m_nodes[x].m_rd < m_nodes[y].m_rd) std::swap(x, y);
                size_type a, b;
                KeyType x_key = m_nodes[x].get();
                _split_by_key(y, &a, &b, [&](const KeyType &key) { return x_key <= key; });
                _merge(&m_nodes[x].m_lc, m_nodes[x].m_lc, a), _merge(&m_nodes[x].m_rc, m_nodes[x].m_rc, b), _update_size(*rt = x), _pushup(x);
            }
            void _merge(size_type &rt1, size_type &rt2) { _merge(&rt1, rt1, rt2); }
            template <typename Getter, typename Judger>
            size_type _bisect(size_type rt, info_type &val, Judger &&judge) const {
                size_type sz = m_nodes[Getter::lchild(&m_nodes[rt])].m_sz;
                if (sz) {
                    auto old_val(val);
                    val = Getter::add(val, Getter::get(&m_nodes[Getter::lchild(&m_nodes[rt])]));
                    if (!judge(val)) return _bisect<Getter>(Getter::lchild(&m_nodes[rt]), old_val, judge);
                }
                val = m_nodes[rt].m_info + val;
                if (!judge(val)) return sz - 1;
                if (!Getter::rchild(&m_nodes[rt])) return sz;
                return sz + 1 + _bisect<Getter>(Getter::rchild(&m_nodes[rt]), val, judge);
            }
            template <typename Judger>
            size_type _max_right(size_type rt, info_type &val, Judger &&judge, bool rev) const {
                if (rev)
                    return _bisect<InfoGetter<true, true>>(rt, val, judge);
                else
                    return _bisect<InfoGetter<false, true>>(rt, val, judge);
            }
            template <typename Judger>
            size_type _min_left(size_type rt, info_type &val, Judger &&judge, bool rev) const {
                if (rev)
                    return m_nodes[rt].m_sz - 1 - _bisect<InfoGetter<false, false>>(rt, val, judge);
                else
                    return m_nodes[rt].m_sz - 1 - _bisect<InfoGetter<true, false>>(rt, val, judge);
            }
            const node *_kth(size_type rt, size_type k) const {
                size_type lsz = m_nodes[m_nodes[rt].m_lc].m_sz;
                if (k < lsz)
                    return _kth(m_nodes[rt].m_lc, k);
                else if (k > lsz)
                    return _kth(m_nodes[rt].m_rc, k - lsz - 1);
                else
                    return &m_nodes[rt];
            }
            const info_type &_tree_info(size_type rt) const { return m_nodes[rt].get_sum(); }
            const info_type &_tree_info(size_type rt, bool rev) const { return rev ? m_nodes[rt].get_sum_rev() : m_nodes[rt].get_sum(); }
            size_type _presum(size_type i) const {
                size_type res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_bit[j];
                return res;
            }
            void _insert(size_type i, size_type pre) {
                for (size_type j = i; j < m_length; j += _lowbit(j + 1)) m_bit[j]++;
            }
            void _remove(size_type i) {
                for (size_type j = i; j < m_length; j += _lowbit(j + 1)) m_bit[j]--;
            }
            void _split_to(size_type pre, size_type nxt, size_type tot) {
                if (m_reversed[pre])
                    m_trees[nxt] = m_trees[pre], _split_by_rank(m_trees[nxt], m_trees[pre], tot - nxt + pre), m_reversed[nxt] = true;
                else
                    _split_by_rank(m_trees[pre], m_trees[nxt], nxt - pre), m_reversed[nxt] = false;
                _insert(nxt, pre);
                size_type nxt0 = m_next[pre];
                m_prev[nxt0] = pre, m_next[nxt] = nxt0, m_prev[nxt0] = m_next[pre] = nxt;
                _update_call(pre), _update_call(nxt);
            }
            size_type _kth(size_type k) const {
                size_type cursor = -1;
                for (size_type d = m_length >> 1; d; d >>= 1)
                    if (m_bit[cursor + d] <= k) k -= m_bit[cursor += d];
                return cursor + 1;
            }
            void _update_call(size_type left) {
                if constexpr (Maintain != MAINTAIN_NONE)
                    if constexpr (Maintain == MAINTAIN_RANGE_REVERSE)
                        m_table.modify(left, _tree_info(m_trees[left], m_reversed[left]));
                    else
                        m_table.modify(left, _tree_info(m_trees[left]));
            }
            void _remove_call(size_type left) {
                if constexpr (Maintain != MAINTAIN_NONE) m_table.modify(left, m_default_info);
            }
            template <typename InitKeyMapping, typename InitMapping>
            void _init(size_type length, InitKeyMapping key_mapping, InitMapping mapping, const info_type &default_info) {
                m_default_info = default_info, m_nodes.resize(length + 1), m_reversed.resize(length), m_trees.resize(length);
                if constexpr (std::is_void<InfoType>::value)
                    for (size_type i = 0; i != length; i++) m_trees[i] = i + 1, m_nodes[i + 1].m_lc = m_nodes[i + 1].m_rc = 0, m_nodes[i + 1].m_sz = 1, m_nodes[i + 1].m_rd = fhq_rand(), m_nodes[i + 1].set(key_mapping(i)), _pushup(i + 1);
                else {
                    for (size_type i = 0; i != length; i++) m_trees[i] = i + 1, m_nodes[i + 1].m_lc = m_nodes[i + 1].m_rc = 0, m_nodes[i + 1].m_sz = 1, m_nodes[i + 1].m_rd = fhq_rand(), m_nodes[i + 1].set(key_mapping(i)), m_nodes[i + 1].m_info = mapping(i), _pushup(i + 1);
                    m_table.resize(
                        length, [&](size_type i) { return m_nodes[i + 1].m_info; }, default_info);
                }
                m_length = std::bit_ceil(length + 1), m_bit.resize(m_length), m_prev.resize(m_length), m_next.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_bit[i] = size_type(1) << std::countr_zero(~i);
                for (size_type i = 0; i != m_length; i++) m_prev[i] = i - 1;
                for (size_type i = 0; i != m_length; i++) m_next[i] = i + 1;
            }
            Tree() = default;
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping key_mapping) { _init(length, key_mapping, 0, {}); }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping key_mapping, InitMapping mapping, const info_type &default_info) { _init(length, key_mapping, mapping, default_info); }
            size_type size() const { return m_trees.size(); }
            void modify(size_type i, KeyType key, const info_type &info = {}) {
                size_type pre = _kth(_presum(i) - 1);
                if (pre != i) _split_to(pre, i, m_next[pre] - pre);
                if (m_next[i] > i + 1) _split_to(i, i + 1, m_next[i] - i);
                node *rt = &m_nodes[m_trees[i]];
                if constexpr (!std::is_void<InfoType>::value) rt->m_info = info;
                rt->set(key), _pushup(m_trees[i]), m_reversed[i] = false, _update_call(i);
            }
            template <bool Reversed>
            void sort(size_type left, size_type right) {
                size_type pre = _kth(_presum(left) - 1);
                if (m_reversed[pre] == Reversed && m_next[pre] > right) return;
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                if (m_next[left] >= right + 1) {
                    if (m_next[left] > right + 1) _split_to(left, right + 1, m_next[left] - left);
                    m_reversed[left] = Reversed, _update_call(left);
                    return;
                }
                for (size_type cur = left; (cur = m_next[cur]) <= right;) {
                    if (m_next[cur] > right + 1) _split_to(cur, right + 1, m_next[cur] - cur);
                    _merge(m_trees[left], m_trees[cur]), _remove(cur), _remove_call(cur);
                }
                m_next[left] = right + 1, m_prev[right + 1] = left, m_reversed[left] = Reversed, _update_call(left);
            }
            void sort_ascending(size_type left, size_type right) { return sort<false>(left, right); }
            void sort_descending(size_type left, size_type right) { return sort<true>(left, right); }
            const node *get_node(size_type i) const {
                size_type it = _kth(_presum(i) - 1);
                return m_reversed[it] ? _kth(m_trees[it], m_next[it] - i - 1) : _kth(m_trees[it], i - it);
            }
            info_type query(size_type i) const { return get_node(i)->m_info; }
            info_type query(size_type left, size_type right) {
                size_type pre = _kth(_presum(left) - 1);
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                if (m_next[left] >= right + 1) {
                    if (m_next[left] > right + 1) _split_to(left, right + 1, m_next[left] - left);
                } else {
                    pre = _kth(_presum(right + 1) - 1);
                    if (pre != right + 1) _split_to(pre, right + 1, m_next[pre] - pre);
                }
                return m_table.query(left, right);
            }
            info_type query_all() const { return m_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                size_type pre = _kth(_presum(left) - 1);
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                size_type res = m_table.max_right(left, judge);
                if (res == size() - 1) return res;
                info_type val = res == left - 1 ? m_default_info : m_table.query(left, res);
                return res + _max_right(m_trees[res + 1], val, judge, m_reversed[res + 1]) + 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                size_type pre = _kth(_presum(right) - 1);
                if (m_next[pre] != right + 1) _split_to(pre, right + 1, m_next[pre] - pre);
                size_type res = m_table.min_left(right, judge);
                if (!res) return res;
                info_type val = res == right + 1 ? m_default_info : m_table.query(res, right);
                return res + _min_left(m_trees[res - 1], val, judge, m_reversed[res - 1]) - 1;
            }
        };
        template <typename Ostream, typename KeyType, typename InfoType, typename Compare, MaintainType Maintain>
        Ostream &operator<<(Ostream &out, const Tree<KeyType, InfoType, Compare, Maintain> &x) {
            out << '{';
            using node = typename Tree<KeyType, InfoType, Compare, Maintain>::node;
            for (size_type i = 0, j = 0; i != x.size(); i = x.m_next[i]) {
                out << (i ? ", {" : "{");
                size_type cnt = x.m_nodes[x.m_trees[i]].m_sz;
                for (size_type k = 0; k != cnt; j++, k++) {
                    auto p = x.get_node(j);
                    if (k) out << ", ";
                    out << p->get();
                    if constexpr (!std::is_void<InfoType>::value) out << ':' << p->m_info;
                }
                out << '}';
            }
            return out << '}';
        }
    }
}

#endif