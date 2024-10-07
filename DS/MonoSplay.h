/*
最后修改:
20241005
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOSPLAY__
#define __OY_MONOSPLAY__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace MONOSPLAY {
        using size_type = uint32_t;
        using state_type = uint32_t;
        struct VoidInfo {};
        template <typename Tp>
        struct NoOp {
            using value_type = Tp;
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
            static sum_type op(const sum_type &x, const sum_type &y) { return Operation()(x, y); }
            static sum_type reversed(const sum_type &x) { return Reversed()(x); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename Monoid, typename value_type, typename sum_type, bool Rev, bool Flag>
        struct NodeWrapper {
            template <typename Node>
            struct type {
                value_type m_val;
                sum_type m_sum;
                const sum_type &get_sum() const { return m_sum; }
                void pushup(Node *lchild, Node *rchild) { m_sum = Monoid::op(Monoid::op(lchild->m_sum, m_val), rchild->m_sum); }
            };
        };
        template <typename Monoid, typename value_type, typename sum_type>
        struct NodeWrapper<Monoid, value_type, sum_type, false, true> {
            template <typename Node>
            struct type {
                bool m_rev_flag;
                value_type m_val;
                sum_type m_sum;
                const sum_type &get_sum() const { return m_sum; }
                sum_type get_sum_rev() const { return Monoid::reversed(m_sum); }
                void pushup(Node *lchild, Node *rchild) { m_sum = Monoid::op(Monoid::op(lchild->m_sum, m_val), rchild->m_sum); }
            };
        };
        template <typename Monoid, typename value_type, typename sum_type>
        struct NodeWrapper<Monoid, value_type, sum_type, true, true> {
            template <typename Node>
            struct type {
                bool m_rev_flag;
                value_type m_val;
                sum_type m_sum, m_sum_rev;
                const sum_type &get_sum() const { return m_sum; }
                const sum_type &get_sum_rev() const { return m_sum_rev; }
                void pushup(Node *lchild, Node *rchild) { m_sum = Monoid::op(Monoid::op(lchild->m_sum, m_val), rchild->m_sum), m_sum_rev = Monoid::op(Monoid::op(rchild->m_sum_rev, m_val), lchild->m_sum_rev); }
            };
        };
        template <typename Monoid, typename value_type>
        struct NodeWrapper<Monoid, value_type, void, false, true> {
            template <typename Node>
            struct type {
                bool m_rev_flag;
                value_type m_val;
            };
        };
        template <typename Monoid, typename value_type>
        struct NodeWrapper<Monoid, value_type, void, false, false> {
            template <typename Node>
            struct type {
                value_type m_val;
            };
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
        template <typename Monoid, bool MaintainReverse, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using group = Monoid;
            using tree_type = Tree<group, MaintainReverse, BufferType>;
            using value_type = typename group::value_type;
            using sum_type = typename Has_Sum_Type<group, value_type>::type;
            static constexpr bool has_op = Has_Op<group, sum_type>::value, has_reversed = has_op && Has_Reversed<group, sum_type>::value;
            struct node : NodeWrapper<group, value_type, typename std::conditional<has_op, sum_type, void>::type, has_op && !has_reversed, MaintainReverse>::template type<node> {
                size_type m_sz, m_lc, m_rc, m_fa;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
                node *parent() const { return _ptr(m_fa); }
                void _reverse() {
                    static_assert(MaintainReverse, "MaintainReverse Must Be True");
                    this->m_rev_flag = !this->m_rev_flag;
                    if constexpr (has_op)
                        if constexpr (has_reversed)
                            this->m_sum = group::reversed(this->m_sum);
                        else
                            std::swap(this->m_sum, this->m_sum_rev);
                }
                void _pushup() {
                    if constexpr (has_op) this->pushup(lchild(), rchild());
                }
                void _pushup_all() { m_sz = lchild()->m_sz + rchild()->m_sz + 1, _pushup(); }
            };
            struct iterator {
                size_type m_rank;
                node *m_ptr;
            };
            using buffer_type = BufferType<node>;
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= _ptr(x)->lchild()->m_sz) return true;
                    m_rank -= _ptr(x)->lchild()->m_sz + 1;
                    return false;
                }
            };
            struct Initializer {
                Initializer() {
                    if constexpr (has_op) {
                        _ptr(0)->m_val = group::identity(), _ptr(0)->m_sum = group::identity();
                        if constexpr (MaintainReverse && !has_reversed) _ptr(0)->m_sum_rev = _ptr(0)->m_sum;
                    }
                }
            };
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _collect(size_type x) { _ptr(x)->m_lc = _ptr(x)->m_rc = 0, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            static size_type _newnode(value_type val) {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_val = val, _ptr(x)->m_sz = 1;
                return x;
            }
            static void _pushdown(size_type x) {
                if constexpr (MaintainReverse) {
                    node *p = _ptr(x);
                    if (p->m_rev_flag) {
                        if (!p->lchild()->is_null()) p->lchild()->_reverse();
                        if (!p->rchild()->is_null()) p->rchild()->_reverse();
                        std::swap(p->m_lc, p->m_rc), p->m_rev_flag = false;
                    }
                }
            }
            static void _set_lc(size_type p, size_type c) { _ptr(p)->m_lc = c, _ptr(c)->m_fa = p; }
            static void _set_rc(size_type p, size_type c) { _ptr(p)->m_rc = c, _ptr(c)->m_fa = p; }
            static void _set_lc0(size_type p) { _ptr(p)->m_lc = 0; }
            static void _set_rc0(size_type p) { _ptr(p)->m_rc = 0; }
            static void _set_lc1(size_type p, size_type c) { c ? _set_lc(p, c) : _set_lc0(p); }
            static void _set_rc1(size_type p, size_type c) { c ? _set_rc(p, c) : _set_rc0(p); }
            static void _rotate_r(size_type *x) {
                size_type lc = _ptr(*x)->m_lc;
                _set_lc1(*x, _ptr(lc)->m_rc), _ptr(*x)->_pushup_all(), _set_rc(lc, *x), *x = lc;
            }
            static void _rotate_l(size_type *x) {
                size_type rc = _ptr(*x)->m_rc;
                _set_rc1(*x, _ptr(rc)->m_lc), _ptr(*x)->_pushup_all(), _set_lc(rc, *x), *x = rc;
            }
            static void _rotate_rr(size_type *x) {
                size_type lc = _ptr(*x)->m_lc, llc = _ptr(lc)->m_lc;
                _set_lc1(*x, _ptr(lc)->m_rc), _ptr(*x)->_pushup_all(), _set_rc(lc, *x), _set_lc1(lc, _ptr(llc)->m_rc), _ptr(lc)->_pushup_all(), _set_rc(llc, lc), *x = llc;
            }
            static void _rotate_ll(size_type *x) {
                size_type rc = _ptr(*x)->m_rc, rrc = _ptr(rc)->m_rc;
                _set_rc1(*x, _ptr(rc)->m_lc), _ptr(*x)->_pushup_all(), _set_lc(rc, *x), _set_rc1(rc, _ptr(rrc)->m_lc), _ptr(rc)->_pushup_all(), _set_lc(rrc, rc), *x = rrc;
            }
            static void _rotate_lr(size_type *x) {
                size_type lc = _ptr(*x)->m_lc, lrc = _ptr(lc)->m_rc;
                _set_rc1(lc, _ptr(lrc)->m_lc), _ptr(lc)->_pushup_all(), _set_lc1(*x, _ptr(lrc)->m_rc), _ptr(*x)->_pushup_all(), _set_lc(lrc, lc), _set_rc(lrc, *x), *x = lrc;
            }
            static void _rotate_rl(size_type *x) {
                size_type rc = _ptr(*x)->m_rc, rlc = _ptr(rc)->m_lc;
                _set_lc1(rc, _ptr(rlc)->m_rc), _ptr(rc)->_pushup_all(), _set_rc1(*x, _ptr(rlc)->m_lc), _ptr(*x)->_pushup_all(), _set_lc(rlc, *x), _set_rc(rlc, rc), *x = rlc;
            }
            static void _update_by_left(size_type *rt, state_type &state) {
                state <<= 1;
                if (state == 4)
                    state = 1, _rotate_rr(rt);
                else if (state == 6)
                    state = 1, _rotate_lr(rt);
            }
            static void _update_by_right(size_type *rt, state_type &state) {
                state = state << 1 | 1;
                if (state == 5)
                    state = 1, _rotate_rl(rt);
                else if (state == 7)
                    state = 1, _rotate_ll(rt);
            }
            template <typename Judger>
            static void _insert(size_type *rt, size_type x, Judger &&judger, state_type &state) {
                if (!*rt)
                    *rt = x, state = 1;
                else {
                    _pushdown(*rt);
                    if (judger(*rt))
                        _insert(&_ptr(*rt)->m_lc, x, judger, state), _update_by_left(rt, state);
                    else
                        _insert(&_ptr(*rt)->m_rc, x, judger, state), _update_by_right(rt, state);
                }
            }
            template <typename Judger>
            static bool _splay_by_key(size_type *rt, Judger &&judger, state_type &state) {
                _pushdown(*rt);
                if (judger(_ptr(*rt)->m_val)) {
                    if (!_ptr(*rt)->m_lc) return state = 1;
                    if (_splay_by_key(&_ptr(*rt)->m_lc, judger, state))
                        _update_by_left(rt, state);
                    else {
                        if (state != 1) {
                            _rotate_l(&_ptr(*rt)->m_lc);
                            _ptr(*rt)->lchild()->m_fa = *rt, state = 1;
                        }
                        _ptr(*rt)->lchild()->_pushup_all();
                    }
                    return true;
                } else {
                    if (!_ptr(*rt)->m_rc) return state = 1, false;
                    bool res = _splay_by_key(&_ptr(*rt)->m_rc, judger, state);
                    _update_by_right(rt, state);
                    return res;
                }
            }
            static void _kth(size_type *rt, size_type k, state_type &state) {
                _pushdown(*rt);
                size_type lsz = _ptr(*rt)->lchild()->m_sz;
                if (k < lsz)
                    _kth(&_ptr(*rt)->m_lc, k, state), _update_by_left(rt, state);
                else if (k > lsz)
                    _kth(&_ptr(*rt)->m_rc, k - lsz - 1, state), _update_by_right(rt, state);
                else
                    state = 1;
            }
            static bool _splay_max(size_type *rt) {
                _pushdown(*rt);
                if (!_ptr(*rt)->m_rc) return true;
                if (_splay_max(&_ptr(*rt)->m_rc)) return false;
                return _rotate_ll(rt), true;
            }
            static void _join(size_type *x, size_type y) {
                if (!_splay_max(x)) _rotate_l(x);
                _set_rc1(*x, y);
            }
            template <typename Compare, typename Func>
            static void _merge(size_type &rt, size_type y, Compare &&comp, Func &&func, state_type &state) {
                if (rt) _pushdown(rt);
                if (y) _pushdown(y);
                if (!rt || !y) return state = (rt || y), (void)(rt |= y);
                if (_ptr(rt)->m_sz > _ptr(y)->m_sz) std::swap(rt, y);
                state_type sub = 0;
                bool res = _splay_by_key(&y, [&](value_type val) { return !comp(val, _ptr(rt)->m_val); }, sub);
                _fetch(&y, sub);
                if (!res)
                    _merge(_ptr(rt)->m_lc, y, comp, func, state), _update_by_left(&rt, state);
                else if (comp(_ptr(rt)->m_val, _ptr(y)->m_val) || std::is_same<typename std::decay<Func>::type, VoidInfo>::value) {
                    size_type ylc = _ptr(y)->m_lc;
                    _ptr(ylc)->m_fa = _ptr(y)->m_lc = 0;
                    _merge(_ptr(rt)->m_lc, ylc, comp, func, state);
                    if (_ptr(rt)->m_lc) _ptr(rt)->lchild()->_pushup_all(), _set_lc(rt, _ptr(rt)->m_lc), _update_by_left(&rt, state);
                    _merge(_ptr(rt)->m_rc, y, comp, func, state = 0), _update_by_right(&rt, state);
                } else {
                    if constexpr (!std::is_same<typename std::decay<Func>::type, VoidInfo>::value) func(_ptr(rt), _ptr(y));
                    _ptr(y)->lchild()->m_fa = 0, _merge(_ptr(rt)->m_lc, _ptr(y)->m_lc, comp, func, state);
                    if (_ptr(rt)->m_lc) _ptr(rt)->lchild()->_pushup_all(), _set_lc(rt, _ptr(rt)->m_lc), _update_by_left(&rt, state);
                    _ptr(y)->rchild()->m_fa = 0, _merge(_ptr(rt)->m_rc, _ptr(y)->m_rc, comp, func, state = 0), _update_by_right(&rt, state);
                    _collect(y);
                }
            }
            template <typename Judger>
            static size_type _max_right(size_type *rt, sum_type &&val, Judger &&judge, state_type &state) {
                _pushdown(*rt);
                size_type lsz = _ptr(*rt)->lchild()->m_sz;
                if (lsz) {
                    auto a = group::op(val, _ptr(*rt)->lchild()->m_sum);
                    if (!judge(a)) {
                        size_type res = _max_right(&_ptr(*rt)->m_lc, std::move(val), judge, state);
                        return _update_by_left(rt, state), res;
                    }
                    val = a;
                }
                val = group::op(val, _ptr(*rt)->m_val);
                if (!judge(val)) return state = 1, lsz - 1;
                if (!_ptr(*rt)->m_rc) return state = 1, lsz;
                size_type res = lsz + 1 + _max_right(&_ptr(*rt)->m_rc, std::move(val), judge, state);
                return _update_by_right(rt, state), res;
            }
            template <typename Judger>
            static size_type _min_left(size_type *rt, sum_type &&val, Judger &&judge, state_type &state) {
                _pushdown(*rt);
                size_type lsz = _ptr(*rt)->lchild()->m_sz;
                if (_ptr(*rt)->m_rc) {
                    auto a = group::op(_ptr(*rt)->rchild()->m_sum, val);
                    if (!judge(a)) {
                        size_type res = lsz + 1 + _min_left(&_ptr(*rt)->m_rc, std::move(val), judge, state);
                        return _update_by_right(rt, state), res;
                    }
                    val = a;
                }
                val = group::op(_ptr(*rt)->m_val, val);
                if (!judge(val)) return state = 1, lsz + 1;
                if (!_ptr(*rt)->m_lc) return state = 1, 0;
                size_type res = _min_left(&_ptr(*rt)->m_lc, std::move(val), judge, state);
                return _update_by_left(rt, state), res;
            }
            static void _fetch(size_type *rt, state_type &state) {
                if (state == 2)
                    _rotate_r(rt);
                else if (state == 3)
                    _rotate_l(rt);
            }
            void _fetch_and_update(state_type &state) const { _fetch_root(state), _update_root(); }
            void _fetch_root(state_type &state) const { _fetch((size_type *)&m_rt, state); }
            void _update_root() const { _ptr(m_rt)->m_fa = 0, _ptr(m_rt)->_pushup_all(); }
            void _erase_root() {
                if (!_ptr(m_rt)->m_lc) {
                    size_type rc = _ptr(m_rt)->m_rc;
                    _collect(m_rt), m_rt = rc, _ptr(m_rt)->m_fa = 0;
                } else {
                    if (_ptr(m_rt)->m_rc) {
                        if (!_splay_max(&_ptr(m_rt)->m_lc)) _rotate_l(&_ptr(m_rt)->m_lc);
                        _set_rc(_ptr(m_rt)->m_lc, _ptr(m_rt)->m_rc);
                        size_type lc = _ptr(m_rt)->m_lc;
                        _collect(m_rt), m_rt = lc, _update_root();
                    } else {
                        size_type lc = _ptr(m_rt)->m_lc;
                        _collect(m_rt), m_rt = lc, _ptr(m_rt)->m_fa = 0;
                    }
                }
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _pushdown(rt);
                size_type lc = _ptr(rt)->m_lc, rc = _ptr(rt)->m_rc;
                if (lc) _do_for_each(lc, call);
                call(_ptr(rt));
                if (rc) _do_for_each(rc, call);
            }
            template <typename InitMapping>
            static size_type _from_mapping(size_type left, size_type right, InitMapping &&mapping) {
                if (left == right) return 0;
                if (left + 1 == right) {
                    size_type x = _newnode(mapping(left));
                    _ptr(x)->_pushup();
                    return x;
                }
                size_type mid = left + (right - left) / 2, lc = _from_mapping(left, mid, mapping), x = _newnode(mapping(mid)), rc = _from_mapping(mid + 1, right, mapping);
                _set_lc(x, lc), _set_rc(x, rc), _ptr(x)->_pushup_all();
                return x;
            }
            static const node *_trace_up(const node *p, size_type &rk) {
                if (!p->m_fa) return _pushdown(p - _ptr(0)), p;
                auto rt = _trace_up(p->parent(), rk);
                if (p == p->parent()->rchild()) rk += p->parent()->lchild()->m_sz + 1;
                _pushdown(p - _ptr(0));
                return rt;
            }
        public:
            template <typename InitMapping>
            static tree_type from_mapping(size_type length, InitMapping mapping) {
                tree_type res;
                res.m_rt = _from_mapping(0, length, mapping);
                return res;
            }
            template <typename Iterator>
            static tree_type from_sorted(Iterator first, Iterator last) {
                return from_mapping(last - first, [&](size_type i) { return *(first + i); });
            }
            static std::pair<node *, size_type> get_root_and_rank(const node *p) {
                size_type rk{};
                auto rt = _trace_up(p, rk);
                return std::make_pair((node *)rt, rk + p->lchild()->m_sz);
            }
            Tree() { static Initializer _init; }
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt);
                return *this;
            }
            node *root() const { return _ptr(m_rt); }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            bool empty() const { return !m_rt; }
            size_type size() const { return _ptr(m_rt)->m_sz; }
            void insert_node_by_rank(node *ptr, size_type k) {
                state_type state{};
                _insert(&m_rt, ptr - _ptr(0), RankJudger{k}, state), _fetch_and_update(state);
            }
            template <typename Judger>
            void insert_by_judger(value_type val, Judger &&judge) {
                state_type state{};
                _insert(&m_rt, _newnode(val), judge, state), _fetch_and_update(state);
            }
            void insert(size_type pos, value_type val) {
                state_type state{};
                _insert(&m_rt, _newnode(val), RankJudger{pos}, state), _fetch_and_update(state);
            }
            void erase(size_type pos) {
                state_type state = 0;
                _kth((size_type *)&m_rt, pos, state), _fetch_root(state), _erase_root();
            }
            void modify(size_type pos, value_type val) {
                state_type state = 0;
                _kth((size_type *)&m_rt, pos, state), _fetch_root(state), _ptr(m_rt)->m_val = val, _update_root();
            }
            void reverse(size_type left, size_type right) {
                static_assert(MaintainReverse, "MaintainReverse Must Be True");
                tree_type S3 = split(right + 1), S2 = split(left);
                S2.root()->_reverse();
                join(S2), join(S3);
            }
            void reverse() {
                static_assert(MaintainReverse, "MaintainReverse Must Be True");
                root()->_reverse();
            }
            value_type query(size_type pos) const { return kth(pos)->m_val; }
            sum_type query(size_type left, size_type right) const {
                tree_type &self = (tree_type &)(*this);
                tree_type S3 = self.split(right + 1), S2 = self.split(left);
                sum_type res = S2.root()->m_sum;
                self.join(S2), self.join(S3);
                return res;
            }
            sum_type query_all() const { return root()->m_sum; }
            template <typename Compare = std::less<value_type>>
            void insert_by_comparator(value_type val, Compare comp = Compare()) {
                insert_by_judger(val, [&](size_type x) { return comp(val, _ptr(x)->m_val); });
            }
            template <typename Compare = std::less<value_type>>
            void erase_by_comparator(value_type val, Compare comp = Compare()) {
                if (!m_rt) return;
                state_type state = 0;
                bool res = _splay_by_key(&m_rt, [&](value_type x) { return !comp(x, val); }, state);
                _fetch_root(state);
                (res && !comp(val, _ptr(m_rt)->m_val)) ? _erase_root() : _update_root();
            }
            template <typename Compare = std::less<value_type>>
            iterator find_by_comparator(value_type val, Compare comp = Compare()) {
                iterator res = _lower_bound(m_rt, val, comp);
                return !res.m_ptr->is_null() && !comp(val, res.m_ptr->m_val) ? res : iterator{size_type(-1), _ptr(0)};
            }
            template <typename Compare = std::less<value_type>>
            iterator lower_bound_by_comparator(value_type val, Compare comp = Compare()) {
                if (!m_rt) return {0, _ptr(0)};
                state_type state = 0;
                bool res = _splay_by_key(&m_rt, [&](value_type x) { return !comp(x, val); }, state);
                _fetch_and_update(state);
                if (!res) return {_ptr(m_rt)->m_sz, _ptr(0)};
                return {_ptr(m_rt)->lchild()->m_sz, _ptr(m_rt)};
            }
            template <typename Compare = std::less<value_type>>
            iterator upper_bound_by_comparator(value_type val, Compare comp = Compare()) {
                return lower_bound_by_comparator(val, [&](value_type x, value_type y) { return !comp(y, x); });
            }
            template <typename Compare = std::less<value_type>, typename Func = VoidInfo>
            void merge_by_comparator(tree_type &rhs, Compare comp = Compare(), Func &&func = Func()) {
                if (empty())
                    m_rt = rhs.m_rt, rhs.m_rt = 0;
                else {
                    state_type state{};
                    _merge(m_rt, rhs.m_rt, comp, func, state), _fetch_and_update(state), rhs.m_rt = 0;
                }
            }
            template <typename Func = VoidInfo>
            void merge_by_comparator(tree_type &&rhs, Func &&func = Func()) { merge_by_comparator(rhs, func); }
            tree_type split(size_type pos) {
                if (!pos) return std::move(*this);
                if (pos == _ptr(m_rt)->m_sz) return {};
                tree_type other;
                state_type state = 0;
                _kth(&m_rt, pos - 1, state), _fetch_root(state), _ptr(other.m_rt = _ptr(m_rt)->m_rc)->m_fa = 0, _set_rc0(m_rt), _update_root();
                return other;
            }
            void join(tree_type &rhs) {
                if (empty()) return std::swap(m_rt, rhs.m_rt);
                _join(&m_rt, rhs.m_rt), rhs.m_rt = 0, _update_root();
            }
            void join(tree_type &&rhs) { join(rhs); }
            node *kth(size_type k) const {
                state_type state = 0;
                _kth((size_type *)&m_rt, k, state), _fetch_and_update(state);
                return _ptr(m_rt);
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                tree_type &self = (tree_type &)(*this);
                state_type state{};
                if (right == self.size() - 1) {
                    size_type res = _min_left(&self.m_rt, group::identity(), judge, state);
                    return self._fetch_and_update(state), res;
                }
                tree_type other = self.split(right + 1);
                size_type res = _min_left(&self.m_rt, group::identity(), judge, state);
                self._fetch_and_update(state), self.join(other);
                return res;
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                tree_type &self = (tree_type &)(*this);
                state_type state{};
                if (!left) {
                    size_type res = _max_right(&self.m_rt, group::identity(), judge, state);
                    return self._fetch_and_update(state), res;
                }
                tree_type other = self.split(left);
                size_type res = _max_right(&other.m_rt, group::identity(), judge, state);
                other._fetch_and_update(state), self.join(other);
                return left + res;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_rt) _do_for_each(m_rt, call);
            }
        };
        template <typename Ostream, typename Monoid, bool MaintainReverse, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Monoid, MaintainReverse, BufferType> &x) {
            using node = typename Tree<Monoid, MaintainReverse, BufferType>::node;
            out << "{";
            bool started = false;
            auto call = [&](node *p) {
                if (started)
                    out << ", ";
                else
                    started = true;
                out << p->m_val;
            };
            x.enumerate(call);
            return out << "}";
        }
    }
    template <typename Tp, bool MaintainReverse, template <typename> typename BufferType = VectorBufferWithCollect>
    using MonoSplaySequence = MONOSPLAY::Tree<MONOSPLAY::NoOp<Tp>, MaintainReverse, BufferType>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), bool MaintainReverse = true>
    using VectorMonoMaxSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, Minimum, MONOSPLAY::ChoiceByCompare<Tp, std::less<Tp>>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), bool MaintainReverse = true>
    using VectorMonoMinSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, Maximum, MONOSPLAY::ChoiceByCompare<Tp, std::greater<Tp>>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, bool MaintainReverse = true>
    using VectorMonoGcdSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, 0, MONOSPLAY::FpTransfer<Tp, std::gcd<Tp>>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, bool MaintainReverse = true>
    using VectorMonoLcmSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, 1, MONOSPLAY::FpTransfer<Tp, std::lcm<Tp>>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, Tp OneMask = Tp(-1), bool MaintainReverse = true>
    using VectorMonoBitAndSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true>
    using VectorMonoBitOrSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true>
    using VectorMonoBitXorSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>, MONOSPLAY::Self>, MaintainReverse>;
    template <typename ValueType, typename SumType, ValueType Zero = ValueType(), bool MaintainReverse = true>
    using VectorMonoSumSplay = MONOSPLAY::Tree<MONOSPLAY::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>, MONOSPLAY::Self>, MaintainReverse>;
}

#endif