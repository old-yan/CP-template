/*
最后修改:
20240407
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SORTSEGTREE__
#define __OY_SORTSEGTREE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace SortSeg {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename InfoType, bool MaintainReverse>
        struct Item {
            InfoType m_info;
            size_type m_cnt;
            Item() = default;
            Item(const InfoType &info) : m_info(info), m_cnt(1) {}
            Item(const InfoType &info, size_type cnt) : m_info(info), m_cnt(cnt) {}
            Item<InfoType, MaintainReverse> operator+(const Item<InfoType, MaintainReverse> &rhs) const { return {m_info + rhs.m_info, m_cnt + rhs.m_cnt}; }
        };
        template <typename InfoType>
        struct Item<InfoType, true> {
            InfoType m_info, m_info_rev;
            size_type m_cnt;
            Item() = default;
            Item(const InfoType &info) : m_info(info), m_info_rev(info), m_cnt(1) {}
            Item(const InfoType &info, size_type cnt) : m_info(info), m_info_rev(info), m_cnt(cnt) {}
            Item(const InfoType &info, const InfoType &info_rev, size_type cnt) : m_info(info), m_info_rev(info_rev), m_cnt(cnt) {}
            Item<InfoType, true> operator+(const Item<InfoType, true> &rhs) const { return {m_info + rhs.m_info, rhs.m_info_rev + m_info_rev, m_cnt + rhs.m_cnt}; }
        };
        template <>
        struct Item<void, false> {
            size_type m_cnt;
            Item() = default;
            Item(Ignore) : m_cnt(1) {}
            Item(size_type cnt) : m_cnt(cnt) {}
            Item(Ignore, size_type cnt) : m_cnt(cnt) {}
            Item<void, false> operator+(const Item<void, false> &rhs) const { return {m_cnt + rhs.m_cnt}; }
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
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_gc[BUFFER], s_use_cnt, s_gc_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++; }
                static void collect(size_type x) { s_gc[s_gc_cnt++] = x; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc_cnt = 0;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static std::vector<size_type> s_gc;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                if (!s_gc.empty()) {
                    size_type res = s_gc.back();
                    s_gc.pop_back();
                    return res;
                }
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static void collect(size_type x) { s_gc.push_back(x); }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Node>
        std::vector<size_type> VectorBuffer<Node>::s_gc;
        template <typename KeyType, typename InfoType, MaintainType Maintain = MAINTAIN_RANGE_REVERSE, template <typename> typename BufferType = VectorBuffer>
        struct Tree {
            using info_type = typename std::conditional<std::is_void<InfoType>::value, Ignore, InfoType>::type;
            using item_type = Item<InfoType, Maintain == MAINTAIN_RANGE_REVERSE>;
            struct node {
                KeyType m_key;
                item_type m_val;
                size_type m_lc, m_rc;
                node *lchild() { return _ptr(m_lc); }
                node *rchild() { return _ptr(m_rc); }
                KeyType key() const { return m_key - 1; }
                const info_type &get() const { return m_val.m_info; }
                const info_type &get_rev() const {
                    if constexpr (Maintain == MAINTAIN_RANGE)
                        return m_val.m_info;
                    else
                        return m_val.m_info_rev;
                }
                size_type size() const { return m_val.m_cnt; }
            };
            using buffer_type = BufferType<node>;
            size_type m_length;
            KeyType m_max_key;
            info_type m_default_info;
            std::vector<bool> m_reversed;
            std::vector<size_type> m_trees, m_bit, m_prev, m_next;
            InfoTable<InfoType> m_table;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static size_type _lowbit(size_type x) { return x & -x; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _newnode(size_type lc, size_type rc) {
                size_type x = _newnode();
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                return x;
            }
            static void _recycle(size_type x) { buffer_type::collect(x); }
            static void _pushup(size_type x) {
                node *p = _ptr(x);
                p->m_val = p->lchild()->m_val + p->rchild()->m_val;
            }
            static size_type _init_single_key(KeyType key, const item_type &item) {
                size_type c = _newnode();
                _ptr(c)->m_key = key + 1, _ptr(c)->m_val = item;
                return c;
            }
            static size_type _modify_single_key(size_type rt, KeyType key, const item_type &item) {
                size_type x = rt;
                while (!_ptr(x)->m_key) {
                    size_type nxt = _ptr(x)->m_lc ^ _ptr(x)->m_rc;
                    _ptr(x)->m_lc = _ptr(x)->m_rc = 0, _recycle(x), x = nxt;
                }
                _ptr(x)->m_key = key + 1, _ptr(x)->m_val = item;
                return x;
            }
            static node *_kth(size_type rt, size_type k) {
                if (_ptr(rt)->m_key) return _ptr(rt);
                if (k < _ptr(rt)->lchild()->size())
                    return _kth(_ptr(rt)->m_lc, k);
                else
                    return _kth(_ptr(rt)->m_rc, k - _ptr(rt)->lchild()->size());
            }
            static void _split_by_rank(size_type &rt, size_type &rt2, size_type k) {
                rt2 = _newnode();
                if (k < _ptr(rt)->lchild()->size()) {
                    if (_ptr(rt)->m_lc) {
                        size_type i1 = _ptr(rt)->m_lc, i2 = _ptr(rt2)->m_lc;
                        _split_by_rank(i1, i2, k);
                        _ptr(rt)->m_lc = i1, _ptr(rt2)->m_lc = i2;
                    }
                    std::swap(_ptr(rt)->m_rc, _ptr(rt2)->m_rc);
                } else if (_ptr(rt)->m_rc) {
                    k -= _ptr(rt)->lchild()->size();
                    if (!k)
                        std::swap(_ptr(rt)->m_rc, _ptr(rt2)->m_rc);
                    else {
                        size_type i1 = _ptr(rt)->m_rc, i2 = _ptr(rt2)->m_rc;
                        _split_by_rank(i1, i2, k);
                        _ptr(rt)->m_rc = i1, _ptr(rt2)->m_rc = i2;
                    }
                }
                _pushup(rt), _pushup(rt2);
            }
            static void _merge(size_type &rt, size_type &rt2, KeyType floor, KeyType ceil) {
                if (!rt) return std::swap(rt, rt2);
                KeyType mid = (floor + ceil) / 2;
                if (_ptr(rt)->m_key && !_ptr(rt2)->m_key) std::swap(rt, rt2);
                if (_ptr(rt)->m_key) rt = _ptr(rt)->m_key - 1 <= mid ? _newnode(rt, 0) : _newnode(0, rt);
                if (_ptr(rt2)->m_key) {
                    if (_ptr(rt2)->m_key - 1 <= mid) {
                        size_type i = _ptr(rt)->m_lc;
                        _merge(i, rt2, floor, mid), _ptr(rt)->m_lc = i;
                    } else {
                        size_type i = _ptr(rt)->m_rc;
                        _merge(i, rt2, mid + 1, ceil), _ptr(rt)->m_rc = i;
                    }
                } else {
                    if (!_ptr(rt)->m_lc)
                        std::swap(_ptr(rt)->m_lc, _ptr(rt2)->m_lc);
                    else if (_ptr(rt2)->m_lc) {
                        size_type i1 = _ptr(rt)->m_lc, i2 = _ptr(rt2)->m_lc;
                        _merge(i1, i2, floor, mid), _ptr(rt)->m_lc = i1, _ptr(rt2)->m_lc = i2;
                    }
                    if (!_ptr(rt)->m_rc)
                        std::swap(_ptr(rt)->m_rc, _ptr(rt2)->m_rc);
                    else if (_ptr(rt2)->m_rc) {
                        size_type i1 = _ptr(rt)->m_rc, i2 = _ptr(rt2)->m_rc;
                        _merge(i1, i2, mid + 1, ceil), _ptr(rt)->m_rc = i1, _ptr(rt2)->m_rc = i2;
                    }
                    _recycle(rt2);
                }
                _pushup(rt), rt2 = 0;
            }
            template <typename Judger>
            static size_type _max_right(size_type rt, info_type &val, Judger &&judge) {
                node *p = _ptr(rt);
                auto a = val + p->get();
                if (judge(a))
                    return val = a, p->size();
                else if (p->m_key)
                    return 0;
                size_type res = p->m_lc ? _max_right(p->m_lc, val, judge) : 0;
                if (res != p->lchild()->size()) return res;
                if (p->m_rc) res += _max_right(p->m_rc, val, judge);
                return res;
            }
            template <typename Judger>
            static size_type _max_right_rev(size_type rt, info_type &val, Judger &&judge) {
                node *p = _ptr(rt);
                auto a = val + p->get_rev();
                if (judge(a))
                    return val = a, p->size();
                else if (p->m_key)
                    return 0;
                size_type res = p->m_rc ? _max_right_rev(p->m_rc, val, judge) : 0;
                if (res != p->rchild()->size()) return res;
                if (p->m_lc) res += _max_right_rev(p->m_lc, val, judge);
                return res;
            }
            template <typename Judger>
            static size_type _max_right(size_type rt, info_type &val, Judger &&judge, bool rev) { return rev ? _max_right_rev(rt, val, judge) : _max_right(rt, val, judge); }
            template <typename Judger>
            static size_type _min_left(size_type rt, info_type &val, Judger &&judge) {
                node *p = _ptr(rt);
                auto a = p->get() + val;
                if (judge(a))
                    return val = a, p->size();
                else if (p->m_key)
                    return 0;
                size_type res = p->m_rc ? _min_left(p->m_rc, val, judge) : 0;
                if (res != p->rchild()->size()) return res;
                if (p->m_lc) res += _min_left(p->m_lc, val, judge);
                return res;
            }
            template <typename Judger>
            static size_type _min_left_rev(size_type rt, info_type &val, Judger &&judge) {
                node *p = _ptr(rt);
                auto a = p->get_rev() + val;
                if (judge(a))
                    return val = a, p->size();
                else if (p->m_key)
                    return 0;
                size_type res = p->m_lc ? _min_left_rev(p->m_lc, val, judge) : 0;
                if (res != p->lchild()->size()) return res;
                if (p->m_rc) res += _min_left_rev(p->m_rc, val, judge);
                return res;
            }
            template <typename Judger>
            static size_type _min_left(size_type rt, info_type &val, Judger &&judge, bool rev) { return rev ? _min_left_rev(rt, val, judge) : _min_left(rt, val, judge); }
            static const info_type &_tree_info(size_type rt) { return _ptr(rt)->get(); }
            static const info_type &_tree_info(size_type rt, bool rev) { return rev ? _ptr(rt)->get_rev() : _ptr(rt)->get(); }
            template <bool Reversed, typename Callback>
            static void _do_for_each_leaf(size_type rt, Callback &&call) {
                node *p = _ptr(rt);
                if (p->m_key)
                    call(p->m_key - 1, p);
                else {
                    if constexpr (!Reversed)
                        if (p->m_lc) _do_for_each_leaf<Reversed>(p->m_lc, call);
                    if (p->m_rc) _do_for_each_leaf<Reversed>(p->m_rc, call);
                    if constexpr (Reversed)
                        if (p->m_lc) _do_for_each_leaf<Reversed>(p->m_lc, call);
                }
            }
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
                m_prev[nxt0] = pre, m_next[nxt] = nxt0, m_prev[nxt0] = m_next[pre] = nxt, _update_call(pre), _update_call(nxt);
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
            void _init(size_type length, InitKeyMapping key_mapping, InitMapping mapping, KeyType max_key, const info_type &default_info) {
                m_max_key = max_key, m_default_info = default_info;
                m_reversed.resize(length);
                m_trees.resize(length);
                _ptr(0)->m_val = item_type{default_info, 0};
                if constexpr (std::is_void<InfoType>::value)
                    for (size_type i = 0; i != length; i++) m_trees[i] = _init_single_key(key_mapping(i), item_type{1});
                else {
                    m_table.resize(length, mapping, default_info);
                    for (size_type i = 0; i != length; i++) m_trees[i] = _init_single_key(key_mapping(i), item_type{m_table.query(i), 1});
                }
                for (m_length = 1; m_length <= length; m_length <<= 1) {}
                m_bit.resize(m_length), m_prev.resize(m_length), m_next.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_bit[i] = size_type(1) << std::countr_zero(~i);
                for (size_type i = 0; i != m_length; i++) m_prev[i] = i - 1;
                for (size_type i = 0; i != m_length; i++) m_next[i] = i + 1;
            }
            Tree() = default;
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping key_mapping) {
                std::vector<KeyType> keys(length);
                for (size_type i = 0; i != length; i++) keys[i] = key_mapping(i);
                _init(
                    length, [&](size_type i) { return keys[i]; }, 0, *std::max_element(keys.begin(), keys.end()), {});
            }
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping key_mapping, KeyType max_key) { _init(length, key_mapping, 0, max_key, {}); }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping key_mapping, InitMapping mapping, const info_type &default_info) {
                std::vector<KeyType> keys(length);
                for (size_type i = 0; i != length; i++) keys[i] = key_mapping(i);
                _init(
                    length, [&](size_type i) { return keys[i]; }, mapping, *std::max_element(keys.begin(), keys.end()), default_info);
            }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping key_mapping, InitMapping mapping, KeyType max_key, const info_type &default_info) { _init(length, key_mapping, mapping, max_key, default_info); }
            size_type size() const { return m_trees.size(); }
            void modify(size_type i, KeyType key, const info_type &info = {}) {
                size_type pre = _kth(_presum(i) - 1);
                if (pre != i) _split_to(pre, i, m_next[pre] - pre);
                if (m_next[i] > i + 1) _split_to(i, i + 1, m_next[i] - i);
                m_reversed[i] = false, m_trees[i] = _modify_single_key(m_trees[i], key, item_type{info}), _update_call(i);
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
                    _merge(m_trees[left], m_trees[cur], 0, m_max_key), _remove(cur), _remove_call(cur);
                }
                m_next[left] = right + 1, m_prev[right + 1] = left, m_reversed[left] = Reversed, _update_call(left);
            }
            void sort_ascending(size_type left, size_type right) { return sort<false>(left, right); }
            void sort_descending(size_type left, size_type right) { return sort<true>(left, right); }
            const node *get_node(size_type i) const {
                size_type it = _kth(_presum(i) - 1);
                return m_reversed[it] ? _kth(m_trees[it], m_next[it] - i - 1) : _kth(m_trees[it], i - it);
            }
            InfoType query(size_type i) const { return get_node(i)->get(); }
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
        template <typename Ostream, typename KeyType, typename InfoType, MaintainType Maintain, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<KeyType, InfoType, Maintain, BufferType> &x) {
            out << '{';
            using node = typename Tree<KeyType, InfoType, Maintain, BufferType>::node;
            for (size_type i = 0; i != x.size(); i = x.m_next[i]) {
                out << (i ? ", {" : "{");
                auto call = [j = 0, &out](KeyType key, node *p) mutable {
                    if (j++) out << ", ";
                    out << key;
                    if constexpr (!std::is_void<InfoType>::value) out << ':' << p->get();
                };
                if (x.m_reversed[i])
                    x.template _do_for_each_leaf<true>(x.m_trees[i], call);
                else
                    x.template _do_for_each_leaf<false>(x.m_trees[i], call);
                out << '}';
            }
            return out << '}';
        }
    }
    template <typename KeyType, typename InfoType, SortSeg::MaintainType Maintain = SortSeg::MAINTAIN_RANGE_REVERSE, SortSeg::size_type BUFFER = 1 << 22>
    using StaticSortSeg = SortSeg::Tree<KeyType, InfoType, Maintain, SortSeg::StaticBufferWrap<BUFFER>::template type>;
    template <typename KeyType, typename InfoType, SortSeg::MaintainType Maintain = SortSeg::MAINTAIN_RANGE_REVERSE>
    using VectorSortSeg = SortSeg::Tree<KeyType, InfoType, Maintain, SortSeg::VectorBuffer>;
}

#endif