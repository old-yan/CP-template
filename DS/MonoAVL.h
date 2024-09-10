/*
最后修改:
20240909
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOAVL__
#define __OY_MONOAVL__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace MONOAVL {
        using size_type = uint32_t;
        struct VoidInfo {};
        template <typename Tp>
        struct NoOpMonoid {
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
        template <typename Ostream, typename Tp, typename = void>
        struct Has_ostream : std::false_type {};
        template <typename Ostream, typename Tp>
        struct Has_ostream<Ostream, Tp, void_t<decltype(std::declval<Ostream>() << std::declval<Tp>())>> : std::true_type {};
        template <typename Monoid, bool MaintainReverse, size_type MAX_NODE>
        class Tree {
        public:
            using tree_type = Tree<Monoid, MaintainReverse, MAX_NODE>;
            using monoid = Monoid;
            using value_type = typename Monoid::value_type;
            using sum_type = typename Has_Sum_Type<Monoid, value_type>::type;
            static constexpr bool has_op = Has_Op<Monoid, sum_type>::value, has_reversed = has_op && Has_Reversed<Monoid, sum_type>::value;
            struct node : NodeWrapper<Monoid, value_type, typename std::conditional<has_op, sum_type, void>::type, has_op && !has_reversed, MaintainReverse>::template type<node> {
                size_type m_hi, m_sz, m_lc, m_rc;
                bool is_null() const { return this == s_buf; }
                node *lchild() const { return s_buf + m_lc; }
                node *rchild() const { return s_buf + m_rc; }
                void _reverse() {
                    static_assert(MaintainReverse, "MaintainReverse Must Be True");
                    this->m_rev_flag = !this->m_rev_flag;
                    if constexpr (has_op)
                        if constexpr (has_reversed)
                            this->m_sum = Monoid::reversed(this->m_sum);
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
                    size_type lc = s_buf[*x].m_lc;
                    s_buf[lc]._pushdown(), s_buf[*x].m_lc = s_buf[lc].m_rc, s_buf[lc].m_rc = *x, s_buf[*x]._pushup_all(), s_buf[lc]._pushup_all(), *x = lc;
                }
                static void rotate_l(size_type *x) {
                    size_type rc = s_buf[*x].m_rc;
                    s_buf[rc]._pushdown(), s_buf[*x].m_rc = s_buf[rc].m_lc, s_buf[rc].m_lc = *x, s_buf[*x]._pushup_all(), s_buf[rc]._pushup_all(), *x = rc;
                }
                static void balance(size_type *x) {
                    size_type lhi = s_buf[*x].lchild()->m_hi, rhi = s_buf[*x].rchild()->m_hi;
                    if (lhi == rhi + 2) {
                        s_buf[*x].lchild()->_pushdown();
                        if (s_buf[*x].lchild()->lchild()->m_hi < s_buf[*x].lchild()->rchild()->m_hi) rotate_l(&s_buf[*x].m_lc);
                        rotate_r(x);
                    } else if (lhi + 2 == rhi) {
                        s_buf[*x].rchild()->_pushdown();
                        if (s_buf[*x].rchild()->lchild()->m_hi > s_buf[*x].rchild()->rchild()->m_hi) rotate_r(&s_buf[*x].m_rc);
                        rotate_l(x);
                    } else
                        s_buf[*x]._pushup_all();
                }
                static size_type join(size_type x, size_type rt, size_type y) {
                    int dif = s_buf[x].m_hi - s_buf[y].m_hi;
                    if (-1 <= dif && dif <= 1) {
                        s_buf[rt].m_lc = x, s_buf[rt].m_rc = y, s_buf[rt]._pushup_all();
                        return rt;
                    } else if (dif > 0) {
                        s_buf[x]._pushdown(), s_buf[x].m_rc = join(s_buf[x].m_rc, rt, y), balance(&x);
                        return x;
                    } else {
                        s_buf[y]._pushdown(), s_buf[y].m_lc = join(x, rt, s_buf[y].m_lc), balance(&y);
                        return y;
                    }
                }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const node &x) {
                    // if constexpr (Has_ostream<Ostream &, NodeWrapper<node>>::value)
                    //     return out << NodeWrapper<node>(x);
                    // else
                    return out << x.m_val;
                }
            };
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= s_buf[x].lchild()->m_sz) return true;
                    m_rank -= s_buf[x].lchild()->m_sz + 1;
                    return false;
                }
            };
            static node s_buf[MAX_NODE + 1];
        private:
            static size_type s_cnt;
            size_type m_rt{};
            static size_type _newnode(value_type val) {
                s_buf[s_cnt].m_val = val, s_buf[s_cnt].m_sz = 1, s_buf[s_cnt].m_hi = 1;
                return s_cnt++;
            }
            template <typename Judger>
            static void _insert(size_type *rt, size_type x, Judger &&judge) {
                if (!*rt) return s_buf[*rt = x]._pushup();
                s_buf[*rt]._pushdown();
                if (judge(*rt))
                    _insert(&s_buf[*rt].m_lc, x, judge);
                else
                    _insert(&s_buf[*rt].m_rc, x, judge);
                node::balance(rt);
            }
            static void _remove_rightest(size_type *rt, size_type &tmp) {
                s_buf[*rt]._pushdown();
                if (s_buf[*rt].m_rc)
                    _remove_rightest(&s_buf[*rt].m_rc, tmp), node::balance(rt);
                else
                    tmp = *rt, *rt = s_buf[*rt].m_lc;
            }
            static void _erase_by_rank(size_type *rt, size_type k) {
                if (!*rt) return;
                s_buf[*rt]._pushdown();
                if (k < s_buf[*rt].lchild()->m_sz) {
                    _erase_by_rank(&s_buf[*rt].m_lc, k);
                    node::balance(rt);
                } else if (k -= s_buf[*rt].lchild()->m_sz) {
                    _erase_by_rank(&s_buf[*rt].m_rc, k - 1);
                    node::balance(rt);
                } else if (!s_buf[*rt].m_lc)
                    *rt = s_buf[*rt].m_rc;
                else {
                    size_type tmp;
                    _remove_rightest(&s_buf[*rt].m_lc, tmp);
                    s_buf[tmp].m_lc = s_buf[*rt].m_lc, s_buf[tmp].m_rc = s_buf[*rt].m_rc, *rt = tmp, node::balance(rt);
                }
            }
            static void _modify_by_rank(size_type rt, size_type k, value_type val) {
                s_buf[rt]._pushdown();
                if (k < s_buf[rt].lchild()->m_sz)
                    _modify_by_rank(s_buf[rt].m_lc, k, val);
                else if (k -= s_buf[rt].lchild()->m_sz)
                    _modify_by_rank(s_buf[rt].m_rc, k - 1, val);
                else
                    s_buf[rt].m_val = val;
                s_buf[rt]._pushup();
            }
            template <typename NodeCallback, typename TreeCallback>
            static void _do_for_subtree_inplace(size_type rt, size_type l, size_type r, NodeCallback &&node_call, TreeCallback &&tree_call) {
                if (!l && r == s_buf[rt].m_sz - 1)
                    tree_call(s_buf + rt);
                else {
                    s_buf[rt]._pushdown();
                    size_type lsz = s_buf[rt].lchild()->m_sz;
                    if (l < lsz) _do_for_subtree_inplace(s_buf[rt].m_lc, l, std::min(r, lsz - 1), node_call, tree_call);
                    if (l <= lsz && lsz <= r) node_call(s_buf + rt);
                    if (r > lsz) _do_for_subtree_inplace(s_buf[rt].m_rc, l <= lsz ? 0 : l - lsz - 1, r - lsz - 1, node_call, tree_call);
                    s_buf[rt]._pushup();
                }
            }
            template <typename Judger>
            static void _split(size_type rt, size_type *x, size_type *y, Judger &&judge) {
                if (!rt)
                    *x = *y = 0;
                else {
                    s_buf[rt]._pushdown();
                    if (judge(rt)) {
                        size_type tmp;
                        _split(s_buf[rt].m_lc, x, &tmp, judge);
                        *y = node::join(tmp, rt, s_buf[rt].m_rc);
                    } else {
                        size_type tmp;
                        _split(s_buf[rt].m_rc, &tmp, y, judge);
                        *x = node::join(s_buf[rt].m_lc, rt, tmp);
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
            template <typename Judger>
            static size_type _min_left(size_type rt, sum_type &&val, Judger &&judge) {
                s_buf[rt]._pushdown();
                size_type lsz = s_buf[rt].lchild()->m_sz;
                if (s_buf[rt].m_rc) {
                    auto a = Monoid::op(s_buf[rt].rchild()->m_sum, val);
                    if (!judge(a)) return lsz + 1 + _min_left(s_buf[rt].m_rc, std::move(val), judge);
                    val = a;
                }
                val = Monoid::op(s_buf[rt].m_val, val);
                if (!judge(val)) return lsz + 1;
                if (!s_buf[rt].m_lc) return 0;
                return _min_left(s_buf[rt].m_lc, std::move(val), judge);
            }
            template <typename Judger>
            static size_type _max_right(size_type rt, sum_type &&val, Judger &&judge) {
                s_buf[rt]._pushdown();
                size_type lsz = s_buf[rt].lchild()->m_sz;
                if (lsz) {
                    auto a = Monoid::op(val, s_buf[rt].lchild()->m_sum);
                    if (!judge(a)) return _max_right(s_buf[rt].m_lc, std::move(val), judge);
                    val = a;
                }
                val = Monoid::op(val, s_buf[rt].m_val);
                if (!judge(val)) return lsz - 1;
                if (!s_buf[rt].m_rc) return lsz;
                return lsz + 1 + _max_right(s_buf[rt].m_rc, std::move(val), judge);
            }
            static size_type _kth(size_type rt, size_type k) {
                s_buf[rt]._pushdown();
                if (k < s_buf[rt].lchild()->m_sz) return _kth(s_buf[rt].m_lc, k);
                if (k -= s_buf[rt].lchild()->m_sz) return _kth(s_buf[rt].m_rc, k - 1);
                return rt;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                s_buf[rt]._pushdown();
                size_type lc = s_buf[rt].m_lc, rc = s_buf[rt].m_rc;
                if (lc) _do_for_each(lc, call);
                call(s_buf + rt);
                if (rc) _do_for_each(rc, call);
            }
            template <typename InitMapping>
            static void _from_mapping(size_type *rt, size_type left, size_type right, InitMapping &&mapping) {
                if (left == right) return;
                if (left + 1 == right) return s_buf[*rt = _newnode(mapping(left))]._pushup();
                size_type mid = left + (right - left) / 2, lc;
                _from_mapping(&lc, left, mid, mapping), *rt = _newnode(mapping(mid)), s_buf[*rt].m_lc = lc, _from_mapping(&s_buf[*rt].m_rc, mid + 1, right, mapping), s_buf[*rt]._pushup_all();
            }
        public:
            template <typename InitMapping>
            static tree_type from_mapping(size_type length, InitMapping mapping) {
                tree_type res;
                _from_mapping(&res.m_rt, 0, length, mapping);
                return res;
            }
            template <typename Iterator>
            static tree_type from_sorted(Iterator first, Iterator last) {
                return from_mapping(last - first, [&](size_type i) { return *(first + i); });
            }
            Tree() {
                if constexpr (has_op) {
                    s_buf->m_val = Monoid::identity(), s_buf->m_sum = Monoid::identity();
                    if constexpr (MaintainReverse && !has_reversed) s_buf->m_sum_rev = s_buf->m_sum;
                }
            }
            Tree(VoidInfo) {}
            node *root() const { return s_buf + m_rt; }
            bool empty() const { return !m_rt; }
            size_type size() const { return s_buf[m_rt].m_sz; }
            void clear() { m_rt = 0; }
            void insert_node_by_rank(node *ptr, size_type k) { ptr->m_hi = ptr->m_sz = 1, _insert(&m_rt, ptr - s_buf, RankJudger(k)); }
            template<typename Judger>
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
                sum_type res = Monoid::identity();
                auto node_call = [&](node *p) { res = Monoid::op(res, p->m_val); };
                auto tree_call = [&](node *p) { res = Monoid::op(res, p->m_sum); };
                _do_for_subtree_inplace(m_rt, left, right, node_call, tree_call);
                return res;
            }
            sum_type query_all() const { return root()->m_sum; }
            tree_type split(size_type pos) {
                tree_type other{{}};
                _split(m_rt, &m_rt, &other.m_rt, RankJudger(pos));
                return other;
            }
            void join(tree_type other) { _join(&m_rt, other.m_rt); }
            node *kth(size_type k) const { return s_buf + _kth(m_rt, k); }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                if (right == size() - 1) return _min_left(m_rt, Monoid::identity(), judge);
                tree_type other = split(right + 1);
                size_type res = _min_left(m_rt, Monoid::identity(), judge);
                join(other);
                return res;
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                if (!left) return _max_right(m_rt, Monoid::identity(), judge);
                tree_type other = split(left);
                size_type res = _max_right(other.m_rt, Monoid::identity(), judge);
                join(other);
                return left + res;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_rt) _do_for_each(m_rt, call);
            }
        };
        template <typename Ostream, typename Monoid, bool MaintainReverse, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Monoid, MaintainReverse, MAX_NODE> &x) {
            out << "{";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
        template <typename Monoid, bool MaintainReverse, size_type MAX_NODE>
        typename Tree<Monoid, MaintainReverse, MAX_NODE>::node Tree<Monoid, MaintainReverse, MAX_NODE>::s_buf[MAX_NODE + 1];
        template <typename Monoid, bool MaintainReverse, size_type MAX_NODE>
        size_type Tree<Monoid, MaintainReverse, MAX_NODE>::s_cnt = 1;
    }
    template <typename Tp, bool MaintainReverse, MONOAVL::size_type MAX_NODE>
    using MonoAVLSequence = MONOAVL::Tree<MONOAVL::NoOpMonoid<Tp>, MaintainReverse, MAX_NODE>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoMaxAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, Minimum, MONOAVL::ChoiceByCompare<Tp, std::less<Tp>>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoMinAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, Maximum, MONOAVL::ChoiceByCompare<Tp, std::greater<Tp>>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoGcdAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, 0, MONOAVL::FpTransfer<Tp, std::gcd<Tp>>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoLcmAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, 1, MONOAVL::FpTransfer<Tp, std::lcm<Tp>>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, Tp OneMask = Tp(-1), bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoBitAndAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoBitOrAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename Tp, Tp ZeroMask = 0, bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoBitXorAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
    template <typename ValueType, typename SumType, ValueType Zero = ValueType(), bool MaintainReverse = true, MONOAVL::size_type MAX_NODE = 1 << 20>
    using MonoSumAVL = MONOAVL::Tree<MONOAVL::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>, MONOAVL::Self>, MaintainReverse, MAX_NODE>;
}

#endif