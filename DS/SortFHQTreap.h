/*
最后修改:
20240908
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

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace SORTFHQ {
        using size_type = uint32_t;
        using priority_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        std::mt19937 fhq_rand;
        struct VoidInfo {};
        struct VoidMonoid {
            using value_type = void;
        };
        struct Self {
            template <typename Tp>
            Tp operator()(const Tp &x) const { return x; }
        };
        template <typename ValueType, typename SumType, ValueType Identity, typename Operation, typename Reversed>
        struct BaseMonoid {
            using value_type = ValueType;
            using sum_type = SumType;
            static constexpr value_type identity() { return Identity; }
            static SumType op(const value_type &x, const value_type &y) { return Operation()(x, y); }
            static SumType reversed(const SumType &x) { return Reversed()(x); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename KeyType, typename Monoid, typename value_type, typename sum_type, bool Rev>
        struct NodeWrapper {
            template <typename Node>
            struct type {
                KeyType m_key;
                value_type m_val;
                sum_type m_sum, m_sum_rev;
                const sum_type &get_sum() const { return m_sum; }
                const sum_type &get_sum_rev() const { return m_sum_rev; }
                void pushup(Node *lchild, Node *rchild) { m_sum = Monoid::op(Monoid::op(lchild->m_sum, m_val), rchild->m_sum), m_sum_rev = Monoid::op(Monoid::op(rchild->m_sum_rev, m_val), lchild->m_sum_rev); }
            };
        };
        template <typename KeyType, typename Monoid, typename value_type, typename sum_type>
        struct NodeWrapper<KeyType, Monoid, value_type, sum_type, false> {
            template <typename Node>
            struct type {
                KeyType m_key;
                value_type m_val;
                sum_type m_sum;
                const sum_type &get_sum() const { return m_sum; }
                sum_type get_sum_rev() const { return Monoid::reversed(m_sum); }
                void pushup(Node *lchild, Node *rchild) { m_sum = Monoid::op(Monoid::op(lchild->m_sum, m_val), rchild->m_sum); }
            };
        };
        template <typename KeyType, typename Monoid, typename value_type>
        struct NodeWrapper<KeyType, Monoid, value_type, void, false> {
            template <typename Node>
            struct type {
                KeyType m_key;
                value_type m_val;
            };
        };
        template <typename KeyType, typename Monoid>
        struct NodeWrapper<KeyType, Monoid, void, void, false> {
            template <typename Node>
            struct type {
                KeyType m_key;
            };
        };
        template <typename Monoid, typename Tp>
        struct InfoTable {
            size_type m_size, m_cap;
            std::vector<Tp> m_data;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_cap = std::bit_ceil(m_size = length);
                m_data.resize(m_cap * 2);
                for (size_type i = 0; i != length; i++) m_data[m_cap + i] = mapping(i);
                std::fill(m_data.data() + m_cap + length, m_data.data() + m_cap * 2, Monoid::identity());
                for (size_type l = m_cap, r = m_cap * 2; r /= 2, l /= 2;)
                    for (size_type i = l; i != r; i++) m_data[i] = Monoid::op(m_data[i * 2], m_data[i * 2 + 1]);
            }
            void modify(size_type i, Tp val) {
                m_data[i += m_cap] = val;
                while (i >>= 1) m_data[i] = Monoid::op(m_data[i * 2], m_data[i * 2 + 1]);
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
        template <typename Monoid>
        struct InfoTable<Monoid, void> {};
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
        struct Has_Sum_Type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_Sum_Type<Tp, ValueType, void_t<typename Tp::sum_type>> : std::true_type {
            using type = typename Tp::sum_type;
        };
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Reversed : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Reversed<Tp, ValueType, void_t<decltype(Tp::reversed(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename KeyType, typename Monoid, typename Compare = std::less<KeyType>>
        class Tree {
        public:
            using group = Monoid;
            using value_type = typename group::value_type;
            using sum_type = typename Has_Sum_Type<group, value_type>::type;
            using value_info_type = typename std::conditional<std::is_void<value_type>::value, VoidInfo, value_type>::type;
            using sum_info_type = typename std::conditional<std::is_void<sum_type>::value, VoidInfo, sum_type>::type;
            static constexpr bool has_op = !std::is_void<sum_type>::value && Has_Op<group, sum_type>::value, has_reversed = has_op && Has_Reversed<group, sum_type>::value;
            struct node : NodeWrapper<KeyType, group, value_type, sum_type, has_op && !has_reversed>::template type<node> {
                size_type m_lc, m_rc, m_sz;
                priority_type m_rd;
                bool is_null() const { return !m_rd; }
            };
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
                static sum_type add(const sum_type &x, const sum_type &y) {
                    if constexpr (LeftAdd)
                        return group::op(x, y);
                    else
                        return group::op(y, x);
                }
                static sum_type get(const node *p) {
                    if constexpr (Reversed)
                        return p->get_sum_rev();
                    else
                        return p->get_sum();
                }
            };
            size_type m_length;
            std::vector<bool> m_reversed;
            std::vector<size_type> m_trees, m_bit, m_prev, m_next;
            std::vector<node> m_nodes;
            InfoTable<group, sum_type> m_table;
        private:
            void _update_size(size_type x) { m_nodes[x].m_sz = m_nodes[m_nodes[x].m_lc].m_sz + m_nodes[m_nodes[x].m_rc].m_sz + 1; }
            void _pushup(size_type x) {
                if constexpr (has_op) m_nodes[x].pushup(&m_nodes[m_nodes[x].m_lc], &m_nodes[m_nodes[x].m_rc]);
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
                if (less(m_nodes[rt].m_key))
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
                KeyType x_key = m_nodes[x].m_key;
                _split_by_key(y, &a, &b, [&](const KeyType &key) { return x_key <= key; });
                _merge(&m_nodes[x].m_lc, m_nodes[x].m_lc, a), _merge(&m_nodes[x].m_rc, m_nodes[x].m_rc, b), _update_size(*rt = x), _pushup(x);
            }
            void _merge(size_type &rt1, size_type &rt2) { _merge(&rt1, rt1, rt2); }
            template <typename Getter, typename Judger>
            size_type _bisect(size_type rt, sum_info_type &val, Judger &&judge) const {
                size_type sz = m_nodes[Getter::lchild(&m_nodes[rt])].m_sz;
                if (sz) {
                    auto old_val(val);
                    val = Getter::add(val, Getter::get(&m_nodes[Getter::lchild(&m_nodes[rt])]));
                    if (!judge(val)) return _bisect<Getter>(Getter::lchild(&m_nodes[rt]), old_val, judge);
                }
                val = m_nodes[rt].m_val + val;
                if (!judge(val)) return sz - 1;
                if (!Getter::rchild(&m_nodes[rt])) return sz;
                return sz + 1 + _bisect<Getter>(Getter::rchild(&m_nodes[rt]), val, judge);
            }
            template <typename Judger>
            size_type _max_right(size_type rt, sum_info_type &val, Judger &&judge, bool rev) const {
                if (rev)
                    return _bisect<InfoGetter<true, true>>(rt, val, judge);
                else
                    return _bisect<InfoGetter<false, true>>(rt, val, judge);
            }
            template <typename Judger>
            size_type _min_left(size_type rt, sum_info_type &val, Judger &&judge, bool rev) const {
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
            sum_type _tree_info(size_type rt, bool rev) const { return rev ? m_nodes[rt].get_sum_rev() : m_nodes[rt].get_sum(); }
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
                if constexpr (has_op) m_table.modify(left, _tree_info(m_trees[left], m_reversed[left]));
            }
            void _remove_call(size_type left) {
                if constexpr (has_op) m_table.modify(left, group::identity());
            }
            template <typename InitKeyMapping, typename InitMapping>
            void _init(size_type length, InitKeyMapping key_mapping, InitMapping mapping) {
                m_nodes.resize(length + 1), m_reversed.resize(length), m_trees.resize(length);
                if constexpr (std::is_void<value_type>::value)
                    for (size_type i = 0; i != length; i++) m_trees[i] = i + 1, m_nodes[i + 1].m_lc = m_nodes[i + 1].m_rc = 0, m_nodes[i + 1].m_sz = 1, m_nodes[i + 1].m_rd = fhq_rand(), m_nodes[i + 1].m_key = key_mapping(i), _pushup(i + 1);
                else if constexpr (!has_op)
                    for (size_type i = 0; i != length; i++) m_trees[i] = i + 1, m_nodes[i + 1].m_lc = m_nodes[i + 1].m_rc = 0, m_nodes[i + 1].m_sz = 1, m_nodes[i + 1].m_rd = fhq_rand(), m_nodes[i + 1].m_key = key_mapping(i), m_nodes[i + 1].m_val = mapping(i), _pushup(i + 1);
                else {
                    m_nodes[0].m_val = group::identity();
                    for (size_type i = 0; i != length; i++) m_trees[i] = i + 1, m_nodes[i + 1].m_lc = m_nodes[i + 1].m_rc = 0, m_nodes[i + 1].m_sz = 1, m_nodes[i + 1].m_rd = fhq_rand(), m_nodes[i + 1].m_key = key_mapping(i), m_nodes[i + 1].m_val = mapping(i), _pushup(i + 1);
                    m_table.resize(length, [&](size_type i) { return m_nodes[i + 1].m_val; });
                }
                m_length = std::bit_ceil(length + 1), m_bit.resize(m_length), m_prev.resize(m_length), m_next.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_bit[i] = size_type(1) << std::countr_zero(~i);
                for (size_type i = 0; i != m_length; i++) m_prev[i] = i - 1;
                for (size_type i = 0; i != m_length; i++) m_next[i] = i + 1;
            }
        public:
            Tree() = default;
            template <typename InitKeyMapping>
            Tree(size_type length, InitKeyMapping key_mapping) { _init(length, key_mapping, 0); }
            template <typename InitKeyMapping, typename InitMapping>
            Tree(size_type length, InitKeyMapping key_mapping, InitMapping mapping) { _init(length, key_mapping, mapping); }
            size_type size() const { return m_trees.size(); }
            void modify(size_type i, KeyType key, const value_info_type &val = {}) {
                size_type pre = _kth(_presum(i) - 1);
                if (pre != i) _split_to(pre, i, m_next[pre] - pre);
                if (m_next[i] > i + 1) _split_to(i, i + 1, m_next[i] - i);
                node *rt = &m_nodes[m_trees[i]];
                if constexpr (!std::is_void<value_type>::value) rt->m_val = val;
                rt->m_key = key, _pushup(m_trees[i]), m_reversed[i] = false, _update_call(i);
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
            value_type query(size_type i) const { return get_node(i)->m_val; }
            sum_type query(size_type left, size_type right) {
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
            sum_type query_all() const { return m_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                size_type pre = _kth(_presum(left) - 1);
                if (pre != left) _split_to(pre, left, m_next[pre] - pre);
                size_type res = m_table.max_right(left, judge);
                if (res == size() - 1) return res;
                sum_type val = res == left - 1 ? group::identity() : m_table.query(left, res);
                return res + _max_right(m_trees[res + 1], val, judge, m_reversed[res + 1]) + 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                size_type pre = _kth(_presum(right) - 1);
                if (m_next[pre] != right + 1) _split_to(pre, right + 1, m_next[pre] - pre);
                size_type res = m_table.min_left(right, judge);
                if (!res) return res;
                sum_type val = res == right + 1 ? group::identity() : m_table.query(res, right);
                return res + _min_left(m_trees[res - 1], val, judge, m_reversed[res - 1]) - 1;
            }
        };
        template <typename Ostream, typename KeyType, typename Monoid, typename Compare>
        Ostream &operator<<(Ostream &out, const Tree<KeyType, Monoid, Compare> &x) {
            out << '{';
            using value_type = typename Tree<KeyType, Monoid, Compare>::value_type;
            using node = typename Tree<KeyType, Monoid, Compare>::node;
            for (size_type i = 0, j = 0; i != x.size(); i = x.m_next[i]) {
                out << (i ? ", {" : "{");
                size_type cnt = x.m_nodes[x.m_trees[i]].m_sz;
                for (size_type k = 0; k != cnt; j++, k++) {
                    auto p = x.get_node(j);
                    if (k) out << ", ";
                    out << p->m_key;
                    if constexpr (!std::is_void<value_type>::value) out << ':' << p->m_val;
                }
                out << '}';
            }
            return out << '}';
        }
    }
    template <typename KeyType, typename ValueType, typename SumType, ValueType Identity, typename Compare = std::less<KeyType>, typename Operation, typename KeyMapping, typename InitMapping, typename TreeType = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<ValueType, SumType, Identity, Operation, SORTFHQ::Self>, Compare>>
    auto make_SortFHQ(SORTFHQ::size_type length, Operation op, KeyMapping key_mapping, InitMapping mapping) -> TreeType { return TreeType(length, key_mapping, mapping); }
    template <typename KeyType, typename Compare = std::less<KeyType>>
    using SortFHQTreap = SORTFHQ::Tree<KeyType, SORTFHQ::VoidMonoid, Compare>;
    template <typename KeyType, typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using SortMaxTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, Minimum, SORTFHQ::ChoiceByCompare<Tp, std::less<Tp>>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using SortMinTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, Maximum, SORTFHQ::ChoiceByCompare<Tp, std::greater<Tp>>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp>
    using SortGcdTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, 0, SORTFHQ::FpTransfer<Tp, std::gcd<Tp>>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp>
    using SortLcmTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, 1, SORTFHQ::FpTransfer<Tp, std::lcm<Tp>>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp, Tp OneMask = Tp(-1)>
    using SortBitAndTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp, Tp ZeroMask = 0>
    using SortBitOrTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>, SORTFHQ::Self>>;
    template <typename KeyType, typename Tp, Tp ZeroMask = 0>
    using SortBitXorTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>, SORTFHQ::Self>>;
    template <typename KeyType, typename ValueType, typename SumType, ValueType Zero = ValueType()>
    using SortSumTreap = SORTFHQ::Tree<KeyType, SORTFHQ::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>, SORTFHQ::Self>>;
}

#endif