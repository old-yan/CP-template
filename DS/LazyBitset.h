/*
最后修改:
20240815
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LAZYBITSET__
#define __OY_LAZYBITSET__

#include <functional>
#include <numeric>

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace LazyBitset {
        using size_type = uint32_t;
        template <typename SizeType, bool MaintainLongest>
        struct NodeWrap {
            template <typename Node>
            struct type {
                using info = void;
                using bound_info = SizeType;
            };
        };
        template <typename SizeType>
        struct NodeWrap<SizeType, true> {
            template <typename Node>
            struct type {
                struct info {
                    SizeType m_l0, m_l1, m_r0, m_r1, m_max0, m_max1;
                    static info zero(SizeType len) {
                        info x;
                        x.set_zero(len);
                        return x;
                    }
                    static info one(SizeType len) {
                        info x;
                        x.set_one(len);
                        return x;
                    }
                    void set_zero(SizeType len) {
                        m_l0 = m_r0 = m_max0 = len;
                        m_l1 = m_r1 = m_max1 = 0;
                    }
                    void set_one(SizeType len) {
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
                    SizeType m_sum;
                    bound_info operator+(const bound_info &rhs) const { return {info::operator+(rhs), m_sum + rhs.m_sum}; }
                };
                info m_info;
                void _set_zero(SizeType len) { m_info.set_zero(len); }
                void _set_one(SizeType len) { m_info.set_one(len); }
                void _flip() { m_info.flip(); }
                void _pushup(Node *lchild, Node *rchild, SizeType len1, SizeType len2) { m_info = (lchild->is_null() ? info::zero(len1) : lchild->m_info) + (rchild->is_null() ? info::zero(len2) : rchild->m_info); }
            };
        };
        template <typename SizeType, bool MaintainLongest, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using tree_type = Tree<SizeType, MaintainLongest, BufferType>;
            struct node : NodeWrap<SizeType, MaintainLongest>::template type<node> {
                using typename NodeWrap<SizeType, MaintainLongest>::template type<node>::bound_info;
                SizeType m_sum;
                bool m_flipped;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
                void set_one(SizeType len) {
                    m_sum = len;
                    if constexpr (MaintainLongest) this->_set_one(len);
                }
                void set_zero(SizeType len) {
                    m_sum = 0;
                    if constexpr (MaintainLongest) this->_set_zero(len);
                }
                void flip(SizeType len) {
                    m_sum = len - m_sum, m_flipped = !m_flipped;
                    if constexpr (MaintainLongest) this->_flip();
                }
                void pushup(node *lchild, node *rchild, SizeType len) {
                    m_sum = lchild->m_sum + rchild->m_sum, m_flipped = false;
                    if constexpr (MaintainLongest) this->_pushup(lchild, rchild, (len + 1) >> 1, len >> 1);
                }
                bound_info to_info() const {
                    if constexpr (MaintainLongest)
                        return {this->m_info, m_sum};
                    else
                        return m_sum;
                }
            };
            struct DefaultGetter {
                using value_type = typename node::bound_info;
                value_type operator()(const node *p) const { return p->to_info(); }
                void operator()(value_type &x, const node *p) const { x = x + p->to_info(); }
                void operator()(const node *p, value_type &x) const { x = p->to_info() + x; }
                void operator()(value_type &x, const value_type &y) const { x = x + y; }
            };
            using buffer_type = BufferType<node>;
            using info = typename node::info;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_root{};
            SizeType m_size{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static void _collect(size_type x) { buffer_type::collect(x); }
            static void _pushdown_one(size_type cur, SizeType len) {
                _lchild(cur, (len + 1) >> 1)->set_one((len + 1) >> 1), _rchild(cur, len >> 1)->set_one(len >> 1);
            }
            static void _pushdown_zero(size_type cur, SizeType len) {
                if (_ptr(cur)->m_lc) _ptr(cur)->lchild()->set_zero((len + 1) >> 1);
                if (_ptr(cur)->m_rc) _ptr(cur)->rchild()->set_zero(len >> 1);
            }
            static void _pushdown_flip(size_type cur, SizeType len) {
                _ptr(cur)->m_flipped = false;
                _lchild(cur, (len + 1) >> 1)->flip((len + 1) >> 1), _rchild(cur, len >> 1)->flip(len >> 1);
            }
            static void _pushdown(size_type cur, SizeType len) {
                if (_ptr(cur)->m_sum == len)
                    _pushdown_one(cur, len);
                else if (!_ptr(cur)->m_sum)
                    _pushdown_zero(cur, len);
                else if (_ptr(cur)->m_flipped)
                    _pushdown_flip(cur, len);
            }
            static node *_lchild(size_type cur, SizeType len) { return _ptr(__lchild(cur, len)); }
            static node *_rchild(size_type cur, SizeType len) { return _ptr(__rchild(cur, len)); }
            static size_type __lchild(size_type cur, SizeType len) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_lc = c;
                    if constexpr (MaintainLongest) _ptr(c)->m_info = info::zero(len);
                }
                return _ptr(cur)->m_lc;
            }
            static size_type __rchild(size_type cur, SizeType len) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_rc = c;
                    if constexpr (MaintainLongest) _ptr(c)->m_info = info::zero(len);
                }
                return _ptr(cur)->m_rc;
            }
            static void _pushup(size_type cur, SizeType len) {
                node *p = _ptr(cur);
                p->pushup(p->lchild(), p->rchild(), len);
            }
            static void _set(size_type cur, SizeType len, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (p->m_sum == len) return;
                if (!left && right == len - 1)
                    p->set_one(len);
                else {
                    if (!p->m_sum)
                        _pushdown_zero(cur, len);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, len);
                    if (left < (len + 1) >> 1) _set(__lchild(cur, (len + 1) >> 1), (len + 1) >> 1, left, std::min((len - 1) >> 1, right));
                    if (right >= (len + 1) >> 1) _set(__rchild(cur, len >> 1), len >> 1, std::max((len + 1) >> 1, left) - ((len + 1) >> 1), right - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static void _set(size_type cur, SizeType len, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == len) return;
                if (len == 1)
                    p->set_one(1);
                else {
                    if (!p->m_sum)
                        _pushdown_zero(cur, len);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, len);
                    if (i < (len + 1) >> 1)
                        _set(__lchild(cur, (len + 1) >> 1), (len + 1) >> 1, i);
                    else
                        _set(__rchild(cur, len >> 1), len >> 1, i - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static void _reset(size_type cur, SizeType len, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return;
                if (!left && right == len - 1)
                    p->set_zero(len);
                else {
                    if (p->m_sum == len)
                        _pushdown_one(cur, len), p = _ptr(cur);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, len), p = _ptr(cur);
                    if (left < (len + 1) >> 1) _reset(__lchild(cur, (len + 1) >> 1), (len + 1) >> 1, left, std::min((len - 1) >> 1, right));
                    if (right >= (len + 1) >> 1) _reset(__rchild(cur, len >> 1), len >> 1, std::max((len + 1) >> 1, left) - ((len + 1) >> 1), right - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static void _reset(size_type cur, SizeType len, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return;
                if (len == 1)
                    p->set_zero(1);
                else {
                    if (p->m_sum == len)
                        _pushdown_one(cur, len), p = _ptr(cur);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, len), p = _ptr(cur);
                    if (i < (len + 1) >> 1)
                        _reset(p->m_lc, (len + 1) >> 1, i);
                    else
                        _reset(p->m_rc, len >> 1, i - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static void _flip(size_type cur, SizeType len, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!left && right == len - 1)
                    p->flip(len);
                else {
                    _pushdown(cur, len);
                    if (left < (len + 1) >> 1) _flip(__lchild(cur, (len + 1) >> 1), (len + 1) >> 1, left, std::min((len - 1) >> 1, right));
                    if (right >= (len + 1) >> 1) _flip(__rchild(cur, len >> 1), len >> 1, std::max((len + 1) >> 1, left) - ((len + 1) >> 1), right - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static void _flip(size_type cur, SizeType len, SizeType i) {
                node *p = _ptr(cur);
                if (len == 1)
                    p->flip(1);
                else {
                    _pushdown(cur, len);
                    if (i < (len + 1) >> 1)
                        _flip(__lchild(cur, (len + 1) >> 1), (len + 1) >> 1, i);
                    else
                        _flip(__rchild(cur, len >> 1), len >> 1, i - ((len + 1) >> 1));
                    _pushup(cur, len);
                }
            }
            static SizeType _count(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return 0;
                if (p->m_sum == ceil - floor + 1) return std::min(ceil, right) - std::max(floor, left) + 1;
                if (left <= floor && right >= ceil) return p->m_sum;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid) res += _count(p->m_lc, floor, mid, left, right), p = _ptr(cur);
                if (right > mid) res += _count(p->m_rc, mid + 1, ceil, left, right);
                return res;
            }
            static info _longest(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                static_assert(MaintainLongest, "MaintainLongest Must Be True");
                node *p = _ptr(cur);
                if (!p->m_sum) return info::zero(std::min(ceil, right) - std::max(floor, left) + 1);
                if (p->m_sum == ceil - floor + 1) return info::one(std::min(ceil, right) - std::max(floor, left) + 1);
                if (left <= floor && right >= ceil) return p->m_info;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                info res{};
                if (left <= mid) res = res + _longest(p->m_lc, floor, mid, left, right), p = _ptr(cur);
                if (right > mid) res = res + _longest(p->m_rc, mid + 1, ceil, left, right);
                return res;
            }
            static bool _any(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return false;
                if (p->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return true;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (left <= mid && _any(p->m_lc, floor, mid, left, right)) return true;
                return right > mid && _any(_ptr(cur)->m_rc, mid + 1, ceil, left, right);
            }
            static bool _all(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return 0;
                if (p->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return false;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid && !_all(p->m_lc, floor, mid, left, right)) return false;
                return right <= mid || _all(_ptr(cur)->m_rc, mid + 1, ceil, left, right);
            }
            static SizeType _first_one(size_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return floor;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->lchild()->m_sum) return _first_one(p->m_lc, floor, mid);
                return _first_one(p->m_rc, mid + 1, ceil);
            }
            static SizeType _last_one(size_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return ceil;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->rchild()->m_sum) return _last_one(p->m_rc, mid + 1, ceil);
                return _last_one(p->m_lc, floor, mid);
            }
            static SizeType _first_zero(size_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (!p->m_sum) return floor;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->lchild()->m_sum != mid - floor + 1) return _first_zero(p->m_lc, floor, mid);
                return _first_zero(p->m_rc, mid + 1, ceil);
            }
            static SizeType _last_zero(size_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (!p->m_sum) return ceil;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->rchild()->m_sum != ceil - mid) return _last_zero(p->m_rc, mid + 1, ceil);
                return _last_zero(p->m_lc, floor, mid);
            }
            static SizeType _prev_one(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return -1;
                if (p->m_sum == ceil - floor + 1) return std::min(i - 1, ceil);
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_one(p->m_rc, mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_one(p->m_lc, floor, mid, i);
            }
            static SizeType _next_one(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return -1;
                if (p->m_sum == ceil - floor + 1) return std::max(i + 1, floor);
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_one(p->m_lc, floor, mid, i);
                    if (~res) return res;
                }
                return _next_one(p->m_rc, mid + 1, ceil, i);
            }
            static SizeType _prev_zero(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return -1;
                if (!p->m_sum) return std::min(i - 1, ceil);
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_zero(p->m_rc, mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_zero(p->m_lc, floor, mid, i);
            }
            static SizeType _next_zero(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return -1;
                if (!p->m_sum) return std::max(i + 1, floor);
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_zero(p->m_lc, floor, mid, i);
                    if (~res) return res;
                }
                return _next_zero(p->m_rc, mid + 1, ceil, i);
            }
            static bool _at(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return false;
                if (p->m_sum == ceil - floor + 1) return true;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid)
                    return _at(p->m_lc, floor, mid, i);
                else
                    return _at(p->m_rc, mid + 1, ceil, i);
            }
            static SizeType _kth(size_type cur, SizeType floor, SizeType ceil, SizeType k) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return floor + k;
                if (p->m_flipped) _pushdown_flip(cur, ceil - floor + 1), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (k < p->lchild()->m_sum)
                    return _kth(p->m_lc, floor, mid, k);
                else
                    return _kth(p->m_rc, mid + 1, ceil, k - p->lchild()->m_sum);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            static SizeType _max_right(size_type cur, SizeType floor, SizeType ceil, SizeType start, typename Getter::value_type &val, Judger &&judge) {
                if (start <= floor) {
                    auto a(val);
                    if (start < floor)
                        Getter()(a, _ptr(cur));
                    else
                        a = Getter()(_ptr(cur));
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor == ceil)
                        return floor - 1;
                }
                SizeType len = ceil - floor + 1, mid = (floor + ceil) >> 1;
                _pushdown(cur, len);
                if (start <= mid) {
                    SizeType res = _max_right<Getter>(__lchild(cur, (len + 1) >> 1), floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right<Getter>(__rchild(cur, len >> 1), mid + 1, ceil, start, val, judge);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            static SizeType _min_left(size_type cur, SizeType floor, SizeType ceil, SizeType start, typename Getter::value_type &val, Judger &&judge) {
                if (start >= ceil) {
                    auto a(val);
                    if (start > ceil)
                        Getter()(_ptr(cur), a);
                    else
                        a = Getter()(_ptr(cur));
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                SizeType len = ceil - floor + 1, mid = (floor + ceil) >> 1;
                _pushdown(cur, len);
                if (start > mid) {
                    SizeType res = _min_left<Getter>(__rchild(cur, len >> 1), mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left<Getter>(__lchild(cur, (len + 1) >> 1), floor, mid, start, val, judge);
            }
            template <typename Callback>
            static void _dfs_range(size_type cur, SizeType floor, SizeType ceil, SizeType &pre, Callback &&call) {
                if (!_ptr(cur)->m_sum) {
                    if (~pre) call(pre, floor - 1), pre = -1;
                    return;
                }
                if (_ptr(cur)->m_sum != ceil - floor + 1) {
                    if (_ptr(cur)->m_flipped) _pushdown_flip(cur, ceil - floor + 1);
                    SizeType mid = (floor + ceil) >> 1;
                    _dfs_range(_ptr(cur)->m_lc, floor, mid, pre, call);
                    _dfs_range(_ptr(cur)->m_rc, mid + 1, ceil, pre, call);
                } else if (!~pre)
                    pre = floor;
            }
            template <typename Callback>
            static void _dfs1(size_type cur, SizeType floor, SizeType ceil, Callback &&call) {
                if (!_ptr(cur)->m_sum) return;
                if (_ptr(cur)->m_sum == ceil - floor + 1)
                    for (SizeType i = floor; i != ceil + 1; i++) call(i);
                else {
                    if (_ptr(cur)->m_flipped) _pushdown_flip(cur, ceil - floor + 1);
                    SizeType mid = (floor + ceil) >> 1;
                    _dfs1(_ptr(cur)->m_lc, floor, mid, call);
                    _dfs1(_ptr(cur)->m_rc, mid + 1, ceil, call);
                }
            }
            template <typename Callback>
            static void _dfs0(size_type cur, SizeType floor, SizeType ceil, Callback &&call) {
                if (_ptr(cur)->m_sum == ceil - floor + 1) return;
                if (!_ptr(cur)->m_sum)
                    for (SizeType i = floor; i != ceil + 1; i++) call(i);
                else {
                    if (_ptr(cur)->m_flipped) _pushdown_flip(cur, ceil - floor + 1);
                    SizeType mid = (floor + ceil) >> 1;
                    _dfs0(_ptr(cur)->m_lc, floor, mid, call);
                    _dfs0(_ptr(cur)->m_rc, mid + 1, ceil, call);
                }
            }
            static void _bit_and(size_type &x, size_type &y, SizeType len) {
                auto p = _ptr(x), q = _ptr(y);
                if (!p->m_sum || !q->m_sum) {
                    if (x) _collect_all(x), x = 0;
                    if (y) _collect_all(y), y = 0;
                    return;
                }
                if (p->m_sum == len) {
                    _collect_all(x);
                    return void(x = y);
                }
                if (q->m_sum == len) {
                    _collect_all(y);
                    return;
                }
                if (_ptr(x)->m_flipped) _pushdown_flip(x, len);
                if (_ptr(y)->m_flipped) _pushdown_flip(y, len);
                _bit_and(_ptr(x)->m_lc, _ptr(y)->m_lc, (len + 1) >> 1);
                _bit_and(_ptr(x)->m_rc, _ptr(y)->m_rc, len >> 1);
                _pushup(x, len), _collect(y);
            }
            static void _bit_or(size_type &x, size_type &y, SizeType len) {
                auto p = _ptr(x), q = _ptr(y);
                if (p->m_sum == len || !q->m_sum) {
                    if (y) _collect_all(y), y = 0;
                    return;
                }
                if (q->m_sum == len || !p->m_sum) {
                    if (x) _collect_all(x), x = 0;
                    return void(x = y);
                }
                if (_ptr(x)->m_flipped) _pushdown_flip(x, len);
                if (_ptr(y)->m_flipped) _pushdown_flip(y, len);
                _bit_or(_ptr(x)->m_lc, _ptr(y)->m_lc, (len + 1) >> 1);
                _bit_or(_ptr(x)->m_rc, _ptr(y)->m_rc, len >> 1);
                _pushup(x, len), _collect(y);
            }
            static size_type _copy(size_type y, SizeType len) {
                node *p = _ptr(y);
                if (!p->m_sum) return 0;
                if (p->m_sum == len) {
                    size_type c = _newnode();
                    _ptr(c)->set_one(len);
                    return c;
                }
                if (p->m_flipped) _pushdown_flip(y, len), p = _ptr(y);
                size_type lc = p->m_lc, rc = p->m_rc, nlc = _copy(lc, (len + 1) >> 1), nrc = _copy(rc, len >> 1), c = _newnode();
                _ptr(c)->m_lc = nlc, _ptr(c)->m_rc = nrc, _pushup(c, len);
                return c;
            }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    if (!cur) return;
                    node *p = _ptr(cur);
                    _collect_all(p->m_lc), _collect_all(p->m_rc), _collect(cur);
                }
            }
            template <size_type Unchecked, typename Callback, typename... Args>
            static void _enumerate_bit_and_l(SizeType floor, SizeType ceil, Callback &&call, size_type x, Args... args) {
                if (_ptr(x)->m_flipped) _pushdown_flip(x, ceil - floor + 1);
                size_type l = _ptr(x)->m_lc;
                if (!_ptr(l)->m_sum) return;
                if (_ptr(l)->m_sum != ceil - floor + 1)
                    if constexpr (Unchecked > 1)
                        _enumerate_bit_and_l<Unchecked - 1>(floor, ceil, call, args..., l);
                    else
                        _enumerate_bit_and(floor, ceil, call, args..., l);
                else if constexpr (Unchecked > 1)
                    _enumerate_bit_and_l<Unchecked - 1>(floor, ceil, call, args...);
                else
                    _enumerate_bit_and(floor, ceil, call, args...);
            }
            template <size_type Unchecked, typename Callback, typename... Args>
            static void _enumerate_bit_and_r(SizeType floor, SizeType ceil, Callback &&call, size_type x, Args... args) {
                size_type r = _ptr(x)->m_rc;
                if (!_ptr(r)->m_sum) return;
                if (_ptr(r)->m_sum != ceil - floor + 1)
                    if constexpr (Unchecked > 1)
                        _enumerate_bit_and_r<Unchecked - 1>(floor, ceil, call, args..., r);
                    else
                        _enumerate_bit_and(floor, ceil, call, args..., r);
                else if constexpr (Unchecked > 1)
                    _enumerate_bit_and_r<Unchecked - 1>(floor, ceil, call, args...);
                else
                    _enumerate_bit_and(floor, ceil, call, args...);
            }
            template <size_type Unchecked, typename... Args>
            static void _enumerate_bit_and_check(SizeType len, Tree<SizeType, MaintainLongest, BufferType> &x, Args &&...args) {
                if (!x.count()) return;
                if (x.count() == len)
                    if constexpr (Unchecked > 1)
                        _enumerate_bit_and_check<Unchecked - 1>(len, args...);
                    else
                        _enumerate_bit_and(0, len - 1, args...);
                else if constexpr (Unchecked > 1)
                    _enumerate_bit_and_check<Unchecked - 1>(len, args..., x.m_root);
                else
                    _enumerate_bit_and(0, len - 1, args..., x.m_root);
            }
            template <typename Callback, typename... Args>
            static void _enumerate_bit_and(SizeType floor, SizeType ceil, Callback &&call, Args... args) {
                if constexpr (sizeof...(Args) == 0)
                    for (SizeType i = floor; i != ceil + 1; i++) call(i);
                else if (floor == ceil)
                    call(floor);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _enumerate_bit_and_l<sizeof...(Args)>(floor, mid, call, args...);
                    _enumerate_bit_and_r<sizeof...(Args)>(mid + 1, ceil, call, args...);
                }
            }
            size_type _root_get() {
                if (!m_root) {
                    m_root = _newnode();
                    if constexpr (MaintainLongest) _ptr(m_root)->m_info = info::zero(m_size);
                }
                return m_root;
            }
            node *_root() { return _ptr(_root_get()); }
        public:
            template <typename... TreesAndCallback>
            static void enumerate_bit_and_ones(Tree<SizeType, MaintainLongest, BufferType> &tree, TreesAndCallback &&...trees_and_call) { _enumerate_bit_and_check<sizeof...(TreesAndCallback)>(tree.size(), tree, trees_and_call...); }
            Tree() = default;
            Tree(SizeType length) { resize(length); }
            Tree(const tree_type &rhs) : m_size(rhs.m_size) {
                if (rhs.m_root) m_root = _copy(rhs.m_root, m_size);
            }
            Tree(tree_type &&rhs) noexcept { std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size), rhs.clear(); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) {
                if (this == &rhs) return *this;
                clear(), m_size = rhs.m_size;
                if (rhs.m_root) m_root = _copy(rhs.m_root, m_size);
                return *this;
            }
            tree_type &operator=(tree_type &&rhs) noexcept {
                if (this == &rhs) return *this;
                std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size), rhs.clear();
                return *this;
            }
            void resize(SizeType length) { clear(), m_size = length; }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0, m_size = 0;
            }
            SizeType size() const { return m_size; }
            void set() { _root()->m_sum = m_size; }
            void set(SizeType i) { _set(_root_get(), m_size, i); }
            void set(SizeType left, SizeType right) { _set(_root_get(), m_size, left, right); }
            void reset() { _root()->m_sum = 0; }
            void reset(SizeType i) { _reset(_root_get(), m_size, i); }
            void reset(SizeType left, SizeType right) { _reset(_root_get(), m_size, left, right); }
            void flip() { _root()->flip(m_size); }
            void flip(SizeType i) { _flip(_root_get(), m_size, i); }
            void flip(SizeType left, SizeType right) { _flip(_root_get(), m_size, left, right); }
            SizeType count() const { return _ptr(m_root)->m_sum; }
            SizeType count(SizeType left, SizeType right) const { return _count(m_root, 0, m_size - 1, left, right); }
            bool any() const { return _ptr(m_root)->m_sum; }
            bool any(SizeType left, SizeType right) const { return _any(m_root, 0, m_size - 1, left, right); }
            bool all() const { return _ptr(m_root)->m_sum == m_size; }
            bool all(SizeType left, SizeType right) const { return _all(m_root, 0, m_size - 1, left, right); }
            SizeType first_one() const { return _ptr(m_root)->m_sum ? _first_one(m_root, 0, m_size - 1) : -1; }
            SizeType last_one() const { return _ptr(m_root)->m_sum ? _last_one(m_root, 0, m_size - 1) : -1; }
            SizeType first_zero() const { return _ptr(m_root)->m_sum < m_size ? _first_zero(m_root, 0, m_size - 1) : -1; }
            SizeType last_zero() const { return _ptr(m_root)->m_sum < m_size ? _last_zero(m_root, 0, m_size - 1) : -1; }
            SizeType prev_one(SizeType i) const { return i ? _prev_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_one(SizeType i) const { return i + 1 < m_size ? _next_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType prev_zero(SizeType i) const { return i ? _prev_zero(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_zero(SizeType i) const { return i + 1 < m_size ? _next_zero(m_root, 0, m_size - 1, i) : -1; }
            bool at(SizeType i) const { return _at(m_root, 0, m_size - 1, i); }
            bool operator[](SizeType i) const { return at(i); }
            SizeType kth(SizeType k, SizeType pos = 0) const { return _kth(m_root, 0, m_size - 1, (pos ? count(0, pos - 1) : 0) + k); }
            SizeType longest_ones(SizeType left, SizeType right) const { return _longest(m_root, 0, m_size - 1, left, right).m_max1; }
            SizeType longest_zeros(SizeType left, SizeType right) const { return _longest(m_root, 0, m_size - 1, left, right).m_max0; }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) {
                typename Getter::value_type val;
                return _max_right<Getter>(_root_get(), 0, m_size - 1, left, val, judge);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) {
                typename Getter::value_type val;
                return _min_left<Getter>(_root_get(), 0, m_size - 1, right, val, judge);
            }
            template <typename Callback>
            void enumerate_range(Callback &&call) const {
                SizeType pre = -1;
                _dfs_range(m_root, 0, m_size - 1, pre, call);
                if (~pre) call(pre, m_size - 1);
            }
            template <typename Callback>
            void enumerate_one(Callback &&call) const { _dfs1(m_root, 0, m_size - 1, call); }
            template <typename Callback>
            void enumerate_zero(Callback &&call) const { _dfs0(m_root, 0, m_size - 1, call); }
            tree_type &operator&=(tree_type &rhs) {
                if (this != &rhs) _bit_and(m_root, rhs.m_root, m_size), rhs.m_root = {}, rhs.m_size = {};
                return *this;
            }
            tree_type &operator|=(tree_type &rhs) {
                if (this != &rhs) _bit_or(m_root, rhs.m_root, m_size), rhs.m_root = {}, rhs.m_size = {};
                return *this;
            }
        };
        template <typename Ostream, typename SizeType, bool MaintainLongest, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, Tree<SizeType, MaintainLongest, BufferType> &x) {
            out << "[";
            auto call = [&, i = 0](SizeType pos) mutable {
                if (i++) out << ", ";
                out << pos;
            };
            x.enumerate_one(call);
            return out << "]";
        }
    }
    template <typename SizeType, bool MaintainLongest = false>
    using VectorLazyBitset = LazyBitset::Tree<SizeType, MaintainLongest>;
}

#endif