/*
最后修改:
20231002
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTSEGTREE__
#define __OY_PERSISTENTSEGTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace PerSeg {
        using index_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            using node_type = BaseNode<ValueType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x) { x->m_val += modify; }
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
        template <typename Node, typename RangeMapping = Ignore, bool Complete = false, bool Lock = false, typename SizeType = uint64_t, index_type MAX_NODE = 1 << 22>
        struct Tree {
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
            using value_type = typename node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            static bool s_lock;
            index_type m_root;
            SizeType m_size;
            static index_type _copynode(node *x) {
                s_buffer[s_use_count] = *x;
                return s_use_count++;
            }
            static void _apply(node *cur, const modify_type &modify, SizeType len) { node::map(modify, cur, len), node::com(modify, cur); }
            static void _apply(node *cur, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type, void>::value)
                    node::map(modify, cur);
                else if constexpr (Has_map<node, node *, modify_type, SizeType>::value)
                    node::map(modify, cur, 1);
                else
                    cur->set(node::op(modify, cur->get()));
            }
            static bool _has_lazy(node *cur) {
                if constexpr (!Has_get_lazy<node>::value) return false;
                if constexpr (!Has_has_lazy<node>::value)
                    return true;
                else
                    return cur->has_lazy();
            }
            static void lock() { s_lock = true; }
            static void unlock() { s_lock = false; }
            static SizeType _reduce_kth(const Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE> &base, const Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE> &end, node *base_cur, node *end_cur, SizeType floor, SizeType ceil, value_type k) {
                if (floor == ceil) return floor;
                SizeType mid = (floor + ceil) >> 1;
                if constexpr (Has_get_lazy<node>::value) {
                    if constexpr (!Has_has_lazy<node>::value)
                        base._pushdown_if_lazy(base_cur, floor, ceil, mid);
                    else if (base_cur->has_lazy())
                        base._pushdown_if_lazy(base_cur, floor, ceil, mid);
                    if constexpr (!Has_has_lazy<node>::value)
                        end._pushdown_if_lazy(end_cur, floor, ceil, mid);
                    else if (end_cur->has_lazy())
                        end._pushdown_if_lazy(end_cur, floor, ceil, mid);
                }
                value_type l = end_cur->lchild()->get() - base_cur->lchild()->get();
                if (l > k)
                    return base_cur->m_lchild ? _reduce_kth(base, end, base_cur->lchild(), end_cur->lchild(), floor, mid, k) : end._kth(end_cur->lchild(), floor, mid, k);
                else
                    return base_cur->m_rchild ? _reduce_kth(base, end, base_cur->rchild(), end_cur->rchild(), mid + 1, ceil, k - l) : end._kth(end_cur->rchild(), mid + 1, ceil, k - l);
            }
            static SizeType reduce_kth(const Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE> &base, const Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE> &end, value_type k) { return _reduce_kth(base, end, base._root(), end._root(), 0, base.m_size - 1, k); }
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
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) cur->set(mapping(floor));
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    cur->m_lchild = _newnode(floor, mid);
                    cur->m_rchild = _newnode(mid + 1, ceil);
                    _initnode(cur->lchild(), floor, mid, mapping);
                    _initnode(cur->rchild(), mid + 1, ceil, mapping);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _copy_lchild(node *cur, SizeType floor, SizeType mid) {
                if (cur->m_lchild) {
                    cur->m_lchild = _copynode(cur->lchild());
                } else if constexpr (!Complete)
                    cur->m_lchild = _newnode(floor, mid);
            }
            static void _copy_rchild(node *cur, SizeType mid, SizeType ceil) {
                if (cur->m_rchild) {
                    cur->m_rchild = _copynode(cur->rchild());
                } else if constexpr (!Complete)
                    cur->m_rchild = _newnode(mid + 1, ceil);
            }
            static void _copy_child(node *cur, SizeType floor, SizeType ceil, SizeType mid) { _copy_lchild(cur, floor, mid), _copy_rchild(cur, mid, ceil); }
            static void _pushdown_naive(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (Has_get_lazy<node>::value) {
                    _apply(cur->lchild(), cur->get_lazy(), mid - floor + 1);
                    _apply(cur->rchild(), cur->get_lazy(), ceil - mid);
                    cur->clear_lazy();
                }
            }
            static void _pushdown(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (!Lock)
                    _copy_child(cur, floor, ceil, mid);
                else if (!s_lock)
                    _copy_child(cur, floor, ceil, mid);
                if (_has_lazy(cur)) _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _pushdown_l(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                if constexpr (!Lock)
                    _copy_lchild(cur, floor, mid);
                else if (!s_lock)
                    _copy_lchild(cur, floor, mid);
            }
            static void _pushdown_r(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_if_lazy(cur, floor, ceil, mid);
                if constexpr (!Lock)
                    _copy_rchild(cur, mid, ceil);
                else if (!s_lock)
                    _copy_rchild(cur, mid, ceil);
            }
            static void _pushdown_if_lazy(node *cur, SizeType floor, SizeType ceil, SizeType mid) {
                if constexpr (!Lock)
                    _copy_child(cur, floor, ceil, mid);
                else if (!s_lock)
                    _copy_child(cur, floor, ceil, mid);
                _pushdown_naive(cur, floor, ceil, mid);
            }
            static void _pushup(node *cur, SizeType len) {
                if constexpr (Has_pushup<node, node *, SizeType>::value)
                    cur->pushup(cur->lchild(), cur->rchild(), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    cur->pushup(cur->lchild(), cur->rchild());
                else
                    cur->set(node::op(cur->lchild()->get(), cur->rchild()->get()));
            }
            static void _modify(node *cur, SizeType floor, SizeType ceil, SizeType i, const value_type &val) {
                if (floor == ceil)
                    cur->set(val);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _pushdown_l(cur, floor, ceil, mid), _modify(cur->lchild(), floor, mid, i, val);
                    else
                        _pushdown_r(cur, floor, ceil, mid), _modify(cur->rchild(), mid + 1, ceil, i, val);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _add(node *cur, SizeType floor, SizeType ceil, SizeType i, const modify_type &modify) {
                if (floor == ceil)
                    _apply(cur, modify);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _pushdown_l(cur, floor, ceil, mid), _add(cur->lchild(), floor, mid, i, modify);
                    else
                        _pushdown_r(cur, floor, ceil, mid), _add(cur->rchild(), mid + 1, ceil, i, modify);
                    _pushup(cur, ceil - floor + 1);
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
            static value_type _query(SizeType left, SizeType right) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return s_buffer[0].get();
                else
                    return RangeMapping()(left, right);
            }
            static value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (cur->is_null()) return _query(i, i);
                if (floor == ceil) return cur->get();
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                return i <= mid ? _query(cur->lchild(), floor, mid, i) : _query(cur->rchild(), mid + 1, ceil, i);
            }
            static value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (cur->is_null()) return _query(left, right);
                if (left == floor && right == ceil) return cur->get();
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (left > mid) return _query(cur->rchild(), mid + 1, ceil, left, right);
                if (right <= mid) return _query(cur->lchild(), floor, mid, left, right);
                return node::op(_query(cur->lchild(), floor, mid, left, mid), _query(cur->rchild(), mid + 1, ceil, mid + 1, right));
            }
            template <typename Judger>
            static SizeType _max_right(node *cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judger &&judge) {
                if (start <= floor) {
                    value_type a = start < floor ? node::op(val, cur->get()) : cur->get();
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor == ceil)
                        return floor - 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start <= mid) {
                    SizeType res = _max_right(cur->lchild(), floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right(cur->rchild(), mid + 1, ceil, start, val, judge);
            }
            template <typename Judger>
            static SizeType _min_left(node *cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judger &&judge) {
                if (start >= ceil) {
                    value_type a = start > ceil ? node::op(cur->get(), val) : cur->get();
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start > mid) {
                    SizeType res = _min_left(cur->rchild(), mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left(cur->lchild(), floor, mid, start, val, judge);
            }
            static SizeType _kth(node *cur, SizeType floor, SizeType ceil, value_type k) {
                if (floor == ceil) return floor;
                SizeType mid = (floor + ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_if_lazy(cur, floor, ceil, mid);
                if (cur->lchild()->get() > k)
                    return _kth(cur->lchild(), floor, mid, k);
                else
                    return _kth(cur->rchild(), mid + 1, ceil, k - cur->lchild()->get());
            }
            node *_root() const { return s_buffer + m_root; }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            Tree copy() const {
                Tree other;
                if (other.m_size = m_size) other.m_root = _copynode(_root());
                return other;
            }
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
            void modify(SizeType i, const value_type &val) { _modify(_root(), 0, m_size - 1, i, val); }
            void add(SizeType i, const modify_type &modify) { _add(_root(), 0, m_size - 1, i, modify); }
            void add(SizeType left, SizeType right, const modify_type &modify) { _add(_root(), 0, m_size - 1, left, right, modify); }
            value_type query(SizeType i) const { return _query(_root(), 0, m_size - 1, i); }
            value_type query(SizeType left, SizeType right) const { return _query(_root(), 0, m_size - 1, left, right); }
            value_type query_all() const { return _root()->get(); }
            template <typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) {
                value_type val;
                return _max_right(_root(), 0, m_size - 1, left, val, judge);
            }
            template <typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) {
                value_type val;
                return _min_left(_root(), 0, m_size - 1, right, val, judge);
            }
            SizeType kth(value_type k) { return _kth(_root(), 0, m_size - 1, k); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, bool Lock, typename SizeType, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, bool Lock, typename SizeType, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE>::node Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, bool Lock, typename SizeType, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE>::s_use_count = 1;
        template <typename Node, typename RangeMapping, bool Complete, bool Lock, typename SizeType, index_type MAX_NODE>
        bool Tree<Node, RangeMapping, Complete, Lock, SizeType, MAX_NODE>::s_lock = true;
    }
    template <typename Tp, bool Complete, typename RangeMapping = PerSeg::Ignore, bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename InitMapping = PerSeg::Ignore, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, Operation>, RangeMapping, Complete, Lock, SizeType, MAX_NODE>>
    auto make_PerSegTree(SizeType length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = PerSeg::Ignore, bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = PerSeg::Ignore, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, RangeMapping, Complete, Lock, SizeType, MAX_NODE>>
    auto make_PerSegTree(SizeType length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = PerSeg::Ignore, bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = PerSeg::Ignore, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, Tp (*)(Tp, Tp)>, RangeMapping, Complete, Lock, SizeType, MAX_NODE>>
    auto make_PerSegTree(SizeType length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, Operation>, PerSeg::Ignore, true, Lock, uint32_t, MAX_NODE>>
    auto make_PerSegTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, PerSeg::Ignore, true, Lock, uint32_t, MAX_NODE>>
    auto make_PerSegTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = PerSeg::Tree<PerSeg::CustomNode<Tp, Tp (*)(Tp, Tp)>, PerSeg::Ignore, true, Lock, uint32_t, MAX_NODE>>
    auto make_PerSegTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, bool Complete, typename RangeMapping = PerSeg::Ignore, bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename Mapping, typename Composition, typename InitMapping = PerSeg::Ignore, typename TreeType = PerSeg::Tree<PerSeg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>, RangeMapping, Complete, Lock, SizeType, MAX_NODE>>
    auto make_lazy_PerSegTree(SizeType length, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType(), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, bool Lock = false, PerSeg::index_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = PerSeg::Tree<PerSeg::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, uint32_t>, PerSeg::Ignore, true, Lock, uint32_t, MAX_NODE>>
    auto make_lazy_PerSegTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(first, last); }
    template <bool Complete = false, bool Lock = false, typename SizeType = uint64_t, PerSeg::index_type MAX_NODE = 1 << 22>
    using PerSegSumTree = PerSeg::Tree<PerSeg::BaseNode<int64_t>, PerSeg::Ignore, Complete, Lock, SizeType, MAX_NODE>;
    template <bool Complete = false, bool Lock = false, typename SizeType = uint64_t, PerSeg::index_type MAX_NODE = 1 << 22>
    using PerSegLazySumTree = PerSeg::Tree<PerSeg::LazyNode<int64_t, int64_t, SizeType>, PerSeg::Ignore, Complete, Lock, SizeType, MAX_NODE>;
}

#endif