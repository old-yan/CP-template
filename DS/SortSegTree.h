/*
最后修改:
20240920
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SORTSEGTREE__
#define __OY_SORTSEGTREE__

#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace SORTSEG {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        struct VoidInfo {};
        struct VoidMonoid {
            using value_type = void;
        };
        struct Self {
            template <typename Tp>
            Tp operator()(const Tp &x) const { return x; }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Reversed>
        struct BaseMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
            static value_type reversed(const value_type &x) { return Reversed()(x); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename Monoid, typename Tp, bool Rev>
        struct Item {
            Tp m_info, m_info_rev;
            size_type m_cnt;
            Item() = default;
            Item(const Tp &info) : m_info(info), m_info_rev(info), m_cnt(1) {}
            Item(const Tp &info, size_type cnt) : m_info(info), m_info_rev(info), m_cnt(cnt) {}
            Item(const Tp &info, const Tp &info_rev, size_type cnt) : m_info(info), m_info_rev(info_rev), m_cnt(cnt) {}
            Item<Monoid, Tp, true> operator+(const Item<Monoid, Tp, true> &rhs) const { return {Monoid::op(m_info, rhs.m_info), Monoid::op(rhs.m_info_rev, m_info_rev), m_cnt + rhs.m_cnt}; }
        };
        template <typename Monoid, typename Tp>
        struct Item<Monoid, Tp, false> {
            Tp m_info;
            size_type m_cnt;
            Item() = default;
            Item(const Tp &info) : m_info(info), m_cnt(1) {}
            Item(const Tp &info, size_type cnt) : m_info(info), m_cnt(cnt) {}
            Item<Monoid, Tp, false> operator+(const Item<Monoid, Tp, false> &rhs) const { return {Monoid::op(m_info, rhs.m_info), m_cnt + rhs.m_cnt}; }
        };
        template <typename Monoid>
        struct Item<Monoid, void, false> {
            size_type m_cnt;
            Item() = default;
            Item(VoidInfo) : m_cnt(1) {}
            Item(size_type cnt) : m_cnt(cnt) {}
            Item(VoidInfo, size_type cnt) : m_cnt(cnt) {}
            Item<Monoid, void, false> operator+(const Item<Monoid, void, false> &rhs) const { return {m_cnt + rhs.m_cnt}; }
        };
        template <typename Monoid, typename Tp>
        struct InfoTable {
            size_type m_size, m_cap;
            std::vector<Tp> m_data;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_cap = std::bit_ceil(m_size = length);
                m_data.resize(m_cap * 2);
                for (size_type i = 0; i != m_size; i++) m_data[m_cap + i] = mapping(i);
                std::fill(m_data.data() + m_cap + m_size, m_data.data() + m_cap * 2, Monoid::identity());
                for (size_type l = m_cap, r = m_cap * 2; r /= 2, l /= 2;)
                    for (size_type i = l; i != r; i++) m_data[i] = Monoid::op(m_data[i * 2], m_data[i * 2 + 1]);
            }
            void modify(size_type i, const Tp &info) {
                m_data[i += m_cap] = info;
                while (i /= 2) m_data[i] = Monoid::op(m_data[i * 2], m_data[i * 2 + 1]);
            }
            const Tp &query(size_type i) const { return m_data[m_cap + i]; }
            Tp query(size_type left, size_type right) const {
                if (left == right) return query(left);
                auto sub = m_data.data();
                Tp res = Monoid::identity();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res = Monoid::op(res, sub[(m_cap + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res = Monoid::op(res, sub[(m_cap + left) >> (j - 1)]);
                    left = left2;
                }
                return res;
            }
            Tp query_all() const { return m_data[1]; }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                Tp val = m_data[left += m_cap];
                if (!judge(val)) return left - m_cap - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    Tp a = Monoid::op(val, m_data[left >>= ctz]);
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_cap; len >>= 1) {
                            Tp a = Monoid::op(val, m_data[left <<= 1]);
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_cap, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                Tp val = m_data[right += m_cap];
                if (!judge(val)) return right - m_cap + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_cap);
                    Tp a = Monoid::op(m_data[(right >>= ctz) - 1], val);
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_cap; len >>= 1) {
                            Tp a = Monoid::op(m_data[(right <<= 1) - 1], val);
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_cap;
                    }
                }
                return 0;
            }
        };
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Reversed : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Reversed<Tp, ValueType, void_t<decltype(Tp::reversed(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename KeyType, typename Monoid, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using group = Monoid;
            using value_type = typename group::value_type;
            using info_type = typename std::conditional<std::is_void<value_type>::value, VoidInfo, value_type>::type;
            static constexpr bool has_op = !std::is_void<value_type>::value && Has_Op<group, info_type>::value, has_reversed = has_op && Has_Reversed<group, info_type>::value;
            using item_type = Item<group, value_type, has_op && !has_reversed>;
            struct node {
                KeyType m_key;
                item_type m_val;
                size_type m_lc, m_rc;
                node *lchild() { return _ptr(m_lc); }
                node *rchild() { return _ptr(m_rc); }
                KeyType key() const { return m_key - 1; }
                info_type get() const { return m_val.m_info; }
                info_type get_rev() const {
                    if constexpr (has_reversed)
                        return group::reversed(m_val.m_info);
                    else
                        return m_val.m_info_rev;
                }
                size_type size() const { return m_val.m_cnt; }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
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
            size_type m_length;
            KeyType m_max_key;
            std::vector<bool> m_reversed;
            std::vector<size_type> m_trees, m_bit, m_prev, m_next;
            InfoTable<group, info_type> m_table;
        private:
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _newnode(size_type lc, size_type rc) {
                size_type x = _newnode();
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                return x;
            }
            static void _collect(size_type x) { *_ptr(x) = {}, buffer_type::collect(x); }
            static void _collect_all(size_type x) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(x);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(x);
                }
            }
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
                    _ptr(x)->m_lc = _ptr(x)->m_rc = 0, _collect(x), x = nxt;
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
                if (_ptr(rt2)->m_key)
                    if (_ptr(rt2)->m_key - 1 <= mid) {
                        size_type i = _ptr(rt)->m_lc;
                        _merge(i, rt2, floor, mid), _ptr(rt)->m_lc = i;
                    } else {
                        size_type i = _ptr(rt)->m_rc;
                        _merge(i, rt2, mid + 1, ceil), _ptr(rt)->m_rc = i;
                    }
                else {
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
                    _collect(rt2);
                }
                _pushup(rt), rt2 = 0;
            }
            template <typename Judger>
            static size_type _max_right(size_type rt, info_type &val, Judger &&judge) {
                node *p = _ptr(rt);
                auto a = group::op(val, p->get());
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
                auto a = group::op(val, p->get_rev());
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
                auto a = group::op(p->get(), val);
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
                auto a = group::op(p->get_rev(), val);
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
            static info_type _tree_info(size_type rt, bool rev) { return rev ? _ptr(rt)->get_rev() : _ptr(rt)->get(); }
            size_type _presum(size_type i) const {
                size_type res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += m_bit[j];
                return res;
            }
            void _insert(size_type i, size_type pre) {
                for (size_type j = i; j < m_length; j += lowbit(j + 1)) m_bit[j]++;
            }
            void _remove(size_type i) {
                for (size_type j = i; j < m_length; j += lowbit(j + 1)) m_bit[j]--;
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
                if constexpr (has_op) m_table.modify(left, _tree_info(m_trees[left], m_reversed[left]));
            }
            void _remove_call(size_type left) {
                if constexpr (has_op) m_table.modify(left, group::identity());
            }
            template <typename InitKeyMapping, typename InitMapping>
            void _init(size_type length, InitKeyMapping key_mapping, InitMapping mapping, KeyType max_key) {
                m_max_key = max_key;
                m_reversed.resize(length);
                m_trees.resize(length);
                if constexpr (std::is_void<value_type>::value) {
                    _ptr(0)->m_val = item_type{{}, 0};
                    for (size_type i = 0; i != length; i++) m_trees[i] = _init_single_key(key_mapping(i), item_type{1});
                } else if constexpr (!has_op)
                    for (size_type i = 0; i != length; i++) m_trees[i] = _init_single_key(key_mapping(i), item_type{mapping(i), 1});
                else {
                    _ptr(0)->m_val = item_type{group::identity(), 0};
                    m_table.resize(length, mapping);
                    for (size_type i = 0; i != length; i++) m_trees[i] = _init_single_key(key_mapping(i), item_type{m_table.query(i), 1});
                }
                m_length = std::bit_ceil(length + 1);
                m_bit.resize(m_length), m_prev.resize(m_length), m_next.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_bit[i] = size_type(1) << std::countr_zero(~i);
                for (size_type i = 0; i != m_length; i++) m_prev[i] = i - 1;
                for (size_type i = 0; i != m_length; i++) m_next[i] = i + 1;
            }
        public:
            Tree() = default;
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping key_mapping, KeyType max_key) { _init(length, key_mapping, 0, max_key); }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping key_mapping, InitMapping mapping, KeyType max_key) { _init(length, key_mapping, mapping, max_key); }
            ~Tree() { clear(); }
            void clear() {
                for (size_type i = 0; i != size(); i = m_next[i]) _collect_all(m_trees[i]);
                m_length = 0, m_trees.clear();
            }
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
            value_type query(size_type i) const { return get_node(i)->get(); }
            value_type query(size_type left, size_type right) {
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
            value_type query_all() const { return m_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                size_type pre = _kth(_presum(left) - 1);
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                size_type res = m_table.max_right(left, judge);
                if (res == size() - 1) return res;
                value_type val = res == left - 1 ? group::identity() : m_table.query(left, res);
                return res + _max_right(m_trees[res + 1], val, judge, m_reversed[res + 1]);
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                size_type pre = _kth(_presum(right) - 1);
                if (m_next[pre] != right + 1) _split_to(pre, right + 1, m_next[pre] - pre);
                size_type res = m_table.min_left(right, judge);
                if (!res) return res;
                value_type val = res == right + 1 ? group::identity() : m_table.query(res, right);
                return m_next[res - 1] - _min_left(m_trees[res - 1], val, judge, m_reversed[res - 1]);
            }
        };
        template <typename Ostream, typename KeyType, typename Monoid, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<KeyType, Monoid, BufferType> &x) {
            out << '{';
            using value_type = typename Tree<KeyType, Monoid, BufferType>::value_type;
            using node = typename Tree<KeyType, Monoid, BufferType>::node;
            for (size_type i = 0; i != x.size(); i = x.m_next[i]) {
                out << (i ? ", {" : "{");
                auto call = [j = 0, &out](KeyType key, node *p) mutable {
                    if (j++) out << ", ";
                    out << key;
                    if constexpr (!std::is_void<value_type>::value) out << ':' << p->get();
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
    template <typename KeyType, typename Tp, Tp Identity, typename Operation, typename KeyMapping, typename InitMapping, typename TreeType = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, Identity, Operation, SORTSEG::Self>>>
    auto make_SortSeg(SORTSEG::size_type length, Operation op, KeyMapping key_mapping, InitMapping mapping, KeyType max_key) -> TreeType { return TreeType(length, key_mapping, mapping, max_key); }
    template <typename KeyType, template <typename> typename BufferType = VectorBufferWithCollect>
    using SortSegTree = SORTSEG::Tree<KeyType, SORTSEG::VoidMonoid, BufferType>;
    template <typename KeyType, typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using VectorSortMaxTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, Minimum, SORTSEG::ChoiceByCompare<Tp, std::less<Tp>>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using VectorSortMinTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, Maximum, SORTSEG::ChoiceByCompare<Tp, std::greater<Tp>>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp>
    using VectorSortGcdTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, 0, SORTSEG::FpTransfer<Tp, std::gcd<Tp>>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp>
    using VectorSortLcmTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, 1, SORTSEG::FpTransfer<Tp, std::lcm<Tp>>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp, Tp OneMask = Tp(-1)>
    using VectorSortBitAndTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, OneMask, std::bit_and<Tp>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp, Tp ZeroMask = 0>
    using VectorSortBitOrTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, ZeroMask, std::bit_or<Tp>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp, Tp ZeroMask = 0>
    using VectorSortBitXorTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, ZeroMask, std::bit_xor<Tp>, SORTSEG::Self>>;
    template <typename KeyType, typename Tp, Tp Zero = Tp()>
    using VectorSortSumTree = SORTSEG::Tree<KeyType, SORTSEG::BaseMonoid<Tp, Zero, std::plus<Tp>, SORTSEG::Self>>;
}

#endif