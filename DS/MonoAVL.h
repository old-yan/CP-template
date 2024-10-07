/*
最后修改:
20240920
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOAVL__
#define __OY_MONOAVL__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace MONOAVL {
        using size_type = uint32_t;
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
                size_type m_hi, m_sz, m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
                void _reverse() {
                    static_assert(MaintainReverse, "MaintainReverse Must Be True");
                    this->m_rev_flag = !this->m_rev_flag;
                    if constexpr (has_op)
                        if constexpr (has_reversed)
                            this->m_sum = group::reversed(this->m_sum);
                        else
                            std::swap(this->m_sum, this->m_sum_rev);
                }
                void _pushdown() {
                    if constexpr (MaintainReverse)
                        if (this->m_rev_flag) {
                            if (!lchild()->is_null()) lchild()->_reverse();
                            if (!rchild()->is_null()) rchild()->_reverse();
                            std::swap(m_lc, m_rc), this->m_rev_flag = false;
                        }
                }
                void _pushup() {
                    if constexpr (has_op) this->pushup(lchild(), rchild());
                }
                void _pushup_all() { m_hi = std::max(lchild()->m_hi, rchild()->m_hi) + 1, m_sz = lchild()->m_sz + rchild()->m_sz + 1, _pushup(); }
                static void rotate_r(size_type *x) {
                    size_type lc = _ptr(*x)->m_lc;
                    _ptr(lc)->_pushdown(), _ptr(*x)->m_lc = _ptr(lc)->m_rc, _ptr(lc)->m_rc = *x, _ptr(*x)->_pushup_all(), _ptr(lc)->_pushup_all(), *x = lc;
                }
                static void rotate_l(size_type *x) {
                    size_type rc = _ptr(*x)->m_rc;
                    _ptr(rc)->_pushdown(), _ptr(*x)->m_rc = _ptr(rc)->m_lc, _ptr(rc)->m_lc = *x, _ptr(*x)->_pushup_all(), _ptr(rc)->_pushup_all(), *x = rc;
                }
                static void balance(size_type *x) {
                    size_type lhi = _ptr(*x)->lchild()->m_hi, rhi = _ptr(*x)->rchild()->m_hi;
                    if (lhi == rhi + 2) {
                        _ptr(*x)->lchild()->_pushdown();
                        if (_ptr(*x)->lchild()->lchild()->m_hi < _ptr(*x)->lchild()->rchild()->m_hi) rotate_l(&_ptr(*x)->m_lc);
                        rotate_r(x);
                    } else if (lhi + 2 == rhi) {
                        _ptr(*x)->rchild()->_pushdown();
                        if (_ptr(*x)->rchild()->lchild()->m_hi > _ptr(*x)->rchild()->rchild()->m_hi) rotate_r(&_ptr(*x)->m_rc);
                        rotate_l(x);
                    } else
                        _ptr(*x)->_pushup_all();
                }
                static size_type join(size_type x, size_type rt, size_type y) {
                    int dif = _ptr(x)->m_hi - _ptr(y)->m_hi;
                    if (-1 <= dif && dif <= 1) {
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y, _ptr(rt)->_pushup_all();
                        return rt;
                    } else if (dif > 0) {
                        _ptr(x)->_pushdown(), _ptr(x)->m_rc = join(_ptr(x)->m_rc, rt, y), balance(&x);
                        return x;
                    } else {
                        _ptr(y)->_pushdown(), _ptr(y)->m_lc = join(x, rt, _ptr(y)->m_lc), balance(&y);
                        return y;
                    }
                }
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
                _ptr(x)->m_val = val, _ptr(x)->m_sz = 1, _ptr(x)->m_hi = 1;
                return x;
            }
            template <typename Judger>
            static void _insert(size_type *rt, size_type x, Judger &&judge) {
                if (!*rt) return _ptr(*rt = x)->_pushup();
                _ptr(*rt)->_pushdown();
                if (judge(*rt))
                    _insert(&_ptr(*rt)->m_lc, x, judge);
                else
                    _insert(&_ptr(*rt)->m_rc, x, judge);
                node::balance(rt);
            }
            static void _remove_rightest(size_type *rt, size_type &tmp) {
                _ptr(*rt)->_pushdown();
                if (_ptr(*rt)->m_rc)
                    _remove_rightest(&_ptr(*rt)->m_rc, tmp), node::balance(rt);
                else
                    tmp = *rt, *rt = _ptr(*rt)->m_lc;
            }
            static void _erase_by_rank(size_type *rt, size_type k) {
                if (!*rt) return;
                _ptr(*rt)->_pushdown();
                if (k < _ptr(*rt)->lchild()->m_sz) {
                    _erase_by_rank(&_ptr(*rt)->m_lc, k);
                    node::balance(rt);
                } else if (k -= _ptr(*rt)->lchild()->m_sz) {
                    _erase_by_rank(&_ptr(*rt)->m_rc, k - 1);
                    node::balance(rt);
                } else if (!_ptr(*rt)->m_lc) {
                    size_type rc = _ptr(*rt)->m_rc;
                    _collect(*rt), *rt = rc;
                } else {
                    size_type tmp;
                    _remove_rightest(&_ptr(*rt)->m_lc, tmp);
                    _ptr(tmp)->m_lc = _ptr(*rt)->m_lc, _ptr(tmp)->m_rc = _ptr(*rt)->m_rc, _collect(*rt), *rt = tmp;
                    node::balance(rt);
                }
            }
            static void _modify_by_rank(size_type rt, size_type k, value_type val) {
                _ptr(rt)->_pushdown();
                if (k < _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_lc, k, val);
                else if (k -= _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_rc, k - 1, val);
                else
                    _ptr(rt)->m_val = val;
                _ptr(rt)->_pushup();
            }
            template <typename NodeCallback, typename TreeCallback>
            static void _do_for_subtree_inplace(size_type rt, size_type l, size_type r, NodeCallback &&node_call, TreeCallback &&tree_call) {
                if (!l && r == _ptr(rt)->m_sz - 1)
                    tree_call(_ptr(rt));
                else {
                    _ptr(rt)->_pushdown();
                    size_type lsz = _ptr(rt)->lchild()->m_sz;
                    if (l < lsz) _do_for_subtree_inplace(_ptr(rt)->m_lc, l, std::min(r, lsz - 1), node_call, tree_call);
                    if (l <= lsz && lsz <= r) node_call(_ptr(rt));
                    if (r > lsz) _do_for_subtree_inplace(_ptr(rt)->m_rc, l <= lsz ? 0 : l - lsz - 1, r - lsz - 1, node_call, tree_call);
                    _ptr(rt)->_pushup();
                }
            }
            template <typename Judger>
            static void _split(size_type rt, size_type *x, size_type *y, Judger &&judge) {
                if (!rt)
                    *x = *y = 0;
                else {
                    _ptr(rt)->_pushdown();
                    if (judge(rt)) {
                        size_type tmp;
                        _split(_ptr(rt)->m_lc, x, &tmp, judge);
                        *y = node::join(tmp, rt, _ptr(rt)->m_rc);
                    } else {
                        size_type tmp;
                        _split(_ptr(rt)->m_rc, &tmp, y, judge);
                        *x = node::join(_ptr(rt)->m_lc, rt, tmp);
                    }
                }
            }
            static void _join(size_type *x, size_type y) {
                if (!*x)
                    *x = y;
                else if (y) {
                    size_type tmp;
                    _remove_rightest(x, tmp);
                    *x = node::join(*x, tmp, y);
                }
            }
            template <typename Compare, typename Func>
            static void _merge(size_type &rt, size_type y, Compare &&comp, Func &&func) {
                if (!rt || !y) return (void)(rt |= y);
                _ptr(rt)->_pushdown(), _ptr(y)->_pushdown();
                size_type l, m, r;
                _split(y, &l, &r, [&](size_type i) { return comp(_ptr(rt)->m_val, _ptr(i)->m_val); });
                if constexpr (!std::is_same<typename std::decay<Func>::type, VoidInfo>::value) {
                    _split(l, &l, &m, [&](size_type i) { return !comp(_ptr(i)->m_val, _ptr(rt)->m_val); });
                    if (m) func(_ptr(rt), _ptr(m)), _collect(m);
                }
                _merge(_ptr(rt)->m_lc, l, comp, func), _merge(_ptr(rt)->m_rc, r, comp, func);
                rt = node::join(_ptr(rt)->m_lc, rt, _ptr(rt)->m_rc);
            }
            template <typename Judger>
            static size_type _min_left(size_type rt, sum_type &&val, Judger &&judge) {
                _ptr(rt)->_pushdown();
                size_type lsz = _ptr(rt)->lchild()->m_sz;
                if (_ptr(rt)->m_rc) {
                    auto a = group::op(_ptr(rt)->rchild()->m_sum, val);
                    if (!judge(a)) return lsz + 1 + _min_left(_ptr(rt)->m_rc, std::move(val), judge);
                    val = a;
                }
                val = group::op(_ptr(rt)->m_val, val);
                if (!judge(val)) return lsz + 1;
                if (!_ptr(rt)->m_lc) return 0;
                return _min_left(_ptr(rt)->m_lc, std::move(val), judge);
            }
            template <typename Judger>
            static size_type _max_right(size_type rt, sum_type &&val, Judger &&judge) {
                _ptr(rt)->_pushdown();
                size_type lsz = _ptr(rt)->lchild()->m_sz;
                if (lsz) {
                    auto a = group::op(val, _ptr(rt)->lchild()->m_sum);
                    if (!judge(a)) return _max_right(_ptr(rt)->m_lc, std::move(val), judge);
                    val = a;
                }
                val = group::op(val, _ptr(rt)->m_val);
                if (!judge(val)) return lsz - 1;
                if (!_ptr(rt)->m_rc) return lsz;
                return lsz + 1 + _max_right(_ptr(rt)->m_rc, std::move(val), judge);
            }
            template <typename Compare>
            static iterator _lower_bound(size_type rt, const value_type &val, Compare &&comp) {
                if (!rt) return {0, _ptr(0)};
                _ptr(rt)->_pushdown();
                if (comp(_ptr(rt)->m_val, val)) {
                    iterator res = _lower_bound(_ptr(rt)->m_rc, val, comp);
                    res.m_rank += _ptr(rt)->lchild()->m_sz + 1;
                    return res;
                }
                iterator res = _lower_bound(_ptr(rt)->m_lc, val, comp);
                return res.m_ptr->is_null() ? iterator{_ptr(rt)->lchild()->m_sz, _ptr(rt)} : res;
            }
            static size_type _kth(size_type rt, size_type k) {
                _ptr(rt)->_pushdown();
                if (k < _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_lc, k);
                if (k -= _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_rc, k - 1);
                return rt;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _ptr(rt)->_pushdown();
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
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc, _ptr(x)->_pushup_all();
                return x;
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
            void insert_node_by_rank(node *ptr, size_type k) { ptr->m_hi = ptr->m_sz = 1, _insert(&m_rt, ptr - _ptr(0), RankJudger(k)); }
            template <typename Judger>
            void insert_by_judger(value_type val, Judger &&judge) { _insert(&m_rt, _newnode(val), judge); }
            void insert(size_type pos, value_type val) { _insert(&m_rt, _newnode(val), RankJudger(pos)); }
            void erase(size_type pos) { _erase_by_rank(&m_rt, pos); }
            void modify(size_type pos, value_type val) { _modify_by_rank(m_rt, pos, val); }
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
            value_type query(size_type i) const { return kth(i)->m_val; }
            sum_type query(size_type left, size_type right) const {
                sum_type res = group::identity();
                auto node_call = [&](node *p) { res = group::op(res, p->m_val); };
                auto tree_call = [&](node *p) { res = group::op(res, p->m_sum); };
                _do_for_subtree_inplace(m_rt, left, right, node_call, tree_call);
                return res;
            }
            sum_type query_all() const { return root()->m_sum; }
            template <typename Compare = std::less<value_type>>
            void insert_by_comparator(value_type val, Compare comp = Compare()) {
                insert_by_judger(val, [&](size_type x) { return comp(val, _ptr(x)->m_val); });
            }
            template <typename Compare = std::less<value_type>>
            void erase_by_comparator(value_type val, Compare comp = Compare()) {
                iterator res = find_by_comparator(val, comp);
                if (!res.m_ptr->is_null()) _erase_by_rank(&m_rt, res.m_rank);
            }
            template <typename Compare = std::less<value_type>>
            iterator find_by_comparator(value_type val, Compare comp = Compare()) {
                iterator res = _lower_bound(m_rt, val, comp);
                return !res.m_ptr->is_null() && !comp(val, res.m_ptr->m_val) ? res : iterator{size_type(-1), _ptr(0)};
            }
            template <typename Compare = std::less<value_type>>
            iterator lower_bound_by_comparator(value_type val, Compare comp = Compare()) { return _lower_bound(m_rt, val, comp); }
            template <typename Compare = std::less<value_type>>
            iterator upper_bound_by_comparator(value_type val, Compare comp = Compare()) {
                return _lower_bound(m_rt, val, [&](value_type x, value_type y) { return !comp(y, x); });
            }
            template <typename Compare = std::less<value_type>, typename Func = VoidInfo>
            void merge_by_comparator(tree_type &rhs, Compare comp = Compare(), Func &&func = Func()) {
                if (empty())
                    m_rt = rhs.m_rt, rhs.m_rt = 0;
                else
                    _merge(m_rt, rhs.m_rt, comp, func), rhs.m_rt = 0;
            }
            template <typename Compare = std::less<value_type>, typename Func = VoidInfo>
            void merge_by_comparator(tree_type &&rhs, Compare comp = Compare(), Func &&func = Func()) { merge_by_comparator(rhs, comp, func); }
            tree_type split(size_type pos) {
                tree_type other;
                _split(m_rt, &m_rt, &other.m_rt, RankJudger(pos));
                return other;
            }
            void join(tree_type &other) { _join(&m_rt, other.m_rt), other.m_rt = 0; }
            void join(tree_type &&other) { join(other); }
            node *kth(size_type k) const { return _ptr(_kth(m_rt, k)); }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                if (right == size() - 1) return _min_left(m_rt, group::identity(), judge);
                tree_type other = split(right + 1);
                size_type res = _min_left(m_rt, group::identity(), judge);
                join(other);
                return res;
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                if (!left) return _max_right(m_rt, group::identity(), judge);
                tree_type other = split(left);
                size_type res = _max_right(other.m_rt, group::identity(), judge);
                join(other);
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
    using MonoAVLSequence = MONOAVL::Tree<MONOAVL::NoOp<Tp>, MaintainReverse, BufferType>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), bool MaintainReverse = true>
    using VectorMonoMaxAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, Minimum, MONOAVL::ChoiceByCompare<Tp, std::less<Tp>>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), bool MaintainReverse = true>
    using VectorMonoMinAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, Maximum, MONOAVL::ChoiceByCompare<Tp, std::greater<Tp>>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, bool MaintainReverse = true>
    using VectorMonoGcdAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, 0, MONOAVL::FpTransfer<Tp, std::gcd<Tp>>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, bool MaintainReverse = true>
    using VectorMonoLcmAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, 1, MONOAVL::FpTransfer<Tp, std::lcm<Tp>>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, Tp OneMask = Tp(-1), bool MaintainReverse = true>
    using VectorMonoBitAndAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true>
    using VectorMonoBitOrAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>, MONOAVL::Self>, MaintainReverse>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true>
    using VectorMonoBitXorAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>, MONOAVL::Self>, MaintainReverse>;
    template <typename ValueType, typename SumType, ValueType Zero = ValueType(), bool MaintainReverse = true>
    using VectorMonoSumAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>, MONOAVL::Self>, MaintainReverse>;
}

#endif