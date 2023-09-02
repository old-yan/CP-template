#ifndef __OY_LAZYSEGTREE2D__
#define __OY_LAZYSEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace LazySeg2D {
        using index_type = uint32_t;
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
            static void map(const modify_type &modify, node_type *x, SizeType row_len, SizeType col_len) { x->m_val += modify * row_len * col_len; }
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
            using modify_type = ValueType;
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
            static void map(const modify_type &modify, node_type *x, SizeType row_len, SizeType col_len) { x->m_val = s_map(modify, x->m_val, row_len, col_len); }
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
        struct NoInit {};
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
        template <typename Tp, typename = void>
        struct Has_has_lazy : std::false_type {};
        template <typename Tp>
        struct Has_has_lazy<Tp, void_t<decltype(std::declval<Tp>().has_lazy())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_get_lazy : std::false_type {};
        template <typename Tp>
        struct Has_get_lazy<Tp, void_t<decltype(std::declval<Tp>().get_lazy())>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename Node, typename RangeMapping = NoInit, bool Complete = false, typename SizeType = uint64_t, index_type MAX_NODE = 1 << 22>
        struct Tree {
            using value_type = typename Node::value_type;
            using modify_type = typename Node::modify_type;
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            index_type m_root;
            SizeType m_row, m_column;
            static bool _has_lazy(node *cur) {
                if constexpr (!Has_get_lazy<node>::value) return false;
                if constexpr (!Has_has_lazy<node>::value)
                    return true;
                else
                    return cur->has_lazy();
            }
            static void _apply(node *cur, const modify_type &modify, SizeType row_len, SizeType column_len) { node::map(modify, cur, row_len, column_len), node::com(modify, cur); }
            static void _apply(node *cur, const modify_type &modify) {
                if constexpr (Has_get_lazy<node>::value)
                    node::map(modify, cur, 1, 1);
                else if constexpr (Has_map<node, node *, modify_type>::value)
                    node::map(modify, cur);
                else
                    cur->set(node::op(modify, cur->get()));
            }
            node *_root() const { return s_buffer + m_root; }
            static index_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                if constexpr (!Complete && !std::is_same<RangeMapping, NoInit>::value) s_buffer[s_use_count].set(RangeMapping()(row_floor, row_ceil, column_floor, column_ceil));
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy)
                        s_buffer[s_use_count].clear_lazy();
                return s_use_count++;
            }
            template <typename InitMapping>
            static void _initnode_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (row_floor != row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    cur->m_lchild = _newnode(row_floor, mid, column_floor, column_ceil);
                    cur->m_rchild = _newnode(mid + 1, row_ceil, column_floor, column_ceil);
                    _initnode_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, mapping);
                    _initnode_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, mapping);
                    _pushup(cur);
                } else if (column_floor != column_ceil)
                    return _initnode_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, mapping);
                else if constexpr (!std::is_same<InitMapping, NoInit>::value)
                    cur->set(mapping(row_floor, column_floor));
            }
            template <typename InitMapping>
            static void _initnode_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor != column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                    _initnode_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, mapping);
                    _initnode_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, mapping);
                    _pushup(cur);
                } else if (row_floor != row_ceil)
                    return _initnode_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, mapping);
                else if constexpr (!std::is_same<InitMapping, NoInit>::value)
                    cur->set(mapping(row_floor, column_floor));
            }
            template <typename InitMapping>
            static void _initnode(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) { return row_floor + column_ceil <= row_ceil + column_floor ? _initnode_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, mapping) : _initnode_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, mapping); }
            static void _pushdown_by_row_up(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_row_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, mid, column_floor, column_ceil);
            }
            static void _pushdown_by_row_down(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_row_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, row_ceil, column_floor, column_ceil);
            }
            static void _pushdown_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_row_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, mid, column_floor, column_ceil);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, row_ceil, column_floor, column_ceil);
                }
            }
            static void _pushdown_by_column_left(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_column_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
            }
            static void _pushdown_by_column_right(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_column_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
            }
            static void _pushdown_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (_has_lazy(cur)) return _pushdown_by_column_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                }
            }
            static void _pushdown_by_row_if_lazy(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, mid, column_floor, column_ceil);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, row_ceil, column_floor, column_ceil);
                }
                if constexpr (Has_get_lazy<node>::value) {
                    _apply(cur->lchild(), cur->get_lazy(), mid - row_floor + 1, column_ceil - column_floor + 1);
                    _apply(cur->rchild(), cur->get_lazy(), row_ceil - mid, column_ceil - column_floor + 1);
                    cur->clear_lazy();
                }
            }
            static void _pushdown_by_column_if_lazy(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                }
                if constexpr (Has_get_lazy<node>::value) {
                    _apply(cur->lchild(), cur->get_lazy(), row_ceil - row_floor + 1, mid - column_floor + 1);
                    _apply(cur->rchild(), cur->get_lazy(), row_ceil - row_floor + 1, column_ceil - mid);
                    cur->clear_lazy();
                }
            }
            static void _pushup(node *cur) {
                if constexpr (Has_pushup<node, node *, void>::value)
                    cur->pushup(cur->lchild(), cur->rchild());
                else
                    cur->set(node::op(cur->lchild()->get(), cur->rchild()->get()));
            }
            static void _modify_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &val) {
                if (row_floor == row_ceil)
                    if (column_floor == column_ceil)
                        cur->set(val);
                    else
                        _modify_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, val);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid) {
                        _pushdown_by_row_up(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _modify_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, i, j, val);
                    } else {
                        _pushdown_by_row_down(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _modify_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, i, j, val);
                    }
                    _pushup(cur);
                }
            }
            static void _modify_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &val) {
                if (column_floor == column_ceil)
                    if (row_floor == row_ceil)
                        cur->set(val);
                    else
                        _modify_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, val);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid) {
                        _pushdown_by_column_left(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _modify_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, i, j, val);
                    } else {
                        _pushdown_by_column_right(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _modify_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, i, j, val);
                    }
                    _pushup(cur);
                }
            }
            static void _modify(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &val) { return row_floor + column_ceil <= row_ceil + column_floor ? _modify_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, val) : _modify_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, val); }
            static void _add_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &modify) {
                if (row_floor == row_ceil)
                    if (column_floor == column_ceil)
                        _apply(cur, modify);
                    else
                        _add_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid) {
                        _pushdown_by_row_up(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, i, j, modify);
                    } else {
                        _pushdown_by_row_down(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, i, j, modify);
                    }
                    _pushup(cur);
                }
            }
            static void _add_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &modify) {
                if (column_floor == column_ceil)
                    if (row_floor == row_ceil)
                        _apply(cur, modify);
                    else
                        _add_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, modify);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid) {
                        _pushdown_by_column_left(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, i, j, modify);
                    } else {
                        _pushdown_by_column_right(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, i, j, modify);
                    }
                    _pushup(cur);
                }
            }
            static void _add(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j, const modify_type &modify) { return row_floor + column_ceil <= row_ceil + column_floor ? _add_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, modify) : _add_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j, modify); }
            static void _add_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const modify_type &modify) {
                if (row1 == row_floor && row2 == row_ceil && column1 == column_floor && column2 == column_ceil)
                    _apply(cur, modify, row_ceil - row_floor + 1, column_ceil - column_floor + 1);
                else if (row_floor == row_ceil)
                    _add_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row2 <= mid) {
                        _pushdown_by_row_up(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, row1, row2, column1, column2, modify);
                    } else if (row1 > mid) {
                        _pushdown_by_row_down(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, row1, row2, column1, column2, modify);
                    } else {
                        _pushdown_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, row1, mid, column1, column2, modify);
                        _add_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, mid + 1, row2, column1, column2, modify);
                    }
                    _pushup(cur);
                }
            }
            static void _add_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const modify_type &modify) {
                if (row1 == row_floor && row2 == row_ceil && column1 == column_floor && column2 == column_ceil)
                    _apply(cur, modify, row_ceil - row_floor + 1, column_ceil - column_floor + 1);
                else if (column_floor == column_ceil)
                    _add_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2, modify);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column2 <= mid) {
                        _pushdown_by_column_left(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, row1, row2, column1, column2, modify);
                    } else if (column1 > mid) {
                        _pushdown_by_column_right(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, row1, row2, column1, column2, modify);
                    } else {
                        _pushdown_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                        _add_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, row1, row2, column1, mid, modify);
                        _add_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, row1, row2, mid + 1, column2, modify);
                    }
                    _pushup(cur);
                }
            }
            static void _add(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const modify_type &modify) { return row_floor + column_ceil <= row_ceil + column_floor ? _add_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2, modify) : _add_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2, modify); }
            static value_type _query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, NoInit>::value)
                    return s_buffer[0].get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            static value_type _query_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j) {
                if (cur->is_null()) return _query(i, i, j, j);
                if (row_floor != row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (_has_lazy(cur)) _pushdown_by_row_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                    return i <= mid ? _query_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, i, j) : _query_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, i, j);
                } else if (column_floor != column_ceil)
                    return _query_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j);
                else
                    return cur->get();
            }
            static value_type _query_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j) {
                if (cur->is_null()) return _query(i, i, j, j);
                if (column_floor != column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (_has_lazy(cur)) _pushdown_by_column_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                    return j <= mid ? _query_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, i, j) : _query_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, i, j);
                } else if (row_floor != row_ceil)
                    return _query_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j);
                else
                    return cur->get();
            }
            static value_type _query(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType i, SizeType j) { return row_floor + column_ceil <= row_ceil + column_floor ? _query_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, i, j) : _query_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, i, j); }
            static value_type _query_by_row(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if (cur->is_null()) return _query(row1, row2, column1, column2);
                if (row1 == row_floor && row2 == row_ceil && column1 == column_floor && column2 == column_ceil) return cur->get();
                if (row_floor == row_ceil) return _query_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2);
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_by_row_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if (row1 > mid) return _query_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, row1, row2, column1, column2);
                if (row2 <= mid) return _query_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, row1, row2, column1, column2);
                return node::op(_query_by_column(cur->lchild(), row_floor, mid, column_floor, column_ceil, row1, mid, column1, column2), _query_by_column(cur->rchild(), mid + 1, row_ceil, column_floor, column_ceil, mid + 1, row2, column1, column2));
            }
            static value_type _query_by_column(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if (cur->is_null()) return _query(row1, row2, column1, column2);
                if (row1 == row_floor && row2 == row_ceil && column1 == column_floor && column2 == column_ceil) return cur->get();
                if (column_floor == column_ceil) return _query_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2);
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (_has_lazy(cur)) _pushdown_by_column_if_lazy(cur, row_floor, row_ceil, column_floor, column_ceil, mid);
                if (column1 > mid) return _query_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, row1, row2, column1, column2);
                if (column2 <= mid) return _query_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, row1, row2, column1, column2);
                return node::op(_query_by_row(cur->lchild(), row_floor, row_ceil, column_floor, mid, row1, row2, column1, mid), _query_by_row(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, row1, row2, mid + 1, column2));
            }
            static value_type _query(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) { return row_floor + column_ceil <= row_ceil + column_floor ? _query_by_row(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2) : _query_by_column(cur, row_floor, row_ceil, column_floor, column_ceil, row1, row2, column1, column2); }
            template <typename InitMapping = NoInit>
            Tree(SizeType row = 0, SizeType column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = NoInit>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_root = _newnode(0, m_row - 1, 0, m_column - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, NoInit>::value) _initnode(_root(), 0, m_row - 1, 0, m_column - 1, mapping);
                }
            }
            void modify(SizeType row, SizeType column, const modify_type &val) { _modify(_root(), 0, m_row - 1, 0, m_column - 1, row, column, val); }
            void add(SizeType row, SizeType column, const modify_type &modify) { _add(_root(), 0, m_row - 1, 0, m_column - 1, row, column, modify); }
            void add(SizeType row1, SizeType row2, SizeType column1, SizeType column2, const modify_type &modify) { _add(_root(), 0, m_row - 1, 0, m_column - 1, row1, row2, column1, column2, modify); }
            value_type query(SizeType row, SizeType column) const { return _query(_root(), 0, m_row - 1, 0, m_column - 1, row, column); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query(_root(), 0, m_row - 1, 0, m_column - 1, row1, row2, column1, column2); }
            value_type query_all() const { return _root()->get(); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::node Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp, bool Complete, typename RangeMapping = LazySeg2D::NoInit, LazySeg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename InitMapping = LazySeg2D::NoInit, typename TreeType = LazySeg2D::Tree<LazySeg2D::CustomNode<Tp, Operation>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = LazySeg2D::NoInit, LazySeg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = LazySeg2D::NoInit, typename TreeType = LazySeg2D::Tree<LazySeg2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = LazySeg2D::NoInit, LazySeg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = LazySeg2D::NoInit, typename TreeType = LazySeg2D::Tree<LazySeg2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, RangeMapping, Complete, SizeType, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <bool Complete = false, typename SizeType = uint32_t, LazySeg2D::index_type MAX_NODE = 1 << 23>
    using SegSumTree2D = LazySeg2D::Tree<LazySeg2D::BaseNode<int64_t>, LazySeg2D::NoInit, Complete, SizeType, MAX_NODE>;
    template <bool Complete = false, typename SizeType = uint32_t, LazySeg2D::index_type MAX_NODE = 1 << 23>
    using SegLazySumTree2D = LazySeg2D::Tree<LazySeg2D::LazyNode<int64_t, int64_t, SizeType>, LazySeg2D::NoInit, Complete, SizeType, MAX_NODE>;
}

#endif