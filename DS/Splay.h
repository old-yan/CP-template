/*
最后修改:
20241007
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

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace SPLAY {
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
                    m_info = s_map(modify, m_info, ((Node *)this)->m_sz);
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
        template <template <typename> typename NodeWrapper, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using tree_type = Tree<NodeWrapper, BufferType>;
            struct node : NodeWrapper<node> {
                using Base = NodeWrapper<node>;
                size_type m_sz, m_lc, m_rc, m_fa;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
                node *parent() const { return _ptr(m_fa); }
                void _pushup() {
                    if constexpr (Has_pushup<node, node *>::value) Base::pushup(lchild(), rchild());
                }
                void _pushup_all() { m_sz = lchild()->m_sz + rchild()->m_sz + 1, _pushup(); }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const node &x) {
                    if constexpr (Has_ostream<Ostream &, NodeWrapper<node>>::value)
                        return out << NodeWrapper<node>(x);
                    else
                        return out << x.get();
                }
            };
            using key_type = typename node::key_type;
            using buffer_type = BufferType<node>;
            struct ValueLessJudger {
                const key_type &m_key;
                ValueLessJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return _comp(m_key, _ptr(x)->get()); }
            };
            struct ValueLessEqualJudger {
                const key_type &m_key;
                ValueLessEqualJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return !_comp(_ptr(x)->get(), m_key); }
            };
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= _ptr(x)->lchild()->m_sz) return true;
                    m_rank -= _ptr(x)->lchild()->m_sz + 1;
                    return false;
                }
            };
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _collect(size_type x) { _ptr(x)->m_lc = _ptr(x)->m_rc = _ptr(x)->m_fa = 0, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            template <typename Modify = Ignore>
            static size_type _newnode(const key_type &key, Modify &&modify = Modify()) {
                size_type x = buffer_type::newnode();
                _ptr(x)->set(key), _ptr(x)->m_sz = 1;
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(x));
                return x;
            }
            static bool _comp(const key_type &x, const key_type &y) {
                if constexpr (Has_comp<node, key_type>::value)
                    return node::comp(x, y);
                else
                    return x < y;
            }
            static void _pushdown(size_type x) {
                if constexpr (Has_pushdown<node, node *>::value) _ptr(x)->pushdown(_ptr(x)->lchild(), _ptr(x)->rchild());
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
                _set_rc1(*x, _ptr(rc)->m_lc),
                    _ptr(*x)->_pushup_all(),
                    _set_lc(rc, *x),
                    _set_rc1(rc, _ptr(rrc)->m_lc),
                    _ptr(rc)->_pushup_all(),
                    _set_lc(rrc, rc),
                    *x = rrc;
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
                if (judger(*rt)) {
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
            template <typename Func>
            static void _merge(size_type &rt, size_type y, Func &&func, state_type &state) {
                if (rt) _pushdown(rt);
                if (y) _pushdown(y);
                if (!rt || !y) return state = (rt || y), (void)(rt |= y);
                if (_ptr(rt)->m_sz > _ptr(y)->m_sz) std::swap(rt, y);
                state_type sub = 0;
                bool res = _splay_by_key(&y, ValueLessEqualJudger(_ptr(rt)->get()), sub);
                _fetch(&y, sub);
                if (!res)
                    _merge(_ptr(rt)->m_lc, y, func, state), _update_by_left(&rt, state);
                else if (_comp(_ptr(rt)->get(), _ptr(y)->get()) || std::is_same<typename std::decay<Func>::type, Ignore>::value) {
                    size_type ylc = _ptr(y)->m_lc;
                    _ptr(ylc)->m_fa = _ptr(y)->m_lc = 0;
                    _merge(_ptr(rt)->m_lc, ylc, func, state);
                    if (_ptr(rt)->m_lc) _ptr(rt)->lchild()->_pushup_all(), _set_lc(rt, _ptr(rt)->m_lc), _update_by_left(&rt, state);
                    _merge(_ptr(rt)->m_rc, y, func, state = 0), _update_by_right(&rt, state);
                } else {
                    if constexpr (!std::is_same<typename std::decay<Func>::type, Ignore>::value) func(_ptr(rt), _ptr(y));
                    _ptr(y)->lchild()->m_fa = 0, _merge(_ptr(rt)->m_lc, _ptr(y)->m_lc, func, state);
                    if (_ptr(rt)->m_lc) _ptr(rt)->lchild()->_pushup_all(), _set_lc(rt, _ptr(rt)->m_lc), _update_by_left(&rt, state);
                    _ptr(y)->rchild()->m_fa = 0, _merge(_ptr(rt)->m_rc, _ptr(y)->m_rc, func, state = 0), _update_by_right(&rt, state);
                    _collect(y);
                }
            }
            template <typename Getter, typename Judger>
            static size_type _max_right(size_type *rt, typename Getter::value_type &&val, Judger &&judge, state_type &state) {
                _pushdown(*rt);
                size_type lsz = _ptr(*rt)->lchild()->m_sz;
                if (lsz) {
                    auto a(val);
                    Getter().tree(a, _ptr(*rt)->lchild());
                    if (!judge(a)) {
                        size_type res = _max_right<Getter>(&_ptr(*rt)->m_lc, std::move(val), judge, state);
                        return _update_by_left(rt, state), res;
                    }
                    val = a;
                }
                Getter().node(val, _ptr(*rt));
                if (!judge(val)) return state = 1, lsz - 1;
                if (!_ptr(*rt)->m_rc) return state = 1, lsz;
                size_type res = lsz + 1 + _max_right<Getter>(&_ptr(*rt)->m_rc, std::move(val), judge, state);
                return _update_by_right(rt, state), res;
            }
            template <typename Getter, typename Judger>
            static size_type _min_left(size_type *rt, typename Getter::value_type &&val, Judger &&judge, state_type &state) {
                _pushdown(*rt);
                size_type lsz = _ptr(*rt)->lchild()->m_sz;
                if (_ptr(*rt)->m_rc) {
                    auto a(val);
                    Getter().tree(_ptr(*rt)->rchild(), a);
                    if (!judge(a)) {
                        size_type res = lsz + 1 + _min_left(&_ptr(*rt)->m_rc, std::move(val), judge, state);
                        return _update_by_right(rt, state), res;
                    }
                    val = a;
                }
                Getter().node(_ptr(*rt), val);
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
            void _update_root() const { root()->m_fa = 0, root()->_pushup_all(); }
            void _erase_root() {
                if (!root()->m_lc || !root()->m_rc) {
                    size_type c = root()->m_lc | root()->m_rc;
                    _collect(m_rt), _ptr(m_rt = c)->m_fa = 0;
                } else {
                    size_type lc = root()->m_lc;
                    _join(&lc, root()->m_rc), _collect(m_rt), m_rt = lc, _update_root();
                }
            }
            bool _lower_bound(const key_type &key) const {
                state_type state = 0;
                bool res = _splay_by_key(&(size_type &)m_rt, ValueLessEqualJudger(key), state);
                return _fetch_root(state), res;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _pushdown(rt);
                size_type lc = _ptr(rt)->m_lc, rc = _ptr(rt)->m_rc;
                if (lc) _do_for_each(lc, call);
                call(_ptr(rt));
                if (rc) _do_for_each(rc, call);
            }
            template <typename InitMapping, typename Modify = Ignore>
            static size_type _from_mapping(size_type left, size_type right, InitMapping &&mapping, Modify &&modify) {
                if (left == right) return 0;
                if (left + 1 == right) {
                    size_type x = _newnode(mapping(left), modify);
                    _ptr(x)->_pushup();
                    return x;
                }
                size_type mid = left + (right - left) / 2, lc = _from_mapping(left, mid, mapping, modify), x = _newnode(mapping(mid), modify), rc = _from_mapping(mid + 1, right, mapping, modify);
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
            template <typename InitMapping, typename Modify = Ignore>
            static tree_type from_mapping(size_type length, InitMapping mapping, Modify &&modify = Modify()) {
                tree_type res;
                res.m_rt = _from_mapping(0, length, mapping, modify);
                return res;
            }
            template <typename Iterator, typename Modify = Ignore>
            static tree_type from_sorted(Iterator first, Iterator last, Modify &&modify = Modify()) {
                return from_mapping(last - first, [&](size_type i) { return *(first + i); }, modify);
            }
            static std::pair<node *, size_type> get_root_and_rank(const node *p) {
                size_type rk{};
                auto rt = _trace_up(p, rk);
                return std::make_pair((node *)rt, rk + p->lchild()->m_sz);
            }
            Tree() = default;
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
            size_type size() const { return root()->m_sz; }
            void insert_node_by_key(node *ptr) {
                state_type state{};
                ptr->m_sz = 1, _insert(&m_rt, ptr - _ptr(0), ValueLessJudger(ptr->get()), state), _fetch_and_update(state);
            }
            void insert_node_by_rank(node *ptr, size_type k) {
                state_type state{};
                ptr->m_sz = 1, _insert(&m_rt, ptr - _ptr(0), RankJudger(k), state), _fetch_and_update(state);
            }
            template <typename Modify = Ignore>
            void insert_by_key(key_type key, Modify &&modify = Modify()) {
                state_type state{};
                _insert(&m_rt, _newnode(key, modify), ValueLessJudger(key), state), _fetch_and_update(state);
            }
            template <typename Modify = Ignore>
            void insert_by_rank(key_type key, size_type k, Modify &&modify = Modify()) {
                state_type state{};
                _insert(&m_rt, _newnode(key, modify), RankJudger(k), state), _fetch_and_update(state);
            }
            bool erase_by_key(const key_type &key) {
                if (!m_rt) return false;
                return (_lower_bound(key) && !_comp(key, root()->get())) ? (_erase_root(), true) : (_update_root(), false);
            }
            void erase_by_rank(size_type k) {
                state_type state = 0;
                _kth((size_type *)&m_rt, k, state), _fetch_root(state), _erase_root();
            }
            template <typename Modify>
            bool modify_by_key(const key_type &key, Modify &&modify) {
                if (!m_rt) return false;
                if (!_lower_bound(key) || _comp(key, root()->get())) return _update_root(), false;
                modify(root()), _update_root();
                return true;
            }
            template <typename Modify>
            void modify_by_rank(size_type k, Modify &&modify) { modify(kth(k)), _update_root(); }
            template <typename Modify>
            bool modify_or_insert(key_type key, Modify &&modify) {
                if (!m_rt) return insert_by_key(key, modify), true;
                bool res = _lower_bound(key);
                if (!res) {
                    auto x = _newnode(key, modify);
                    _update_root(), _set_lc(x, m_rt), m_rt = x, _update_root();
                    return true;
                } else if (_comp(key, root()->get())) {
                    auto x = _newnode(key, modify);
                    _set_lc1(x, root()->m_lc), _set_lc0(m_rt), root()->_pushup_all(), _set_rc(x, m_rt), m_rt = x, _update_root();
                    return true;
                }
                return modify(root()), _update_root(), false;
            }
            tree_type split_by_key(const key_type &key) {
                if (!m_rt) return {};
                if (!_lower_bound(key)) return _update_root(), tree_type{};
                tree_type other;
                other.m_rt = m_rt, _ptr(m_rt = root()->m_lc)->m_fa = 0, _set_lc0(other.m_rt), other._update_root();
                return other;
            }
            tree_type split_by_rank(size_type k) {
                if (!k) return std::move(*this);
                if (k == root()->m_sz) return {};
                tree_type other;
                state_type state = 0;
                _kth(&m_rt, k - 1, state), _fetch_root(state), _ptr(other.m_rt = root()->m_rc)->m_fa = 0, _set_rc0(m_rt), _update_root();
                return other;
            }
            void join(tree_type &rhs) {
                if (empty()) return std::swap(m_rt, rhs.m_rt);
                _join(&m_rt, rhs.m_rt), rhs.m_rt = 0, _update_root();
            }
            void join(tree_type &&rhs) { join(rhs); }
            template <typename Func = Ignore>
            void merge(tree_type &rhs, Func &&func = Func()) {
                if (empty())
                    m_rt = rhs.m_rt, rhs.m_rt = 0;
                else {
                    state_type state{};
                    _merge(m_rt, rhs.m_rt, func, state), _fetch_and_update(state), rhs.m_rt = 0;
                }
            }
            template <typename Func = Ignore>
            void merge(tree_type &&rhs, Func &&func = Func()) { merge(rhs, func); }
            node *kth(size_type k) const {
                state_type state = 0;
                _kth((size_type *)&m_rt, k, state), _fetch_and_update(state);
                return root();
            }
            template <typename Getter, typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                tree_type &self = (tree_type &)(*this);
                state_type state{};
                if (right == self.size() - 1) {
                    size_type res = _min_left<Getter>(&self.m_rt, Getter()(), judge, state);
                    return self._fetch_and_update(state), res;
                }
                tree_type other = self.split_by_rank(right + 1);
                size_type res = _min_left<Getter>(&self.m_rt, Getter()(), judge, state);
                self._fetch_and_update(state), self.join(other);
                return res;
            }
            template <typename Getter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                tree_type &self = (tree_type &)(*this);
                state_type state{};
                if (!left) {
                    size_type res = _max_right<Getter>(&self.m_rt, Getter()(), judge, state);
                    return self._fetch_and_update(state), res;
                }
                tree_type other = self.split_by_rank(left);
                size_type res = _max_right<Getter>(&other.m_rt, Getter()(), judge, state);
                other._fetch_and_update(state), self.join(other);
                return left + res;
            }
            size_type rank(const key_type &key) const {
                if (!m_rt) return 0;
                if (!_lower_bound(key)) return _update_root(), root()->m_sz;
                return _update_root(), root()->lchild()->m_sz;
            }
            node *smaller_bound(const key_type &key) const {
                if (!m_rt) return _ptr(0);
                state_type state = 0;
                if (!_lower_bound(key)) return _update_root(), root();
                size_type lc = root()->m_lc;
                if (!lc) return _ptr(0);
                if (!_splay_max(&lc)) _rotate_l(&lc);
                _set_lc0(m_rt), root()->_pushup_all(), _set_rc(lc, m_rt), (size_type &)m_rt = lc, _update_root();
                return root();
            }
            node *lower_bound(const key_type &key) const {
                if (!m_rt) return _ptr(0);
                if (!_lower_bound(key)) return _update_root(), _ptr(0);
                return _update_root(), root();
            }
            node *upper_bound(const key_type &key) const {
                if (!m_rt) return _ptr(0);
                state_type state = 0;
                bool res = _splay_by_key(&(size_type &)m_rt, ValueLessJudger(key), state);
                return _fetch_and_update(state), _ptr(res ? m_rt : 0);
            }
            template <typename Callback>
            void do_for_subtree(size_type left, size_type right, Callback &&call) {
                tree_type S3 = split_by_rank(right + 1), S2 = split_by_rank(left);
                call(S2.root()), join(S2), join(S3);
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                if (m_rt) _do_for_each(m_rt, call);
            }
        };
        template <typename Ostream, template <typename> typename NodeWrapper, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<NodeWrapper, BufferType> &x) {
            out << "{";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
    }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename TreeType = SPLAY::Tree<SPLAY::CustomNodeWrapper<Tp, Operation, Compare>::template type, BufferType>>
    auto make_Splay(Operation op) -> TreeType { return TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename TreeType = SPLAY::Tree<SPLAY::CustomNodeWrapper<Tp, const Tp &(*)(const Tp &, const Tp &), Compare>::template type, BufferType>>
    auto make_Splay(const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename TreeType = SPLAY::Tree<SPLAY::CustomNodeWrapper<Tp, Tp (*)(Tp, Tp), Compare>::template type, BufferType>>
    auto make_Splay(Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename ModifyType, bool InitClearLazy, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename Mapping, typename Composition, typename TreeType = SPLAY::Tree<SPLAY::CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::template type, BufferType>>
    auto make_lazy_Splay(Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect>
    using SplayMultiset = SPLAY::Tree<SPLAY::BaseNodeWrapper<Tp, Compare>::template type, BufferType>;
}

#endif