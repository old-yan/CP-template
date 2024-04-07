/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGTREE__
#define __OY_SEGTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace Seg {
        using index_type = uint32_t;
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
        template <typename Tp, typename = void>
        struct Has_init_set : std::false_type {};
        template <typename Tp>
        struct Has_init_set<Tp, void_t<decltype(std::declval<Tp>().init_set({}))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename SizeType, typename = void>
        struct Has_init_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_init_pushup<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().init_pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename SizeType>
        struct Has_init_pushup<Tp, NodePtr, SizeType, void_t<decltype(std::declval<Tp>().init_pushup(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<SizeType>()))>> : std::true_type {};
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
        template <typename Node, typename RangeMapping = Ignore, bool Complete = false, typename SizeType = uint64_t, index_type MAX_NODE = 1 << 22>
        struct Tree {
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
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
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            index_type m_root;
            SizeType m_size;
            static void _apply(node *cur, const modify_type &modify, SizeType len) { node::map(modify, cur, len), node::com(modify, cur); }
            static void _apply(node *cur, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type, void>::value)
                    node::map(modify, cur);
                else if constexpr (Has_map<node, node *, modify_type, SizeType>::value)
                    node::map(modify, cur, 1);
                else
                    cur->set(node::op(modify, cur->get()));
            }
            template <typename Func>
            static void _merge_by(node *cur, node *other, SizeType floor, SizeType ceil, Func &&func) {
                if constexpr (std::is_same<typename std::decay<Func>::type, Ignore>::value) {
                    cur->set(node::op(cur->get(), other->get()));
                    if (floor != ceil)
                        if constexpr (Has_get_lazy<node>::value) {
                            if constexpr (Has_has_lazy<node>::value)
                                if (!other->has_lazy()) return;
                            node::com(other->get_lazy(), cur);
                        }
                } else
                    func(cur, other);
            }
            static bool _has_lazy(node *cur) {
                if constexpr (!Has_get_lazy<node>::value) return false;
                if constexpr (!Has_has_lazy<node>::value)
                    return true;
                else
                    return cur->has_lazy();
            }
            static index_type _newnode(SizeType floor, SizeType ceil) {
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) s_buffer[s_use_count].set(RangeMapping()(floor, ceil));
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy)
                        s_buffer[s_use_count].clear_lazy();
                return s_use_count++;
            }
            template <typename InitMapping>
            static void _initnode(node *cur, SizeType floor, SizeType ceil, InitMapping mapping) {
                if (floor == ceil) {
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                        if constexpr (Has_init_set<node>::value)
                            cur->init_set(mapping(floor));
                        else
                            cur->set(mapping(floor));
                    }
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    cur->m_lchild = _newnode(floor, mid);
                    cur->m_rchild = _newnode(mid + 1, ceil);
                    _initnode(cur->lchild(), floor, mid, mapping);
                    _initnode(cur->rchild(), mid + 1, ceil, mapping);
                    _init_pushup(cur, ceil - floor + 1);
                }
            }
            static void _pushdown_naive(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (Has_get_lazy<node>::value) {
                    _apply(cur->lchild(), cur->get_lazy(), mid - floor + 1);
                    _apply(cur->rchild(), cur->get_lazy(), ceil - mid);
                    cur->clear_lazy();
                }
            }
            static void _pushdown(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(floor, mid);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, ceil);
                }
                if (_has_lazy(cur)) _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _pushdown_l(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newnode(floor, mid);
            }
            static void _pushdown_r(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, ceil);
            }
            static void _pushdown_if_lazy(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(floor, mid);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, ceil);
                }
                _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _init_pushup(node *cur, SizeType len) {
                if constexpr (Has_init_pushup<node, node *, SizeType>::value)
                    cur->init_pushup(cur->lchild(), cur->rchild(), len);
                else if constexpr (Has_init_pushup<node, node *, void>::value)
                    cur->init_pushup(cur->lchild(), cur->rchild());
                _pushup(cur, len);
            }
            static void _pushup(node *cur, SizeType len) {
                if constexpr (Has_pushup<node, node *, SizeType>::value)
                    cur->pushup(cur->lchild(), cur->rchild(), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    cur->pushup(cur->lchild(), cur->rchild());
                else
                    cur->set(node::op(cur->lchild()->get(), cur->rchild()->get()));
            }
            template <bool ReadOnly, typename Callback>
            static auto _do_for_node(node *cur, SizeType floor, SizeType ceil, SizeType i, Callback &&call) -> decltype(call(cur)) {
                if (floor == ceil)
                    return call(cur);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid) {
                        _pushdown_l(cur, floor, ceil, mid);
                        if constexpr (ReadOnly)
                            return _do_for_node<ReadOnly>(cur->lchild(), floor, mid, i, call);
                        else
                            _do_for_node<ReadOnly>(cur->lchild(), floor, mid, i, call), _pushup(cur, ceil - floor + 1);
                    } else {
                        _pushdown_r(cur, floor, ceil, mid);
                        if constexpr (ReadOnly)
                            return _do_for_node<ReadOnly>(cur->rchild(), mid + 1, ceil, i, call);
                        else
                            _do_for_node<ReadOnly>(cur->rchild(), mid + 1, ceil, i, call), _pushup(cur, ceil - floor + 1);
                    }
                }
            }
            static void _add(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, const modify_type &modify) {
                if (left <= floor && right >= ceil)
                    _apply(cur, modify, ceil - floor + 1);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, floor, ceil, mid);
                    if (left <= mid) _add(cur->lchild(), floor, mid, left, right, modify);
                    if (right > mid) _add(cur->rchild(), mid + 1, ceil, left, right, modify);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(SizeType left, SizeType right) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return Getter()(s_buffer);
                else
                    return RangeMapping()(left, right);
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (cur->is_null()) return _query<Getter>(i, i);
                if (floor == ceil) return Getter()(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                return i <= mid ? _query<Getter>(cur->lchild(), floor, mid, i) : _query<Getter>(cur->rchild(), mid + 1, ceil, i);
            }
            template <typename Getter = DefaultGetter>
            static typename Getter::value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (cur->is_null()) return _query<Getter>(left, right);
                if (left == floor && right == ceil) return Getter()(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (left > mid) return _query<Getter>(cur->rchild(), mid + 1, ceil, left, right);
                if (right <= mid) return _query<Getter>(cur->lchild(), floor, mid, left, right);
                auto res(_query<Getter>(cur->lchild(), floor, mid, left, mid));
                Getter()(res, _query<Getter>(cur->rchild(), mid + 1, ceil, mid + 1, right));
                return res;
            }
            template <typename Getter = DefaultGetter, typename Judger>
            static SizeType _max_right(node *cur, SizeType floor, SizeType ceil, SizeType start, typename Getter::value_type &val, Judger &&judge) {
                if (start <= floor) {
                    auto a(val);
                    if (start < floor)
                        Getter()(a, cur);
                    else
                        a = Getter()(cur);
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor == ceil)
                        return floor - 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start <= mid) {
                    SizeType res = _max_right<Getter>(cur->lchild(), floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right<Getter>(cur->rchild(), mid + 1, ceil, start, val, judge);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            static SizeType _min_left(node *cur, SizeType floor, SizeType ceil, SizeType start, typename Getter::value_type &val, Judger &&judge) {
                if (start >= ceil) {
                    auto a(val);
                    if (start > ceil)
                        Getter()(cur, a);
                    else
                        a = Getter()(cur);
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start > mid) {
                    SizeType res = _min_left<Getter>(cur->rchild(), mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left<Getter>(cur->lchild(), floor, mid, start, val, judge);
            }
            template <typename Getter = DefaultGetter>
            static iterator _kth(node *cur, SizeType floor, SizeType ceil, typename Getter::value_type k) {
                if (floor == ceil) return {floor, cur};
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (Getter()(cur->lchild()) > k)
                    return _kth<Getter>(cur->lchild(), floor, mid, k);
                else
                    return _kth<Getter>(cur->rchild(), mid + 1, ceil, k - Getter()(cur->lchild()));
            }
            static void _split_by_key(node *cur, node *other, SizeType floor, SizeType ceil, SizeType key) {
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (key > mid + 1) {
                    if (cur->m_rchild) other->m_rchild = _newnode(mid + 1, ceil), _split_by_key(cur->rchild(), other->rchild(), mid + 1, ceil, key);
                } else {
                    if (key <= mid && cur->m_lchild) other->m_lchild = _newnode(floor, mid), _split_by_key(cur->lchild(), other->lchild(), floor, mid, key);
                    std::swap(cur->m_rchild, other->m_rchild);
                }
                _pushup(cur, ceil - floor + 1), _pushup(other, ceil - floor + 1);
            }
            template <typename Getter = DefaultGetter>
            static void _split_by_rank(node *cur, node *other, SizeType floor, SizeType ceil, uint32_t k) {
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (k < Getter()(cur->lchild())) {
                    if (cur->m_lchild) other->m_lchild = _newnode(floor, mid), _split_by_rank<Getter>(cur->lchild(), other->lchild(), floor, mid, k);
                    std::swap(cur->m_rchild, other->m_rchild);
                } else if (cur->m_rchild) {
                    k -= Getter()(cur->lchild());
                    if (!k)
                        std::swap(cur->m_rchild, other->m_rchild);
                    else
                        other->m_rchild = _newnode(mid + 1, ceil), _split_by_rank<Getter>(cur->rchild(), other->rchild(), mid + 1, ceil, k);
                }
                _pushup(cur, ceil - floor + 1), _pushup(other, ceil - floor + 1);
            }
            template <typename Func>
            static void _merge(node *cur, node *other, SizeType floor, SizeType ceil, Func &&func) {
                if (floor == ceil) return _merge_by(cur, other, floor, ceil, func);
                SizeType mid = (floor + ceil) >> 1;
                _merge_by(cur, other, floor, ceil, func);
                if (!cur->m_lchild)
                    std::swap(cur->m_lchild, other->m_lchild);
                else if (other->m_lchild)
                    _merge(cur->lchild(), other->lchild(), floor, mid, func);
                if (!cur->m_rchild)
                    std::swap(cur->m_rchild, other->m_rchild);
                else if (other->m_rchild)
                    _merge(cur->rchild(), other->rchild(), mid + 1, ceil, func);
                _pushup(cur, ceil - floor + 1);
            }
            node *_root() const { return s_buffer + m_root; }
            template <typename Callback>
            static void _do_for_each(node *cur, SizeType floor, SizeType ceil, Callback &&call) {
                if (cur->is_null())
                    for (SizeType i = floor; i <= ceil; i++) call(_query(i, i));
                else if (floor == ceil)
                    call(cur->get());
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                    _do_for_each(cur->lchild(), floor, mid, call), _do_for_each(cur->rchild(), mid + 1, ceil, call);
                }
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(SizeType length, InitMapping mapping = InitMapping()) {
                if (m_size = length) {
                    m_root = _newnode(0, m_size - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _initnode(_root(), 0, m_size - 1, mapping);
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
            void add(SizeType left, SizeType right, const modify_type &modify) { _add(_root(), 0, m_size - 1, left, right, modify); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(SizeType i) const { return _query<Getter>(_root(), 0, m_size - 1, i); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(SizeType left, SizeType right) const { return _query<Getter>(_root(), 0, m_size - 1, left, right); }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query_all() const { return Getter()(_root()); }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) const {
                typename Getter::value_type val;
                return _max_right<Getter>(_root(), 0, m_size - 1, left, val, judge);
            }
            template <typename Getter = DefaultGetter, typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) const {
                typename Getter::value_type val;
                return _min_left<Getter>(_root(), 0, m_size - 1, right, val, judge);
            }
            template <typename Getter = DefaultGetter>
            iterator kth(typename Getter::value_type k) const { return _kth<Getter>(_root(), 0, m_size - 1, k); }
            Tree<Node, RangeMapping, false, SizeType, MAX_NODE> split_by_key(SizeType key) {
                static_assert(!Complete, "Complete Segtree Mustn't Split");
                Tree other(m_size);
                if (!key)
                    std::swap(m_root, other.m_root);
                else if (key < m_size)
                    _split_by_key(_root(), other._root(), 0, m_size - 1, key);
                return other;
            }
            template <typename Getter = DefaultGetter>
            Tree<Node, RangeMapping, false, SizeType, MAX_NODE> split_by_rank(typename Getter::value_type k) {
                static_assert(!Complete, "Complete Segtree Mustn't Split");
                Tree other(m_size);
                if (!k)
                    std::swap(m_root, other.m_root);
                else if (k < Getter()(_root()))
                    _split_by_rank<Getter>(_root(), other._root(), 0, m_size - 1, k);
                return other;
            }
            template <typename Func = Ignore>
            void merge(Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE> &other, Func &&func = Func()) { _merge(_root(), other._root(), 0, m_size - 1, func); }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(SizeType i, Callback &&call) const -> decltype(call(s_buffer)) { return _do_for_node<ReadOnly>(_root(), 0, m_size - 1, i, call); }
            template <typename Callback>
            void do_for_each(Callback &&call) const { _do_for_each(_root(), 0, m_size - 1, call); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::node Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp, bool Complete, typename RangeMapping = Seg::Ignore, Seg::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename InitMapping = Seg::Ignore, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Operation>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree(SizeType length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg::Ignore, Seg::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = Seg::Ignore, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree(SizeType length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg::Ignore, Seg::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = Seg::Ignore, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Tp (*)(Tp, Tp)>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree(SizeType length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <Seg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Operation>, Seg::Ignore, true, uint32_t, MAX_NODE>>
    auto make_SegTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <Seg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, Seg::Ignore, true, uint32_t, MAX_NODE>>
    auto make_SegTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <Seg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Seg::Tree<Seg::CustomNode<Tp, Tp (*)(Tp, Tp)>, Seg::Ignore, true, uint32_t, MAX_NODE>>
    auto make_SegTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, bool Complete, typename RangeMapping = Seg::Ignore, Seg::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = Seg::Ignore, typename Operation, typename Mapping, typename Composition, typename TreeType = Seg::Tree<Seg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_lazy_SegTree(SizeType length, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, Seg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = Seg::Tree<Seg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, uint32_t>, Seg::Ignore, true, uint32_t, MAX_NODE>>
    auto make_lazy_SegTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(first, last); }
    template <bool Complete = false, typename SizeType = uint64_t, Seg::index_type MAX_NODE = 1 << 22>
    using SegSumTree = Seg::Tree<Seg::BaseNode<int64_t>, Seg::Ignore, Complete, SizeType, MAX_NODE>;
    template <bool Complete = false, typename SizeType = uint64_t, Seg::index_type MAX_NODE = 1 << 22>
    using SegLazySumTree = Seg::Tree<Seg::LazyNode<int64_t, int64_t, SizeType>, Seg::Ignore, Complete, SizeType, MAX_NODE>;
}

#endif