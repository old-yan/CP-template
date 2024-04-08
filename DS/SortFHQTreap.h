/*
最后修改:
20240408
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
#include <vector>

#include "../TEST/std_bit.h"
#include "FHQTreap.h"

namespace OY {
    namespace SortFHQ {
        using size_type = uint32_t;
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
            void resize(size_type length, InitMapping &&mapping, const InfoType &default_info) {
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
        template <typename KeyType, typename InfoType, typename Compare, MaintainType Maintain = MAINTAIN_RANGE_REVERSE, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using fhqtreap = FHQ::Multiset<NodeWrapper<KeyType, InfoType, Compare, Maintain == MAINTAIN_RANGE_REVERSE>::template type, MAX_NODE>;
            using node = typename fhqtreap::node;
            using info_type = typename std::conditional<std::is_void<InfoType>::value, Ignore, InfoType>::type;
            template <typename Judger, bool Reversed, bool LeftAdd>
            struct SubJudger {
                info_type &val;
                Judger &judge;
                static info_type add(const info_type &x, const info_type &y) {
                    if constexpr (LeftAdd)
                        return x + y;
                    else
                        return y + x;
                }
                static info_type get(node *p) {
                    if constexpr (Reversed)
                        return p->get_sum_rev();
                    else
                        return p->get_sum();
                }
                bool try_lchild(node *x) {
                    if (!x->m_lchild) return true;
                    info_type a = add(val, get(x->lchild()));
                    if (!judge(a)) return false;
                    return val = a, true;
                }
                bool try_rchild(node *x) {
                    info_type a = add(val, get(x->rchild()));
                    if (!judge(a)) return false;
                    return val = a, true;
                }
                bool try_mid(node *x) {
                    info_type a = add(val, x->m_info);
                    if (!judge(a)) return false;
                    return val = a, true;
                }
            };
            size_type m_length;
            info_type m_default_info;
            std::vector<bool> m_reversed;
            std::vector<fhqtreap> m_trees;
            std::vector<size_type> m_bit, m_prev, m_next;
            InfoTable<InfoType> m_table;
            static size_type _lowbit(size_type x) { return x & -x; }
            static void _split_by_rank(fhqtreap &tr1, fhqtreap &tr2, size_type k) { tr2 = tr1.split_by_rank(k); }
            static void _merge(fhqtreap &tr1, fhqtreap &tr2) { tr1.merge(tr2); }
            template <typename Judger>
            static size_type _max_right(fhqtreap &tr, info_type &val, Judger &&judge) { return tr.max_right(0, SubJudger<Judger, false, true>{val, judge}) + 1; }
            template <typename Judger>
            static size_type _max_right_rev(fhqtreap &tr, info_type &val, Judger &&judge) { return tr.size() - tr.min_left(tr.size() - 1, SubJudger<Judger, true, true>{val, judge}); }
            template <typename Judger>
            static size_type _max_right(fhqtreap &tr, info_type &val, Judger &&judge, bool rev) { return rev ? _max_right_rev(tr, val, judge) : _max_right(tr, val, judge); }
            template <typename Judger>
            static size_type _min_left(fhqtreap &tr, info_type &val, Judger &&judge) { return tr.size() - tr.min_left(tr.size() - 1, SubJudger<Judger, false, false>{val, judge}); }
            template <typename Judger>
            static size_type _min_left_rev(fhqtreap &tr, info_type &val, Judger &&judge) { return tr.max_right(0, SubJudger<Judger, true, false>{val, judge}) + 1; }
            template <typename Judger>
            static size_type _min_left(fhqtreap &tr, info_type &val, Judger &&judge, bool rev) { return rev ? _min_left_rev(tr, val, judge) : _min_left(tr, val, judge); }
            static const info_type &_tree_info(const fhqtreap &tr) { return tr.root()->get_sum(); }
            static const info_type &_tree_info(const fhqtreap &tr, bool rev) { return rev ? tr.root()->get_sum_rev() : tr.root()->get_sum(); }
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
            void _init(size_type length, InitKeyMapping &&key_mapping, InitMapping &&mapping, const info_type &default_info) {
                m_default_info = default_info;
                m_reversed.resize(length);
                m_trees.resize(length);
                if constexpr (std::is_void<InfoType>::value)
                    for (size_type i = 0; i != length; i++) m_trees[i].insert_by_key(key_mapping(i));
                else {
                    m_table.resize(length, mapping, default_info);
                    for (size_type i = 0; i != length; i++)
                        m_trees[i].insert_by_key(key_mapping(i), [&](node *p) { p->m_info = mapping(i); });
                }
                for (m_length = 1; m_length <= length; m_length <<= 1) {}
                m_bit.resize(m_length), m_prev.resize(m_length), m_next.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_bit[i] = size_type(1) << std::countr_zero(~i);
                for (size_type i = 0; i != m_length; i++) m_prev[i] = i - 1;
                for (size_type i = 0; i != m_length; i++) m_next[i] = i + 1;
            }
            Tree() = default;
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping &&key_mapping) { _init(length, key_mapping, 0, {}); }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping &&key_mapping, InitMapping &&mapping, const info_type &default_info) { _init(length, key_mapping, mapping, default_info); }
            size_type size() const { return m_trees.size(); }
            void modify(size_type i, KeyType key, const info_type &info = {}) {
                size_type pre = _kth(_presum(i) - 1);
                if (pre != i) _split_to(pre, i, m_next[pre] - pre);
                if (m_next[i] > i + 1) _split_to(i, i + 1, m_next[i] - i);
                node *rt = m_trees[i].root();
                if constexpr (!std::is_void<InfoType>::value) rt->m_info = info;
                rt->set(key), rt->pushup(rt->lchild(), rt->rchild()), m_reversed[i] = false, _update_call(i);
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
            const node *get_node(size_type i) const {
                size_type it = _kth(_presum(i) - 1);
                return m_reversed[it] ? m_trees[it].kth(m_next[it] - i - 1) : m_trees[it].kth(i - it);
            }
            InfoType query(size_type i) const { return get_node(i)->m_info; }
            InfoType query(size_type left, size_type right) {
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
            InfoType query_all() const { return m_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                size_type pre = _kth(_presum(left) - 1);
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                size_type res = m_table.max_right(left, judge);
                if (res == size() - 1) return res;
                info_type val = res == left - 1 ? m_default_info : m_table.query(left, res);
                return res + _max_right(m_trees[res + 1], val, judge, m_reversed[res + 1]);
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                size_type pre = _kth(_presum(right) - 1);
                if (m_next[pre] != right + 1) _split_to(pre, right + 1, m_next[pre] - pre);
                size_type res = m_table.min_left(right, judge);
                if (!res) return res;
                info_type val = res == right + 1 ? m_default_info : m_table.query(res, right);
                return m_next[res - 1] - _min_left(m_trees[res - 1], val, judge, m_reversed[res - 1]);
            }
        };
        template <typename Ostream, typename KeyType, typename InfoType, typename Compare, MaintainType Maintain, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<KeyType, InfoType, Compare, Maintain, MAX_NODE> &x) {
            out << '{';
            using node = typename Tree<KeyType, InfoType, Compare, Maintain, MAX_NODE>::node;
            for (size_type i = 0; i != x.size(); i = x.m_next[i]) {
                out << (i ? ", {" : "{");
                auto call = [j = 0, &out](node *p) mutable {
                    if (j++) out << ", ";
                    out << p->get();
                    if constexpr (!std::is_void<InfoType>::value) out << ':' << p->m_info;
                };
                if (x.m_reversed[i])
                    for (size_type j = x.m_trees[i].size(); ~--j;) call(x.m_trees[i].kth(j));
                else
                    x.m_trees[i].do_for_each(call);
                out << '}';
            }
            return out << '}';
        }
    }
}

#endif