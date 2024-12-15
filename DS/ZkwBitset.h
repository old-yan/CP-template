/*
最后修改:
20241202
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ZKWBITSET__
#define __OY_ZKWBITSET__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace ZkwBitset {
        using size_type = uint32_t;
        template <bool MaintainLongest>
        struct NodeWrap {
            template <typename Node>
            struct type {
                using info = void;
                using bound_info = size_type;
            };
        };
        template <>
        struct NodeWrap<true> {
            template <typename Node>
            struct type {
                struct info {
                    size_type m_l0, m_l1, m_r0, m_r1, m_max0, m_max1;
                    static info zero(size_type len) {
                        info x;
                        x.set_zero(len);
                        return x;
                    }
                    static info one(size_type len) {
                        info x;
                        x.set_one(len);
                        return x;
                    }
                    void set_zero(size_type len) {
                        m_l0 = m_r0 = m_max0 = len;
                        m_l1 = m_r1 = m_max1 = 0;
                    }
                    void set_one(size_type len) {
                        m_l1 = m_r1 = m_max1 = len;
                        m_l0 = m_r0 = m_max0 = 0;
                    }
                    void flip() {
                        std::swap(m_l0, m_l1);
                        std::swap(m_r0, m_r1);
                        std::swap(m_max0, m_max1);
                    }
                    info operator+(const info &rhs) const {
                        info res;
                        res.m_l0 = m_max1 ? m_l0 : m_l0 + rhs.m_l0;
                        res.m_l1 = m_max0 ? m_l1 : m_l1 + rhs.m_l1;
                        res.m_r0 = rhs.m_max1 ? rhs.m_r0 : m_r0 + rhs.m_r0;
                        res.m_r1 = rhs.m_max0 ? rhs.m_r1 : m_r1 + rhs.m_r1;
                        res.m_max0 = std::max({m_max0, rhs.m_max0, m_r0 + rhs.m_l0});
                        res.m_max1 = std::max({m_max1, rhs.m_max1, m_r1 + rhs.m_l1});
                        return res;
                    }
                };
                struct bound_info : info {
                    size_type m_sum;
                    bound_info operator+(const bound_info &rhs) const { return {info::operator+(rhs), m_sum + rhs.m_sum}; }
                };
                info m_info;
                void _set_zero(size_type len) { m_info.set_zero(len); }
                void _set_one(size_type len) { m_info.set_one(len); }
                void _flip() { m_info.flip(); }
                void _pushup(Node *lchild, Node *rchild) { m_info = lchild->m_info + rchild->m_info; }
            };
        };
        template <bool MaintainLongest>
        class Tree {
        public:
            using tree_type = Tree<MaintainLongest>;
            struct node : NodeWrap<MaintainLongest>::template type<node> {
                using typename NodeWrap<MaintainLongest>::template type<node>::bound_info;
                size_type m_sum;
                bool m_flipped;
                void set_one(size_type len) {
                    m_sum = len;
                    if constexpr (MaintainLongest) this->_set_one(len);
                }
                void set_zero(size_type len) {
                    m_sum = 0;
                    if constexpr (MaintainLongest) this->_set_zero(len);
                }
                void flip(size_type len) {
                    m_sum = len - m_sum, m_flipped = !m_flipped;
                    if constexpr (MaintainLongest) this->_flip();
                }
                void pushup(node *lchild, node *rchild) {
                    m_sum = lchild->m_sum + rchild->m_sum, m_flipped = false;
                    if constexpr (MaintainLongest) this->_pushup(lchild, rchild);
                }
                bound_info to_info() const {
                    if constexpr (MaintainLongest)
                        return {this->m_info, m_sum};
                    else
                        return m_sum;
                }
            };
            struct CountGetter {
                using value_type = size_type;
                value_type operator()(const node *p) const { return p->m_sum; }
                void operator()(value_type &x, const node *p) const { x = x + p->m_sum; }
                void operator()(const node *p, value_type &x) const { x = p->m_sum + x; }
                void operator()(value_type &x, const value_type &y) const { x = x + y; }
            };
            struct DefaultGetter {
                using value_type = typename node::bound_info;
                value_type operator()(const node *p) const { return p->to_info(); }
                void operator()(value_type &x, const node *p) const { x = x + p->to_info(); }
                void operator()(const node *p, value_type &x) const { x = p->to_info() + x; }
                void operator()(value_type &x, const value_type &y) const { x = x + y; }
            };
            using info = typename node::info;
        private:
            size_type m_size, m_cap, m_dep;
            mutable std::vector<node> m_sub;
            static void _pushdown_one(node *sub, size_type cur, size_type len) { sub[cur * 2].set_one(len >> 1), sub[cur * 2 + 1].set_one(len >> 1); }
            static void _pushdown_zero(node *sub, size_type cur, size_type len) { sub[cur * 2].set_zero(len >> 1), sub[cur * 2 + 1].set_zero(len >> 1); }
            static void _pushdown_flip(node *sub, size_type cur, size_type len) { sub[cur].m_flipped = false, sub[cur * 2].flip(len >> 1), sub[cur * 2 + 1].flip(len >> 1); }
            static void _pushdown(node *sub, size_type cur, size_type len) {
                if (sub[cur].m_sum == len)
                    _pushdown_one(sub, cur, len);
                else if (!sub[cur].m_sum)
                    _pushdown_zero(sub, cur, len);
                else if (sub[cur].m_flipped)
                    _pushdown_flip(sub, cur, len);
            }
            static void _pushup(node *sub, size_type cur) { sub[cur].pushup(sub + cur * 2, sub + cur * 2 + 1); }
            static bool _any(node *sub, size_type cur, size_type floor, size_type ceil, size_type left, size_type right) {
                if (!sub[cur].m_sum) return false;
                if (sub[cur].m_sum == ceil - floor) return true;
                if (left <= floor && right >= ceil) return true;
                if (sub[cur].m_flipped) _pushdown_flip(sub, cur, ceil - floor);
                size_type mid = (floor + ceil) >> 1;
                if (left < mid && _any(sub, cur * 2, floor, mid, left, right)) return true;
                return right > mid && _any(sub, cur * 2 + 1, mid, ceil, left, right);
            }
            static bool _all(node *sub, size_type cur, size_type floor, size_type ceil, size_type left, size_type right) {
                if (!sub[cur].m_sum) return false;
                if (sub[cur].m_sum == ceil - floor) return true;
                if (left <= floor && right >= ceil) return false;
                if (sub[cur].m_flipped) _pushdown_flip(sub, cur, ceil - floor);
                size_type mid = (floor + ceil) >> 1;
                if (left < mid && !_all(sub, cur * 2, floor, mid, left, right)) return false;
                return right <= mid || _all(sub, cur * 2 + 1, mid, ceil, left, right);
            }
            static size_type _kth(node *sub, size_type cur, size_type floor, size_type ceil, size_type k) {
                if (floor + 1 == ceil) return floor;
                if (sub[cur].m_sum == ceil - floor) return floor + k;
                if (sub[cur].m_flipped) _pushdown_flip(sub, cur, ceil - floor);
                size_type mid = (floor + ceil) >> 1;
                if (k < sub[cur * 2].m_sum)
                    return _kth(sub, cur * 2, floor, mid, k);
                else
                    return _kth(sub, cur * 2 + 1, mid, ceil, k - sub[cur * 2].m_sum);
            }
            static size_type _kth_zero(node *sub, size_type cur, size_type floor, size_type ceil, size_type k) {
                if (!sub[cur].m_sum) return floor + k;
                if (sub[cur].m_flipped) _pushdown_flip(sub, cur, ceil - floor);
                size_type mid = (floor + ceil) >> 1;
                if (k < (mid - floor - sub[cur * 2]->m_sum))
                    return _kth_zero(sub, cur * 2, floor, mid, k);
                else
                    return _kth_zero(sub, cur * 2 + 1, mid, ceil, k - (mid - floor - sub[cur * 2]->m_sum));
            }
            template <typename Callback>
            static void _dfs_range(node *sub, size_type cur, size_type floor, size_type ceil, size_type &pre, Callback &&call) {
                if (!sub[cur].m_sum) {
                    if (~pre) call(pre, floor - 1), pre = -1;
                    return;
                }
                if (sub[cur].m_sum != ceil - floor + 1) {
                    if (sub[cur].m_flipped) _pushdown_flip(sub, cur, ceil - floor + 1);
                    size_type mid = (floor + ceil) >> 1;
                    _dfs_range(sub, cur * 2, floor, mid, pre, call), _dfs_range(sub, cur * 2 + 1, mid + 1, ceil, pre, call);
                } else if (!~pre)
                    pre = floor;
            }
            template <bool ReadOnly, typename Callback>
            auto _do_for_node(size_type i, Callback &&call) const -> decltype(call(m_sub.data())) {
                i += m_cap;
                node *sub = m_sub.data();
                for (size_type d = m_dep, len = m_cap; d; d--, len >>= 1) _pushdown(sub, i >> d, len);
                if constexpr (ReadOnly)
                    return call(sub + i);
                else {
                    call(sub + i);
                    while (i >>= 1) _pushup(sub, i);
                }
            }
            template <typename Callback>
            void _do_for_range(size_type left, size_type right, Callback &&call) {
                left += m_cap, right += m_cap;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1, len = m_cap;
                for (size_type d = m_dep; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                call(sub + left, 1), call(sub + right, 1), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) call(sub + (left + 1), len);
                    _pushup(sub, left >>= 1);
                    if (right & 1) call(sub + (right - 1), len);
                    _pushup(sub, right >>= 1), len <<= 1;
                }
                while (left >>= 1) _pushup(sub, left);
            }
            template <typename Judger, typename Callback>
            void _do_for_each(node *sub, size_type cur, size_type floor, size_type ceil, Judger &&judge, Callback &&call) const {
                if (floor >= m_size || !judge(sub + cur, floor, ceil)) return;
                if (floor + 1 == ceil)
                    call(floor);
                else {
                    _pushdown(sub, cur, ceil - floor);
                    size_type mid = (floor + ceil) >> 1;
                    _do_for_each(sub, cur * 2, floor, mid, judge, call), _do_for_each(sub, cur * 2 + 1, mid, ceil, judge, call);
                }
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type _query(size_type i) const {
                return _do_for_node<true>(i, [](const node *p) { return Getter()(p); });
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type _query(size_type left, size_type right) const {
                if (left == right) return _query<Getter>(left);
                left += m_cap, right += m_cap;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1, len = m_cap;
                for (size_type d = m_dep; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                typename Getter::value_type resl = Getter()(sub + left), resr = Getter()(sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) Getter()(resl, sub + (left ^ 1));
                    if (right & 1) Getter()(sub + (right ^ 1), resr);
                }
                Getter()(resl, resr);
                return resl;
            }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type _max_right(size_type left, Judger &&judge) const {
                left += m_cap;
                node *sub = m_sub.data();
                size_type l = 0;
                for (size_type d = m_dep, len = m_cap; d; d--, len >>= 1) _pushdown(sub, left >> d, len);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + left);
                if (!judge(val, 1)) return left - m_cap - 1;
                left++, l++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    get_type a(val);
                    Getter()(a, sub + (left >>= ctz));
                    len <<= ctz;
                    if (judge(a, l + len))
                        val = a, left++, l += len;
                    else {
                        for (; left < m_cap; len >>= 1) {
                            _pushdown(sub, left, len);
                            get_type a(val);
                            Getter()(a, sub + (left <<= 1));
                            if (judge(a, l + (len >> 1))) val = a, left++, l += len >> 1;
                        }
                        return std::min(left - m_cap, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type _min_left(size_type right, Judger &&judge) const {
                right += m_cap;
                node *sub = m_sub.data();
                size_type l = 0;
                for (size_type d = m_dep, len = m_cap; d; d--, len >>= 1) _pushdown(sub, right >> d, len);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + right);
                if (!judge(val, 1)) return right - m_cap + 1;
                l++;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_cap);
                    get_type a(val);
                    Getter()(sub + ((right >>= ctz) - 1), a);
                    len <<= ctz;
                    if (judge(a, l + len))
                        val = a, right--, l += len;
                    else {
                        for (; right <= m_cap; len >>= 1) {
                            _pushdown(sub, right - 1, len);
                            get_type a(val);
                            Getter()(sub + ((right <<= 1) - 1), a);
                            if (judge(a, l + (len >> 1))) val = a, right--, l += len >> 1;
                        }
                        return right - m_cap;
                    }
                }
                return 0;
            }
        public:
            Tree(size_type length = 0) { resize(length); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_dep = std::max<size_type>(1, std::bit_width(m_size - 1)), m_cap = 1 << m_dep;
                m_sub.assign(m_cap * 2, {});
                node *sub = m_sub.data();
                for (size_type len = m_cap, w = 1; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) sub[i].set_zero(w);
            }
            size_type size() const { return m_size; }
            void set() { set(0, m_size - 1); }
            void set(size_type i) {
                _do_for_node<false>(i, [](node *p) { p->set_one(1); });
            }
            void set(size_type left, size_type right) {
                if (left == right) return set(left);
                _do_for_range(left, right, [](node *p, size_type len) { p->set_one(len); });
            }
            void reset() { m_sub[1].m_sum = 0; }
            void reset(size_type i) {
                _do_for_node<false>(i, [](node *p) { p->set_zero(1); });
            }
            void reset(size_type left, size_type right) {
                if (left == right) return reset(left);
                _do_for_range(left, right, [](node *p, size_type len) { p->set_zero(len); });
            }
            void flip() { flip(0, m_size - 1); }
            void flip(size_type i) {
                _do_for_node<false>(i, [](node *p) { p->flip(1); });
            }
            void flip(size_type left, size_type right) {
                if (left == right) return flip(left);
                _do_for_range(left, right, [](node *p, size_type len) { p->flip(len); });
            }
            size_type count() const { return m_sub[1].m_sum; }
            size_type count(size_type left, size_type right) const { return _query<CountGetter>(left, right); }
            bool any() const { return m_sub[1].m_sum; }
            bool any(size_type left, size_type right) const { return _any(m_sub.data(), 1, 0, m_cap, left, right + 1); }
            bool all() const { return m_sub[1].m_sum == m_size; }
            bool all(size_type left, size_type right) const { return _all(m_sub.data(), 1, 0, m_cap, left, right + 1); }
            size_type first_one() const { return any() ? kth(0) : -1; }
            size_type last_one() const { return any() ? kth(count() - 1) : -1; }
            size_type first_zero() const { return all() ? -1 : _kth_zero(m_sub.data(), 1, 0, m_cap, 0); }
            size_type last_zero() const { return all() ? -1 : _kth_zero(m_sub.data(), 1, 0, m_cap, size() - count() - 1); }
            size_type prev_one(size_type i) const {
                return i ? min_left<CountGetter>(i - 1, [](size_type cnt) { return !cnt; }) - 1 : -1;
            }
            size_type next_one(size_type i) const {
                size_type res = i + 1 < m_size ? max_right<CountGetter>(i + 1, [](size_type cnt) { return !cnt; }) + 1 : -1;
                return res == m_size ? -1 : res;
            }
            size_type prev_zero(size_type i) const {
                return i ? _min_left<CountGetter>(i - 1, [](size_type cnt, size_type l) { return cnt == l; }) - 1 : -1;
            }
            size_type next_zero(size_type i) const {
                size_type res = i + 1 < m_size ? _max_right<CountGetter>(i + 1, [](size_type cnt, size_type l) { return cnt == l; }) + 1 : -1;
                return res == m_size ? -1 : res;
            }
            bool at(size_type i) const {
                return _do_for_node<true>(i, [](const node *p) { return p->m_sum; });
            }
            bool operator[](size_type i) const { return at(i); }
            size_type kth(size_type k, size_type pos = 0) const { return _kth(m_sub.data(), 1, 0, m_cap, (pos ? count(0, pos - 1) : 0) + k); }
            size_type longest_ones(size_type left, size_type right) const { return _query(left, right).m_max1; }
            size_type longest_zeros(size_type left, size_type right) const { return _query(left, right).m_max0; }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                return _max_right<Getter>(left, [&judge](typename Getter::value_type &val, size_type) { return judge(val); });
            }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                return _min_left<Getter>(right, [&judge](typename Getter::value_type &val, size_type) { return judge(val); });
            }
            template <typename Callback>
            void enumerate_range(Callback &&call) const {
                size_type pre = -1;
                _dfs_range(m_sub.data(), 1, 0, m_cap, pre, call);
                if (~pre) call(pre, m_size - 1);
            }
            template <typename Callback>
            void enumerate_one(Callback &&call) const {
                _do_for_each(m_sub.data(), 1, 0, m_cap, [](const node *p, size_type, size_type) { return p->m_sum; }, call);
            }
            template <typename Callback>
            void enumerate_zero(Callback &&call) const {
                _do_for_each(m_sub.data(), 1, 0, m_cap, [](const node *p, size_type floor, size_type ceil) { return p->m_sum != ceil - floor; }, call);
            }
        };
        template <typename Ostream, bool MaintainLongest>
        Ostream &operator<<(Ostream &out, Tree<MaintainLongest> &x) {
            out << "[";
            auto call = [&, i = 0](size_type pos) mutable {
                if (i++) out << ", ";
                out << pos;
            };
            x.enumerate_one(call);
            return out << "]";
        }
    }
    template <bool MaintainLongest = false>
    using ZkwTreeBitset = ZkwBitset::Tree<MaintainLongest>;
}

#endif