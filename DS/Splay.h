/*
最后修改:
20231002
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SPLAY__
#define __OY_SPLAY__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace Splay {
        using size_type = uint32_t;
        using state_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename Compare = std::less<Tp>>
        struct BaseNodeWrapper {
            template <typename Node>
            struct type {
                using key_type = Tp;
                key_type m_key;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
            };
        };
        template <typename Tp, typename Operation, typename Compare = std::less<Tp>>
        struct CustomNodeWrapper {
            template <typename Node>
            struct type {
                using key_type = Tp;
                static Operation s_op;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                key_type m_key, m_info;
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
                void pushup(Node *lchild, Node *rchild) { m_info = lchild->is_null() ? (rchild->is_null() ? get() : s_op(get(), rchild->m_info)) : (rchild->is_null() ? s_op(lchild->m_info, get()) : s_op(s_op(lchild->m_info, get()), rchild->m_info)); }
            };
        };
        template <typename Tp, typename Operation, typename Compare>
        template <typename Node>
        Operation CustomNodeWrapper<Tp, Operation, Compare>::type<Node>::s_op;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        struct CustomLazyNodeWrapper {
            template <typename Node>
            struct type {
                using key_type = Tp;
                static Operation s_op;
                static Mapping s_map;
                static Composition s_com;
                static ModifyType s_default_modify;
                key_type m_key, m_info;
                ModifyType m_modify;
                void set(const key_type &key) {
                    m_key = key;
                    if constexpr (InitClearLazy) m_modify = s_default_modify;
                }
                const key_type &get() const { return m_key; }
                void modify(const ModifyType &modify) {
                    m_key = s_map(modify, m_key, 1);
                    m_info = s_map(modify, m_info, ((Node *)this)->m_size);
                    m_modify = s_com(modify, m_modify);
                }
                void pushup(Node *lchild, Node *rchild) { m_info = lchild->is_null() ? (rchild->is_null() ? get() : s_op(get(), rchild->m_info)) : (rchild->is_null() ? s_op(lchild->m_info, get()) : s_op(s_op(lchild->m_info, get()), rchild->m_info)); }
                void pushdown(Node *lchild, Node *rchild) {
                    if (!lchild->is_null()) lchild->modify(m_modify);
                    if (!rchild->is_null()) rchild->modify(m_modify);
                    m_modify = s_default_modify;
                };
            };
        };
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Operation CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_op;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Mapping CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_map;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Composition CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_com;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        ModifyType CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_default_modify;
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
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename Fp, typename = void>
        struct Has_comp : std::false_type {};
        template <typename Tp, typename Fp>
        struct Has_comp<Tp, Fp, void_t<decltype(Tp::comp(std::declval<Fp>(), std::declval<Fp>()))>> : std::true_type {};
        template <typename Ostream, typename Tp, typename = void>
        struct Has_ostream : std::false_type {};
        template <typename Ostream, typename Tp>
        struct Has_ostream<Ostream, Tp, void_t<decltype(std::declval<Ostream>() << std::declval<Tp>())>> : std::true_type {};
        template <template <typename> typename NodeWrapper, size_type MAX_NODE = 1 << 20>
        struct Multiset {
            struct node : NodeWrapper<node> {
                size_type m_size, m_parent, m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const node &x) {
                    if constexpr (Has_ostream<Ostream &, NodeWrapper<node>>::value)
                        return out << NodeWrapper<node>(x);
                    else
                        return out << x.get();
                }
            };
            using key_type = typename node::key_type;
            using tree_type = Multiset<NodeWrapper, MAX_NODE>;
            struct ValueLessJudger {
                const key_type &m_key;
                ValueLessJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return _comp(m_key, s_buffer[x].get()); }
            };
            struct ValueLessEqualJudger {
                const key_type &m_key;
                ValueLessEqualJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return !_comp(s_buffer[x].get(), m_key); }
            };
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= s_buffer[x].lchild()->m_size) return true;
                    m_rank -= s_buffer[x].lchild()->m_size + 1;
                    return false;
                }
            };
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            size_type m_root{};
            template <typename Modify = Ignore>
            static size_type _create(const key_type &key, Modify &&modify = Modify()) {
                s_buffer[s_use_count].set(key);
                s_buffer[s_use_count].m_size = 1;
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(s_buffer + s_use_count);
                return s_use_count++;
            }
            static bool _comp(const key_type &x, const key_type &y) {
                if constexpr (Has_comp<node, key_type>::value)
                    return node::comp(x, y);
                else
                    return x < y;
            }
            static void _update_size(size_type x) { s_buffer[x].m_size = s_buffer[x].lchild()->m_size + s_buffer[x].rchild()->m_size + 1; }
            static void _pushdown(size_type x) {
                if constexpr (Has_pushdown<node, node *>::value) s_buffer[x].pushdown(s_buffer[x].lchild(), s_buffer[x].rchild());
            }
            static void _pushup(size_type x) {
                _update_size(x);
                if constexpr (Has_pushup<node, node *>::value) s_buffer[x].pushup(s_buffer[x].lchild(), s_buffer[x].rchild());
            }
            static void _set_lchild(size_type x, size_type c) { s_buffer[x].m_lchild = c, s_buffer[c].m_parent = x; }
            static void _set_lchild_null(size_type x) { s_buffer[x].m_lchild = 0; }
            static void _set_lchild_or(size_type x, size_type c) {
                if (c)
                    _set_lchild(x, c);
                else
                    _set_lchild_null(x);
            }
            static void _set_rchild(size_type x, size_type c) { s_buffer[x].m_rchild = c, s_buffer[c].m_parent = x; }
            static void _set_rchild_null(size_type x) { s_buffer[x].m_rchild = 0; }
            static void _set_rchild_or(size_type x, size_type c) {
                if (c)
                    _set_rchild(x, c);
                else
                    _set_rchild_null(x);
            }
            static void _rotate_r(size_type *x) {
                size_type lc = s_buffer[*x].m_lchild;
                _set_lchild_or(*x, s_buffer[lc].m_rchild), _pushup(*x), _set_rchild(lc, *x), *x = lc;
            }
            static void _rotate_l(size_type *x) {
                size_type rc = s_buffer[*x].m_rchild;
                _set_rchild_or(*x, s_buffer[rc].m_lchild), _pushup(*x), _set_lchild(rc, *x), *x = rc;
            }
            static void _rotate_rr(size_type *x) {
                size_type lc = s_buffer[*x].m_lchild, llc = s_buffer[lc].m_lchild;
                _set_lchild_or(*x, s_buffer[lc].m_rchild), _pushup(*x), _set_rchild(lc, *x), _set_lchild_or(lc, s_buffer[llc].m_rchild), _pushup(lc), _set_rchild(llc, lc), *x = llc;
            }
            static void _rotate_ll(size_type *x) {
                size_type rc = s_buffer[*x].m_rchild, rrc = s_buffer[rc].m_rchild;
                _set_rchild_or(*x, s_buffer[rc].m_lchild), _pushup(*x), _set_lchild(rc, *x), _set_rchild_or(rc, s_buffer[rrc].m_lchild), _pushup(rc), _set_lchild(rrc, rc), *x = rrc;
            }
            static void _rotate_lr(size_type *x) {
                size_type lc = s_buffer[*x].m_lchild, lrc = s_buffer[lc].m_rchild;
                _set_rchild_or(lc, s_buffer[lrc].m_lchild), _pushup(lc), _set_lchild_or(*x, s_buffer[lrc].m_rchild), _pushup(*x), _set_lchild(lrc, lc), _set_rchild(lrc, *x), *x = lrc;
            }
            static void _rotate_rl(size_type *x) {
                size_type rc = s_buffer[*x].m_rchild, rlc = s_buffer[rc].m_lchild;
                _set_lchild_or(rc, s_buffer[rlc].m_rchild), _pushup(rc), _set_rchild_or(*x, s_buffer[rlc].m_lchild), _pushup(*x), _set_lchild(rlc, *x), _set_rchild(rlc, rc), *x = rlc;
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
                        _insert(&s_buffer[*rt].m_lchild, x, judger, state), _update_by_left(rt, state);
                    else
                        _insert(&s_buffer[*rt].m_rchild, x, judger, state), _update_by_right(rt, state);
                }
            }
            template <typename Judger>
            static bool _splay_by_key(size_type *rt, Judger &&judger, state_type &state) {
                _pushdown(*rt);
                if (judger(*rt)) {
                    if (!s_buffer[*rt].m_lchild) return state = 1, true;
                    if (_splay_by_key(&s_buffer[*rt].m_lchild, judger, state))
                        _update_by_left(rt, state);
                    else {
                        if (state != 1) {
                            _rotate_l(&s_buffer[*rt].m_lchild);
                            s_buffer[*rt].lchild()->m_parent = *rt, state = 1;
                        }
                        _pushup(s_buffer[*rt].m_lchild);
                    }
                    return true;
                } else {
                    if (!s_buffer[*rt].m_rchild) return state = 1, false;
                    bool res = _splay_by_key(&s_buffer[*rt].m_rchild, judger, state);
                    _update_by_right(rt, state);
                    return res;
                }
            }
            static void _kth(size_type *rt, size_type k, state_type &state) {
                _pushdown(*rt);
                size_type lsize = s_buffer[*rt].lchild()->m_size;
                if (k < lsize)
                    _kth(&s_buffer[*rt].m_lchild, k, state), _update_by_left(rt, state);
                else if (k > lsize)
                    _kth(&s_buffer[*rt].m_rchild, k - lsize - 1, state), _update_by_right(rt, state);
                else
                    state = 1;
            }
            static bool _smaller_bound(size_type *rt, const key_type &key, state_type &state) {
                _pushdown(*rt);
                if (_comp(s_buffer[*rt].get(), key)) {
                    if (!s_buffer[*rt].m_rchild) return state = 1, true;
                    if (_smaller_bound(&s_buffer[*rt].m_rchild, key, state))
                        _update_by_right(rt, state);
                    else {
                        if (state != 1) {
                            _rotate_r(&s_buffer[*rt].m_rchild);
                            s_buffer[*rt].rchild()->m_parent = *rt;
                            state = 1;
                        }
                        _pushup(s_buffer[*rt].m_rchild);
                    }
                    return true;
                } else {
                    if (!s_buffer[*rt].m_lchild) return state = 1, false;
                    bool res = _smaller_bound(&s_buffer[*rt].m_lchild, key, state);
                    _update_by_left(rt, state);
                    return res;
                }
            }
            static bool _splay_max(size_type *rt) {
                _pushdown(*rt);
                if (!s_buffer[*rt].m_rchild) return true;
                if (_splay_max(&s_buffer[*rt].m_rchild)) return false;
                return _rotate_ll(rt), true;
            }
            static void _join(size_type *x, size_type y) {
                if (!_splay_max(x)) _rotate_l(x);
                _set_rchild_or(*x, y);
            }
            template <typename Func>
            static void _merge(size_type *x, size_type y, Func &&func, state_type &state) {
                if (!*x) return (void)(state = 1, *x = y);
                if (!y) return (void)(state = 1);
                if (s_buffer[*x].m_size > s_buffer[y].m_size) std::swap(*x, y);
                state_type state2 = 0;
                bool res = _splay_by_key(&y, ValueLessEqualJudger(s_buffer[*x].get()), state2);
                _fetch(&y, state2), _pushup(y);
                if (res) {
                    if (s_buffer[y].m_lchild)
                        if (s_buffer[*x].m_lchild)
                            _merge(&s_buffer[*x].m_lchild, s_buffer[y].m_lchild, func, state), _pushup(s_buffer[*x].m_lchild);
                        else
                            _set_lchild(*x, s_buffer[y].m_lchild);
                    state = 0;
                    if constexpr (std::is_same<typename std::decay<Func>::type, Ignore>::value)
                        _set_lchild_null(y), _pushup(y), _merge(&s_buffer[*x].m_rchild, y, func, state);
                    else if (_comp(s_buffer[*x].get(), s_buffer[y].get()))
                        _set_lchild_null(y), _pushup(y), _merge(&s_buffer[*x].m_rchild, y, func, state);
                    else
                        func(s_buffer + *x, s_buffer + y), _merge(&s_buffer[*x].m_rchild, s_buffer[y].m_rchild, func, state);
                    if (s_buffer[*x].m_rchild) _pushup(s_buffer[*x].m_rchild);
                    _update_by_right(x, state);
                } else if (s_buffer[*x].m_lchild)
                    _merge(&s_buffer[*x].m_lchild, y, func, state), _pushup(s_buffer[*x].m_lchild), _update_by_left(x, state);
                else
                    _set_lchild(*x, y), state = 1;
            }
            template <typename Judge>
            static void _max_right(size_type *rt, Judge &&judge, state_type &state) {
                if (!*rt) return;
                _pushdown(*rt);
                if (!judge.try_lchild(s_buffer + *rt))
                    _max_right(&s_buffer[*rt].m_lchild, judge, state), _update_by_left(rt, state);
                else if (judge.try_mid(s_buffer + *rt))
                    _max_right(&s_buffer[*rt].m_rchild, judge, state), _update_by_right(rt, state);
                else
                    state = 1;
            }
            template <typename Judge>
            static void _min_left(size_type *rt, Judge &&judge, state_type &state) {
                if (!*rt) return;
                _pushdown(*rt);
                if (!judge.try_rchild(s_buffer + *rt))
                    _min_left(&s_buffer[*rt].m_rchild, judge, state), _update_by_right(rt, state);
                else if (judge.try_mid(s_buffer + *rt))
                    _min_left(&s_buffer[*rt].m_lchild, judge, state), _update_by_left(rt, state);
                else
                    state = 1;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _pushdown(rt);
                if (s_buffer[rt].m_lchild) _do_for_each(s_buffer[rt].m_lchild, call);
                call(s_buffer + rt);
                if (s_buffer[rt].m_rchild) _do_for_each(s_buffer[rt].m_rchild, call);
            }
            static void _fetch(size_type *rt, state_type &state) {
                if (state == 2)
                    _rotate_r(rt);
                else if (state == 3)
                    _rotate_l(rt);
            }
            void _fetch_and_update(state_type &state) const { _fetch_root(state), _update_root(); }
            void _fetch_root(state_type &state) const { _fetch((size_type *)&m_root, state); }
            void _update_root() const { s_buffer[m_root].m_parent = 0, _pushup(m_root); }
            template <typename Iterator>
            static void _from_sorted(size_type *rt, Iterator first, Iterator last) {
                if (first != last) {
                    Iterator mid = first + (last - first) / 2;
                    *rt = _create(*mid);
                    _from_sorted(&s_buffer[*rt].m_lchild, first, mid);
                    _from_sorted(&s_buffer[*rt].m_rchild, mid + 1, last);
                    _pushup(*rt);
                }
            }
            template <typename Iterator>
            static tree_type from_sorted(Iterator first, Iterator last) {
                tree_type res;
                _from_sorted(&res.m_root, first, last);
                return res;
            }
            void clear() { m_root = 0; }
            template <typename Modify = Ignore>
            void insert_by_key(const key_type &key, Modify &&modify = Modify()) {
                state_type state = 0;
                _insert(&m_root, _create(key, modify), ValueLessJudger(key), state), _fetch_and_update(state);
            }
            template <typename Modify = Ignore>
            void insert_by_rank(const key_type &key, size_type k, Modify &&modify = Modify()) {
                state_type state = 0;
                _insert(&m_root, _create(key, modify), RankJudger(k), state), _fetch_and_update(state);
            }
            bool erase_by_key(const key_type &key) {
                if (!m_root) return false;
                state_type state = 0;
                bool res = _splay_by_key(&m_root, ValueLessEqualJudger(key), state);
                _fetch_root(state);
                if (!res || _comp(key, s_buffer[m_root].get())) return _update_root(), false;
                if (!s_buffer[m_root].m_lchild)
                    m_root = s_buffer[m_root].m_rchild, s_buffer[m_root].m_parent = 0;
                else {
                    if (s_buffer[m_root].m_rchild) {
                        if (!_splay_max(&s_buffer[m_root].m_lchild)) _rotate_l(&s_buffer[m_root].m_lchild);
                        _set_rchild(s_buffer[m_root].m_lchild, s_buffer[m_root].m_rchild), m_root = s_buffer[m_root].m_lchild, _update_root();
                    } else
                        m_root = s_buffer[m_root].m_lchild, s_buffer[m_root].m_parent = 0;
                }
                return true;
            }
            void erase_by_rank(size_type k) {
                state_type state = 0;
                _kth(&m_root, k, state), _fetch_root(state);
                if (!s_buffer[m_root].m_lchild)
                    m_root = s_buffer[m_root].m_rchild, s_buffer[m_root].m_parent = 0;
                else {
                    if (s_buffer[m_root].m_rchild) {
                        if (!_splay_max(&s_buffer[m_root].m_lchild)) _rotate_l(&s_buffer[m_root].m_lchild);
                        _set_rchild(s_buffer[m_root].m_lchild, s_buffer[m_root].m_rchild), m_root = s_buffer[m_root].m_lchild, _update_root();
                    } else
                        m_root = s_buffer[m_root].m_lchild, s_buffer[m_root].m_parent = 0;
                }
            }
            template <typename Modify>
            bool modify_by_key(const key_type &key, Modify &&modify) {
                if (!m_root) return false;
                state_type state = 0;
                bool res = _splay_by_key(&m_root, ValueLessEqualJudger(key), state);
                _fetch_root(state);
                if (!res || _comp(key, s_buffer[m_root].get())) return _update_root(), false;
                modify(s_buffer + m_root);
                return _update_root(), true;
            }
            template <typename Modify>
            void modify_by_rank(size_type k, Modify &&modify) {
                state_type state = 0;
                _kth(&m_root, RankJudger(k), state), _fetch_root(state), modify(s_buffer + m_root), _update_root();
            }
            tree_type split_by_key(const key_type &key) {
                tree_type other;
                if (!m_root) return other;
                state_type state = 0;
                bool res = _splay_by_key(&m_root, ValueLessEqualJudger(key), state);
                _fetch_root(state);
                if (!res) return _update_root(), other;
                other.m_root = m_root, s_buffer[m_root = s_buffer[m_root].m_lchild].m_parent = 0, _set_lchild_null(other.m_root), other._update_root();
                return other;
            }
            tree_type split_by_rank(size_type k) {
                tree_type other;
                if (!k) return std::swap(m_root, other.m_root), other;
                if (k == s_buffer[m_root].m_size) return other;
                state_type state = 0;
                _kth(&m_root, k - 1, state), _fetch_root(state), s_buffer[other.m_root = s_buffer[m_root].m_rchild].m_parent = 0, _set_rchild_null(m_root), _update_root();
                return other;
            }
            void join(tree_type other) {
                if (!other.m_root) return;
                if (!m_root) return (void)(m_root = other.m_root);
                _join(&m_root, other.m_root), _update_root();
            }
            template <typename Func = Ignore>
            void merge(tree_type other, Func &&func = Func()) {
                state_type state = 0;
                _merge(&m_root, other.m_root, func, state), _fetch_and_update(state);
            }
            node *root() const { return s_buffer + m_root; }
            size_type size() const { return s_buffer[m_root].m_size; }
            node *kth(size_type k) const {
                state_type state = 0;
                _kth((size_type *)&m_root, k, state), _fetch_and_update(state);
                return s_buffer + m_root;
            }
            size_type rank(const key_type &key) const {
                if (!m_root) return 0;
                state_type state = 0;
                bool res = _splay_by_key((size_type *)&m_root, ValueLessEqualJudger(key), state);
                _fetch_and_update(state);
                return res ? s_buffer[m_root].lchild()->m_size : s_buffer[m_root].m_size;
            }
            node *smaller_bound(const key_type &key) const {
                if (!m_root) return s_buffer;
                state_type state = 0;
                bool res = _smaller_bound((size_type *)&m_root, key, state);
                _fetch_and_update(state);
                return res ? s_buffer + m_root : s_buffer;
            }
            node *lower_bound(const key_type &key) const {
                if (!m_root) return s_buffer;
                state_type state = 0;
                bool res = _splay_by_key((size_type *)&m_root, ValueLessEqualJudger(key), state);
                _fetch_and_update(state);
                return res ? s_buffer + m_root : s_buffer;
            }
            node *upper_bound(const key_type &key) const {
                if (!m_root) return s_buffer;
                state_type state = 0;
                bool res = _splay_by_key((size_type *)&m_root, ValueLessJudger(key), state);
                _fetch_and_update(state);
                return res ? s_buffer + m_root : s_buffer;
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judger) {
                tree_type other = split_by_rank(left);
                state_type state = 0;
                _max_right(&other.m_root, judger, state), other._fetch_and_update(state);
                size_type res = s_buffer[other.m_root].lchild()->m_size;
                return join(other), left + res - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judger) {
                tree_type other = split_by_rank(right + 1);
                state_type state = 0;
                _min_left(&m_root, judger, state), _fetch_and_update(state);
                size_type res = s_buffer[m_root].lchild()->m_size;
                return join(other), res + 1;
            }
            template <typename Callback>
            void do_for_subtree(size_type left, size_type right, Callback &&call) {
                if (!left) {
                    if (right + 1 == s_buffer[m_root].m_size)
                        call(s_buffer + m_root), _pushup(m_root);
                    else {
                        state_type state = 0;
                        _kth(&m_root, right + 1, state), _fetch_root(state), call(s_buffer[m_root].lchild()), _pushup(s_buffer[m_root].m_lchild), _update_root();
                    }
                } else if (right + 1 == s_buffer[m_root].m_size) {
                    state_type state = 0;
                    _kth(&m_root, left - 1, state), _fetch_root(state), call(s_buffer[m_root].rchild()), _pushup(s_buffer[m_root].m_rchild), _update_root();
                } else {
                    state_type state = 0;
                    _kth(&m_root, right + 1, state), _fetch_root(state);
                    state = 0, _kth(&s_buffer[m_root].m_lchild, left - 1, state), _fetch(&s_buffer[m_root].m_lchild, state);
                    s_buffer[m_root].lchild()->m_parent = m_root, call(s_buffer[m_root].lchild()->rchild()), _pushup(s_buffer[m_root].lchild()->m_rchild), _pushup(s_buffer[m_root].m_lchild), _update_root();
                }
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                if (m_root) _do_for_each(m_root, call);
            }
        };
        template <typename Ostream, template <typename> typename NodeWrapper, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Multiset<NodeWrapper, MAX_NODE> &x) {
            out << "{";
            for (size_type i = 0; i < x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
        template <template <typename> typename NodeWrapper, size_type MAX_NODE>
        typename Multiset<NodeWrapper, MAX_NODE>::node Multiset<NodeWrapper, MAX_NODE>::s_buffer[MAX_NODE];
        template <template <typename> typename NodeWrapper, size_type MAX_NODE>
        size_type Multiset<NodeWrapper, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp, typename Compare = std::less<Tp>, Splay::size_type MAX_NODE = 1 << 20, typename Operation, typename TreeType = Splay::Multiset<Splay::CustomNodeWrapper<Tp, Operation, Compare>::template type, MAX_NODE>>
    auto make_Splay(Operation op) -> TreeType { return TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, Splay::size_type MAX_NODE = 1 << 20, typename TreeType = Splay::Multiset<Splay::CustomNodeWrapper<Tp, const Tp &(*)(const Tp &, const Tp &), Compare>::template type, MAX_NODE>>
    auto make_Splay(const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, Splay::size_type MAX_NODE = 1 << 20, typename TreeType = Splay::Multiset<Splay::CustomNodeWrapper<Tp, Tp (*)(Tp, Tp), Compare>::template type, MAX_NODE>>
    auto make_Splay(Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename ModifyType, bool InitClearLazy, typename Compare = std::less<Tp>, Splay::size_type MAX_NODE = 1 << 20, typename Operation, typename Mapping, typename Composition, typename TreeType = Splay::Multiset<Splay::CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::template type, MAX_NODE>>
    auto make_lazy_Splay(Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, Splay::size_type MAX_NODE = 1 << 20>
    using SplayTree = Splay::Multiset<Splay::BaseNodeWrapper<Tp, Compare>::template type, MAX_NODE>;
}

#endif