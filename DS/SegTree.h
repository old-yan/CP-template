/*
最后修改:
20240928
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGTREE__
#define __OY_SEGTREE__

#include <functional>
#include <numeric>

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace Seg {
        using size_type = uint32_t;
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using node_type = BaseNode<ValueType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const value_type &modify, node_type *x) { x->m_val += modify; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename ModifyType, typename SizeType>
        struct LazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = LazyNode<ValueType, ModifyType, SizeType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x, SizeType len) { x->m_val += modify * len; }
            static void com(const modify_type &modify, node_type *x) { x->m_modify += modify; }
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            bool has_lazy() const { return bool(m_modify); }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = 0; }
            void pushup(node_type *lchild, node_type *rchild) { m_val = lchild->m_val + rchild->m_val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            using node_type = CustomNode<ValueType, Operation>;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        struct CustomLazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>;
            static Operation s_op;
            static Mapping s_map;
            static Composition s_com;
            static modify_type s_default_modify;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            static void map(const modify_type &modify, node_type *x, SizeType len) { x->m_val = s_map(modify, x->m_val, len); }
            static void com(const modify_type &modify, node_type *x) { x->m_modify = s_com(modify, x->m_modify); }
            static constexpr bool init_clear_lazy = InitClearLazy;
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = s_default_modify; }
        };
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Operation CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Mapping CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_map;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Composition CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_com;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        ModifyType CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_default_modify;
        struct Ignore {};
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
        struct Has_modify_type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_modify_type<Tp, ValueType, void_t<typename Tp::modify_type>> : std::true_type {
            using type = typename Tp::modify_type;
        };
        template <typename Tp, typename = void>
        struct Has_has_lazy : std::false_type {};
        template <typename Tp>
        struct Has_has_lazy<Tp, void_t<decltype(std::declval<Tp>().has_lazy())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_get_lazy : std::false_type {};
        template <typename Tp>
        struct Has_get_lazy<Tp, void_t<decltype(std::declval<Tp>().get_lazy())>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename SizeType, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr, typename SizeType>
        struct Has_pushup<Tp, NodePtr, SizeType, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType, typename SizeType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, void, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType, typename SizeType>
        struct Has_map<Tp, NodePtr, ModifyType, SizeType, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename Node, typename RangeMapping = Ignore, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            struct node : Node {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using tree_type = Tree<Node, RangeMapping, SizeType, BufferType>;
            using buffer_type = BufferType<node>;
            using value_type = typename node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            struct DefaultGetter {
                using value_type = typename node::value_type;
                value_type operator()(const node *p) const { return p->get(); }
                void operator()(value_type &x, const node *p) const { x = node::op(x, p->get()); }
                void operator()(const node *p, value_type &x) const { x = node::op(p->get(), x); }
                void operator()(value_type &x, const value_type &y) const { x = node::op(x, y); }
            };
            struct iterator {
                SizeType m_index;
                node *m_ptr;
            };
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            SizeType m_sz;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _apply(size_type cur, const modify_type &modify, SizeType len) { _apply(_ptr(cur), modify, len); }
            static void _apply(node *p, const modify_type &modify, SizeType len) { node::map(modify, p, len), node::com(modify, p); }
            static void _apply(size_type cur, const modify_type &modify) { _apply(_ptr(cur), modify); }
            static void _apply(node *p, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type, void>::value)
                    node::map(modify, p);
                else if constexpr (Has_map<node, node *, modify_type, SizeType>::value)
                    node::map(modify, p, 1);
                else
                    p->set(node::op(modify, p->get()));
            }
            template <typename Func>
            static void _merge_by(size_type cur, size_type other, SizeType floor, SizeType ceil, Func &&func) {
                node *p = _ptr(cur), *q = _ptr(other);
                if constexpr (std::is_same<typename std::decay<Func>::type, Ignore>::value) {
                    p->set(node::op(p->get(), q->get()));
                    if (floor != ceil)
                        if constexpr (Has_get_lazy<node>::value) {
                            if constexpr (Has_has_lazy<node>::value)
                                if (!q->has_lazy()) return;
                            node::com(q->get_lazy(), _ptr(cur));
                        }
                } else
                    func(p, q);
                _collect(other);
            }
            static bool _has_lazy(size_type cur) {
                if constexpr (!Has_get_lazy<node>::value) return false;
                if constexpr (!Has_has_lazy<node>::value)
                    return true;
                else
                    return _ptr(cur)->has_lazy();
            }
            static size_type _newnode(SizeType floor, SizeType ceil) {
                size_type c = buffer_type::newnode();
                if constexpr (!std::is_same<RangeMapping, Ignore>::value) _ptr(c)->set(RangeMapping()(floor, ceil));
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy) _ptr(c)->clear_lazy();
                return c;
            }
            static void _collect(size_type x) { *_ptr(x) = {}, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            static size_type _lchild(size_type cur, SizeType floor, SizeType ceil) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode(floor, ceil);
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType floor, SizeType ceil) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode(floor, ceil);
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            template <typename InitMapping>
            static void _initnode(size_type cur, SizeType floor, SizeType ceil, InitMapping &&mapping) {
                if (floor == ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) _ptr(cur)->set(mapping(floor));
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    _initnode(_lchild(cur, floor, mid), floor, mid, mapping);
                    _initnode(_rchild(cur, mid + 1, ceil), mid + 1, ceil, mapping);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _pushdown_naive(size_type cur, SizeType floor, SizeType ceil, SizeType mid) {
                node *p = _ptr(cur);
                if constexpr (Has_get_lazy<node>::value) {
                    _apply(p->lchild(), p->get_lazy(), mid - floor + 1);
                    _apply(p->rchild(), p->get_lazy(), ceil - mid);
                    p->clear_lazy();
                }
            }
            static void _pushdown(size_type cur, SizeType floor, SizeType ceil, SizeType mid) {
                _lchild(cur, floor, mid), _rchild(cur, mid + 1, ceil);
                if (_has_lazy(cur)) _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _pushdown_l(size_type cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                _lchild(cur, floor, mid);
            }
            static void _pushdown_r(size_type cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                _rchild(cur, mid + 1, ceil);
            }
            static void _pushdown_if_lazy(size_type cur, SizeType floor, SizeType ceil, SizeType mid) {
                _lchild(cur, floor, mid), _rchild(cur, mid + 1, ceil);
                _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _pushup(size_type cur, SizeType len) {
                node *p = _ptr(cur);
                if constexpr (Has_pushup<node, node *, SizeType>::value)
                    p->pushup(p->lchild(), p->rchild(), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    p->pushup(p->lchild(), p->rchild());
                else
                    p->set(node::op(p->lchild()->get(), p->rchild()->get()));
            }
            template <bool ReadOnly, typename Callback>
            static auto _do_for_node(size_type cur, SizeType floor, SizeType ceil, SizeType i, Callback &&call) -> decltype(call(_ptr(0))) {
                if (floor == ceil)
                    return call(_ptr(cur));
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid) {
                        _pushdown_l(cur, floor, ceil, mid);
                        if constexpr (ReadOnly)
                            return _do_for_node<ReadOnly>(_ptr(cur)->m_lc, floor, mid, i, call);
                        else
                            _do_for_node<ReadOnly>(_ptr(cur)->m_lc, floor, mid, i, call), _pushup(cur, ceil - floor + 1);
                    } else {
                        _pushdown_r(cur, floor, ceil, mid);
                        if constexpr (ReadOnly)
                            return _do_for_node<ReadOnly>(_ptr(cur)->m_rc, mid + 1, ceil, i, call);
                        else
                            _do_for_node<ReadOnly>(_ptr(cur)->m_rc, mid + 1, ceil, i, call), _pushup(cur, ceil - floor + 1);
                    }
                }
            }
            static void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, const modify_type &modify) {
                if (left <= floor && right >= ceil)
                    _apply(cur, modify, ceil - floor + 1);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, floor, ceil, mid);
                    if (left <= mid) _add(_ptr(cur)->m_lc, floor, mid, left, right, modify);
                    if (right > mid) _add(_ptr(cur)->m_rc, mid + 1, ceil, left, right, modify);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(SizeType left, SizeType right) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return Getter()(_ptr(0));
                else
                    return RangeMapping()(left, right);
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur) return _query<Getter>(i, i);
                if (floor == ceil) return Getter()(_ptr(cur));
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                return i <= mid ? _query<Getter>(_ptr(cur)->m_lc, floor, mid, i) : _query<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, i);
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur) return _query<Getter>(left, right);
                if (left == floor && right == ceil) return Getter()(_ptr(cur));
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (left > mid) return _query<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, left, right);
                if (right <= mid) return _query<Getter>(_ptr(cur)->m_lc, floor, mid, left, right);
                auto res(_query<Getter>(_ptr(cur)->m_lc, floor, mid, left, mid));
                Getter()(res, _query<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, mid + 1, right));
                return res;
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
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start <= mid) {
                    SizeType res = _max_right<Getter>(_ptr(cur)->m_lc, floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, start, val, judge);
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
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start > mid) {
                    SizeType res = _min_left<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left<Getter>(_ptr(cur)->m_lc, floor, mid, start, val, judge);
            }
            template <typename Getter = DefaultGetter>
            static iterator _kth(size_type cur, SizeType floor, SizeType ceil, typename Getter::value_type k) {
                if (floor == ceil) return {floor, _ptr(cur)};
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (Getter()(_ptr(cur)->lchild()) > k)
                    return _kth<Getter>(_ptr(cur)->m_lc, floor, mid, k);
                else
                    return _kth<Getter>(_ptr(cur)->m_rc, mid + 1, ceil, k - Getter()(_ptr(cur)->lchild()));
            }
            static void _split_by_key(size_type cur, size_type other, SizeType floor, SizeType ceil, SizeType key) {
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (key > mid + 1) {
                    if (_ptr(cur)->m_rc) _split_by_key(_ptr(cur)->m_rc, _rchild(other, mid + 1, ceil), mid + 1, ceil, key);
                } else {
                    if (key <= mid && _ptr(cur)->m_lc) _split_by_key(_ptr(cur)->m_lc, _lchild(other, floor, mid), floor, mid, key);
                    std::swap(_ptr(cur)->m_rc, _ptr(other)->m_rc);
                }
                _pushup(cur, ceil - floor + 1), _pushup(other, ceil - floor + 1);
            }
            template <typename Getter = DefaultGetter>
            static void _split_by_rank(size_type cur, size_type other, SizeType floor, SizeType ceil, uint32_t k) {
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (k < Getter()(_ptr(cur)->lchild())) {
                    if (_ptr(cur)->m_lc) _split_by_rank<Getter>(_ptr(cur)->m_lc, _lchild(other, floor, mid), floor, mid, k);
                    std::swap(_ptr(cur)->m_rc, _ptr(other)->m_rc);
                } else if (_ptr(cur)->m_rc) {
                    k -= Getter()(_ptr(cur)->lchild());
                    if (!k)
                        std::swap(_ptr(cur)->m_rc, _ptr(other)->m_rc);
                    else
                        _split_by_rank<Getter>(_ptr(cur)->m_rc, _rchild(other, mid + 1, ceil), mid + 1, ceil, k);
                }
                _pushup(cur, ceil - floor + 1), _pushup(other, ceil - floor + 1);
            }
            template <typename Func>
            static void _merge(size_type &cur, size_type other, SizeType floor, SizeType ceil, Func &&func) {
                if (floor == ceil) return _merge_by(cur, other, floor, ceil, func);
                if (!_ptr(other)->m_lc && !_ptr(other)->m_rc) return _merge_by(cur, other, floor, ceil, func);
                if (!_ptr(cur)->m_lc && !_ptr(cur)->m_rc) return std::swap(cur, other), _merge_by(cur, other, floor, ceil, func);
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (_has_lazy(other)) _pushdown_if_lazy(other, floor, ceil, mid);
                if (!_ptr(cur)->m_lc)
                    _ptr(cur)->m_lc = _ptr(other)->m_lc;
                else if (_ptr(other)->m_lc) {
                    size_type lc = _ptr(cur)->m_lc;
                    _merge(lc, _ptr(other)->m_lc, floor, mid, func), _ptr(cur)->m_lc = lc;
                }
                if (!_ptr(cur)->m_rc)
                    _ptr(cur)->m_rc = _ptr(other)->m_rc;
                else if (_ptr(other)->m_rc) {
                    size_type rc = _ptr(cur)->m_rc;
                    _merge(rc, _ptr(other)->m_rc, mid + 1, ceil, func), _ptr(cur)->m_rc = rc;
                }
                _pushup(cur, ceil - floor + 1), _collect(other);
            }
            node *_root() const { return _ptr(m_rt); }
            template <typename Callback>
            static void _do_for_each(size_type cur, SizeType floor, SizeType ceil, Callback &&call) {
                if (!cur)
                    for (SizeType i = floor; i <= ceil; i++) call(_query(i, i));
                else if (floor == ceil)
                    call(_ptr(cur)->get());
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                    _do_for_each(_ptr(cur)->m_lc, floor, mid, call), _do_for_each(_ptr(cur)->m_rc, mid + 1, ceil, call);
                }
            }
        public:
            Tree() = default;
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt), std::swap(m_sz, rhs.m_sz); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt), std::swap(m_sz, rhs.m_sz);
                return *this;
            }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            template <typename InitMapping = Ignore>
            Tree(SizeType length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            SizeType size() const { return m_sz; }
            template <typename InitMapping = Ignore>
            void resize(SizeType length, InitMapping mapping = InitMapping()) {
                if ((m_sz = length)) {
                    m_rt = _newnode(0, m_sz - 1);
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) _initnode(m_rt, 0, m_sz - 1, mapping);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](SizeType i) { return *(first + i); });
            }
            void modify(SizeType i, const value_type &val) {
                do_for_node<false>(i, [&](node *p) { p->set(val); });
            }
            void add(SizeType i, const modify_type &modify) {
                do_for_node<false>(i, [&](node *p) { _apply(p, modify); });
            }
            void add(SizeType left, SizeType right, const modify_type &modify) { _add(m_rt, 0, m_sz - 1, left, right, modify); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(SizeType i) const { return _query<Getter>(m_rt, 0, m_sz - 1, i); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(SizeType left, SizeType right) const { return _query<Getter>(m_rt, 0, m_sz - 1, left, right); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query_all() const { return Getter()(_root()); }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) const {
                typename Getter::value_type val;
                return _max_right<Getter>(m_rt, 0, m_sz - 1, left, val, judge);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) const {
                typename Getter::value_type val;
                return _min_left<Getter>(m_rt, 0, m_sz - 1, right, val, judge);
            }
            template <typename Getter = DefaultGetter>
            iterator kth(typename Getter::value_type k) const { return _kth<Getter>(m_rt, 0, m_sz - 1, k); }
            tree_type split_by_key(SizeType key) {
                tree_type other(m_sz);
                if (!key)
                    std::swap(m_rt, other.m_rt);
                else if (key < m_sz)
                    _split_by_key(m_rt, other.m_rt, 0, m_sz - 1, key);
                return other;
            }
            template <typename Getter = DefaultGetter>
            tree_type split_by_rank(typename Getter::value_type k) {
                tree_type other(m_sz);
                if (!k)
                    std::swap(m_rt, other.m_rt);
                else if (k < Getter()(_root()))
                    _split_by_rank<Getter>(m_rt, other.m_rt, 0, m_sz - 1, k);
                return other;
            }
            template <typename Func = Ignore>
            void merge(tree_type &other, Func &&func = Func()) { _merge(m_rt, other.m_rt, 0, m_sz - 1, func), other.m_rt = 0; }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(SizeType i, Callback &&call) const -> decltype(call(_ptr(0))) { return _do_for_node<ReadOnly>(m_rt, 0, m_sz - 1, i, call); }
            template <typename Callback>
            void do_for_each(Callback &&call) const { _do_for_each(m_rt, 0, m_sz - 1, call); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, SizeType, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename RangeMapping = Seg::Ignore, typename SizeType, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename InitMapping = Seg::Ignore, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Operation>, RangeMapping, SizeType, BufferType>>
    auto make_SegTree(SizeType length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <template <typename> typename BufferType = VectorBufferWithCollect, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Operation>, Seg::Ignore, uint32_t, BufferType>>
    auto make_SegTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, typename RangeMapping = Seg::Ignore, typename SizeType, template <typename> typename BufferType = VectorBufferWithCollect, typename InitMapping = Seg::Ignore, typename Operation, typename Mapping, typename Composition, typename TreeType = Seg::Tree<Seg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>, RangeMapping, SizeType, BufferType>>
    auto make_lazy_SegTree(SizeType length, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, template <typename> typename BufferType = VectorBufferWithCollect, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = Seg::Tree<Seg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, uint32_t>, Seg::Ignore, uint32_t, BufferType>>
    auto make_lazy_SegTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(first, last); }
    template <typename Tp, typename SizeType = uint64_t>
    using VectorSegSumTree = Seg::Tree<Seg::BaseNode<Tp>, Seg::Ignore, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorSegLazySumTree = Seg::Tree<Seg::LazyNode<Tp, Tp, SizeType>, Seg::Ignore, SizeType>;
}

#endif