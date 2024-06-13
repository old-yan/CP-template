/*
最后修改:
20240613
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGTREE2D__
#define __OY_SEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace Seg2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
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
        template <typename Tp, typename NodePtr, typename ModifyType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <size_type BUFFER1, size_type BUFFER2>
        struct StaticBufferWrap {
            template <typename Node1, typename Node2>
            struct type {
                static Node1 s_buf1[BUFFER1];
                static Node2 s_buf2[BUFFER2];
                static size_type s_use_cnt1, s_use_cnt2;
                static constexpr Node1 *data1() { return s_buf1; }
                static constexpr Node2 *data2() { return s_buf2; }
                static size_type newnode1() { return s_use_cnt1++; }
                static size_type newnode2() { return s_use_cnt2++; }
            };
        };
        template <size_type BUFFER1, size_type BUFFER2>
        template <typename Node1, typename Node2>
        Node1 StaticBufferWrap<BUFFER1, BUFFER2>::type<Node1, Node2>::s_buf1[BUFFER1];
        template <size_type BUFFER1, size_type BUFFER2>
        template <typename Node1, typename Node2>
        Node2 StaticBufferWrap<BUFFER1, BUFFER2>::type<Node1, Node2>::s_buf2[BUFFER2];
        template <size_type BUFFER1, size_type BUFFER2>
        template <typename Node1, typename Node2>
        size_type StaticBufferWrap<BUFFER1, BUFFER2>::type<Node1, Node2>::s_use_cnt1 = 1;
        template <size_type BUFFER1, size_type BUFFER2>
        template <typename Node1, typename Node2>
        size_type StaticBufferWrap<BUFFER1, BUFFER2>::type<Node1, Node2>::s_use_cnt2 = 1;
        template <typename Node1, typename Node2>
        struct VectorBuffer {
            static std::vector<Node1> s_buf1;
            static std::vector<Node2> s_buf2;
            static Node1 *data1() { return s_buf1.data(); }
            static Node2 *data2() { return s_buf2.data(); }
            static size_type newnode1() {
                s_buf1.push_back({});
                return s_buf1.size() - 1;
            }
            static size_type newnode2() {
                s_buf2.push_back({});
                return s_buf2.size() - 1;
            }
        };
        template <typename Node1, typename Node2>
        std::vector<Node1> VectorBuffer<Node1, Node2>::s_buf1{Node1{}};
        template <typename Node1, typename Node2>
        std::vector<Node2> VectorBuffer<Node1, Node2>::s_buf2{Node2{}};
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, template <typename, typename> typename BufferType = VectorBuffer>
        struct Tree {
            struct node : Node {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr1(0); }
                node *lchild() const { return _ptr1(m_lc); }
                node *rchild() const { return _ptr1(m_rc); }
            };
            using value_type = typename Node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            struct treenode {
                size_type m_id, m_lc, m_rc;
                bool is_null() const { return this == _ptr2(0); }
                node *root() const { return _ptr1(m_id); }
                treenode *lchild() const { return _ptr2(m_lc); }
                treenode *rchild() const { return _ptr2(m_rc); }
            };
            using buffer_type = BufferType<node, treenode>;
            size_type m_tree_root;
            SizeType m_row, m_column;
            static node *_ptr1(size_type cur) { return buffer_type::data1() + cur; }
            static treenode *_ptr2(size_type cur) { return buffer_type::data2() + cur; }
            static void _reserve(size_type node_capacity, size_type treenode_capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node, treenode>>::value, "Only In Vector Mode");
                buffer_type::s_buf1.reserve(node_capacity), buffer_type::s_buf2.reserve(treenode_capacity);
            }
            static size_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = buffer_type::newnode1();
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) _ptr1(c)->set(RangeMapping()(row_floor, row_ceil, column_floor, column_ceil));
                return c;
            }
            static size_type _newtreenode(SizeType row_floor, SizeType row_ceil) { return buffer_type::newnode2(); }
            static void _apply(node *x, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type>::value)
                    node::map(modify, x);
                else
                    x->set(node::op(modify, x->get()));
            }
            static size_type _init_lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = _newnode(row_floor, row_ceil, column_floor, column_ceil);
                return _ptr1(cur)->m_lc = c;
            }
            static size_type _init_rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = _newnode(row_floor, row_ceil, column_floor, column_ceil);
                return _ptr1(cur)->m_rc = c;
            }
            template <typename InitMapping>
            static void _initnode(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, InitMapping &&mapping) {
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) _ptr1(cur)->set(mapping(row, column_floor));
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _initnode(_init_lchild(cur, row, row, column_floor, mid), row, column_floor, mid, mapping);
                    _initnode(_init_rchild(cur, row, row, mid + 1, column_ceil), row, mid + 1, column_ceil, mapping);
                    _ptr1(cur)->set(node::op(_ptr1(cur)->lchild()->get(), _ptr1(cur)->rchild()->get()));
                }
            }
            template <typename InitMapping>
            static void _initnode(size_type cur, size_type up_child, size_type down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _initnode(_init_lchild(cur, row_floor, row_ceil, column_floor, mid), _ptr1(up_child)->m_lc, _ptr1(down_child)->m_lc, row_floor, row_ceil, column_floor, mid, mapping);
                    _initnode(_init_rchild(cur, row_floor, row_ceil, mid + 1, column_ceil), _ptr1(up_child)->m_rc, _ptr1(down_child)->m_rc, row_floor, row_ceil, mid + 1, column_ceil, mapping);
                }
                _ptr1(cur)->set(node::op(_ptr1(up_child)->get(), _ptr1(down_child)->get()));
            }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr1(cur)->m_lc) {
                        size_type c = _newnode(row_floor, row_floor, column_floor, mid);
                        _ptr1(cur)->m_lc = c;
                    }
                return _ptr1(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr1(cur)->m_rc) {
                        size_type c = _newnode(row_floor, row_floor, mid + 1, column_ceil);
                        _ptr1(cur)->m_rc = c;
                    }
                return _ptr1(cur)->m_rc;
            }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr2(cur)->m_lc) {
                        size_type c = _newtreenode(row_floor, mid);
                        _ptr2(cur)->m_lc = c;
                    }
                return _ptr2(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr2(cur)->m_rc) {
                        size_type c = _newtreenode(mid + 1, row_ceil);
                        _ptr2(cur)->m_rc = c;
                    }
                return _ptr2(cur)->m_rc;
            }
            static void _pushup_left_right(size_type cur, size_type lc, size_type rc, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                node *p = _ptr1(cur), *l = _ptr1(lc), *r = _ptr1(rc);
                if (lc && rc) return p->set(node::op(l->get(), r->get()));
                value_type lval, rval;
                if (lc)
                    lval = l->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    lval = RangeMapping()(row_floor, row_ceil, column_floor, mid);
                else
                    lval = _ptr1(0)->get();
                if (rc)
                    rval = r->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    rval = RangeMapping()(row_floor, row_ceil, mid + 1, column_ceil);
                else
                    rval = _ptr1(0)->get();
                p->set(node::op(lval, rval));
            }
            static void _pushup_up_down(size_type cur, size_type uc, size_type dc, SizeType row_floor, SizeType row_ceil, SizeType mid, SizeType column_floor, SizeType column_ceil) {
                node *p = _ptr1(cur), *u = _ptr1(uc), *d = _ptr1(dc);
                if (uc && dc) return p->set(node::op(u->get(), d->get()));
                value_type up_val, down_val;
                if (uc)
                    up_val = u->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    up_val = RangeMapping()(row_floor, mid, column_floor, column_ceil);
                else
                    up_val = _ptr1(0)->get();
                if (dc)
                    down_val = d->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    down_val = RangeMapping()(mid + 1, row_ceil, column_floor, column_ceil);
                else
                    down_val = _ptr1(0)->get();
                p->set(node::op(up_val, down_val));
            }
            static void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j, const modify_type &modify) {
                _apply(_ptr1(cur), modify);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, column_floor, mid, j, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, mid + 1, column_ceil, j, modify);
                }
            }
            static void _modify(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &val) {
                if (column_floor == column_ceil)
                    _ptr1(cur)->set(val);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _modify(_lchild(cur, row, row, column_floor, column_ceil, mid), row, column_floor, mid, j, val);
                    else
                        _modify(_rchild(cur, row, row, column_floor, column_ceil, mid), row, mid + 1, column_ceil, j, val);
                    _pushup_left_right(cur, _ptr1(cur)->m_lc, _ptr1(cur)->m_rc, row, row, column_floor, column_ceil, mid);
                }
            }
            static void _modify(size_type cur, size_type up_child, size_type down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j) {
                _pushup_up_down(cur, up_child, down_child, row_floor, row_ceil, (row_floor + row_ceil) >> 1, column_floor, column_ceil);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _modify(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), _ptr1(up_child)->m_lc, _ptr1(down_child)->m_lc, row_floor, row_ceil, column_floor, mid, j);
                    else
                        _modify(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), _ptr1(up_child)->m_rc, _ptr1(down_child)->m_rc, row_floor, row_ceil, mid + 1, column_ceil, j);
                }
            }
            static value_type _query(SizeType i, SizeType j) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return _ptr1(0)->get();
                else
                    return RangeMapping()(i, i, j, j);
            }
            static value_type _query(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j) {
                if (!cur) return _query(row, j);
                node *p = _ptr1(cur);
                if (column_floor == column_ceil) return p->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                return j <= mid ? _query(p->m_lc, row, column_floor, mid, j) : _query(p->m_rc, row, mid + 1, column_ceil, j);
            }
            static value_type _query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return _ptr1(0)->get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            static value_type _query(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) {
                if (!cur) return _query(row_floor, row_ceil, column1, column2);
                node *p = _ptr1(cur);
                if (column1 == column_floor && column2 == column_ceil) return p->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column1 > mid) return _query(p->m_rc, row_floor, row_ceil, mid + 1, column_ceil, column1, column2);
                if (column2 <= mid) return _query(p->m_lc, row_floor, row_ceil, column_floor, mid, column1, column2);
                return node::op(_query(p->m_lc, row_floor, row_ceil, column_floor, mid, column1, mid), _query(p->m_rc, row_floor, row_ceil, mid + 1, column_ceil, mid + 1, column2));
            }
            static void _get_trees(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, node **trees, size_type &count) {
                if (row1 > row_floor || row2 < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid && cur->m_lc) _get_trees(cur->lchild(), row_floor, mid, row1, row2, trees, count);
                    if (row2 > mid && cur->m_rc) _get_trees(cur->rchild(), mid + 1, row_ceil, row1, row2, trees, count);
                } else if (cur->m_id)
                    trees[count++] = cur->root();
            }
            static SizeType _kth(node **trees, size_type &count, SizeType column_floor, SizeType column_ceil, value_type k) {
                if (column_floor == column_ceil) return column_floor;
                value_type cnt = 0;
                for (size_type i = 0; i < count; i++) cnt += trees[i]->lchild()->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (cnt > k) {
                    size_type i = 0;
                    for (size_type j = 0; j < count; j++)
                        if (trees[j]->m_lc) trees[i++] = trees[j]->lchild();
                    count = i;
                    return _kth(trees, count, column_floor, mid, k);
                } else {
                    size_type i = 0;
                    for (size_type j = 0; j < count; j++)
                        if (trees[j]->m_rc) trees[i++] = trees[j]->rchild();
                    count = i;
                    return _kth(trees, count, mid + 1, column_ceil, k - cnt);
                }
            }
            template <typename InitMapping>
            void _inittreenode(size_type cur, SizeType row_floor, SizeType row_ceil, InitMapping &&mapping) {
                size_type c = _newnode(row_floor, row_ceil, 0, m_column - 1);
                _ptr2(cur)->m_id = c;
                if (row_floor == row_ceil)
                    _initnode(_ptr2(cur)->m_id, row_floor, 0, m_column - 1, mapping);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    size_type c = _newtreenode(row_floor, mid);
                    _ptr2(cur)->m_lc = c;
                    c = _newtreenode(mid + 1, row_ceil);
                    _ptr2(cur)->m_rc = c;
                    _inittreenode(_ptr2(cur)->m_lc, row_floor, mid, mapping);
                    _inittreenode(_ptr2(cur)->m_rc, mid + 1, row_ceil, mapping);
                    _initnode(_ptr2(cur)->m_id, _ptr2(cur)->lchild()->m_id, _ptr2(cur)->rchild()->m_id, row_floor, row_ceil, 0, m_column - 1, mapping);
                }
            }
            size_type _root(size_type cur, SizeType row_floor, SizeType row_ceil) {
                if constexpr (!Complete)
                    if (!_ptr2(cur)->m_id) {
                        size_type c = _newnode(row_floor, row_ceil, 0, m_column - 1);
                        _ptr2(cur)->m_id = c;
                    }
                return _ptr2(cur)->m_id;
            }
            void _add_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const modify_type &modify) {
                _add(_root(cur, row_floor, row_ceil), row_floor, row_ceil, 0, m_column - 1, j, modify);
                if (row_floor < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid)
                        _add_tree(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, i, j, modify);
                    else
                        _add_tree(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, i, j, modify);
                }
            }
            void _modify_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const value_type &val) {
                if (row_floor == row_ceil) {
                    _modify(_root(cur, row_floor, row_ceil), i, 0, m_column - 1, j, val);
                } else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid)
                        _modify_tree(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, i, j, val);
                    else
                        _modify_tree(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, i, j, val);
                    _modify(_root(cur, row_floor, row_ceil), _ptr2(cur)->lchild()->m_id, _ptr2(cur)->rchild()->m_id, row_floor, row_ceil, 0, m_column - 1, j);
                }
            }
            value_type _query_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j) const {
                if (!cur) return _query(i, j);
                treenode *p = _ptr2(cur);
                if (row_floor == row_ceil) return _query(p->m_id, i, 0, m_column - 1, j);
                SizeType mid = (row_floor + row_ceil) >> 1;
                return i <= mid ? _query_tree(p->m_lc, row_floor, mid, i, j) : _query_tree(p->m_rc, mid + 1, row_ceil, i, j);
            }
            value_type _query_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) const {
                if (!cur) return _query(row1, row2, column1, column2);
                treenode *p = _ptr2(cur);
                if (row1 == row_floor && row2 == row_ceil) return _query(p->m_id, row_floor, row_ceil, 0, m_column - 1, column1, column2);
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row1 > mid) return _query_tree(p->m_rc, mid + 1, row_ceil, row1, row2, column1, column2);
                if (row2 <= mid) return _query_tree(p->m_lc, row_floor, mid, row1, row2, column1, column2);
                return node::op(_query_tree(p->m_lc, row_floor, mid, row1, mid, column1, column2), _query_tree(p->m_rc, mid + 1, row_ceil, mid + 1, row2, column1, column2));
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType row, SizeType column, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_tree_root = _newtreenode(0, m_row - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _inittreenode(m_tree_root, 0, m_row - 1, mapping);
                }
            }
            void add(SizeType row, SizeType column, const modify_type &modify) { _add_tree(m_tree_root, 0, m_row - 1, row, column, modify); }
            void modify(SizeType row, SizeType column, const value_type &val) { _modify_tree(m_tree_root, 0, m_row - 1, row, column, val); }
            value_type query(SizeType row, SizeType column) const { return _query_tree(m_tree_root, 0, m_row - 1, row, column); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query_tree(m_tree_root, 0, m_row - 1, row1, row2, column1, column2); }
            value_type query_all() const { return _ptr1(_ptr2(m_tree_root)->m_id)->get(); }
            SizeType kth(SizeType row1, SizeType row2, value_type k) const {
                static node *buffer[128];
                size_type count = 0;
                _get_trees(_ptr2(m_tree_root), 0, m_row - 1, row1, row2, buffer, count);
                return _kth(buffer, count, 0, m_column - 1, k);
            }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, template <typename, typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, typename SizeType, template <typename, typename> typename BufferType = Seg2D::VectorBuffer, typename Operation, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Operation>, RangeMapping, Complete, SizeType, BufferType>>
    auto make_SegTree2D(SizeType row, SizeType column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, typename SizeType, template <typename, typename> typename BufferType = Seg2D::VectorBuffer, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, RangeMapping, Complete, SizeType, BufferType>>
    auto make_SegTree2D(SizeType row, SizeType column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, typename SizeType, template <typename, typename> typename BufferType = Seg2D::VectorBuffer, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, RangeMapping, Complete, SizeType, BufferType>>
    auto make_SegTree2D(SizeType row, SizeType column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t, Seg2D::size_type MAX_TREENODE = 1 << 20, Seg2D::size_type MAX_NODE = 1 << 23>
    using StaticSegSumTree2D = Seg2D::Tree<Seg2D::BaseNode<Tp>, Seg2D::Ignore, Complete, SizeType, Seg2D::StaticBufferWrap<MAX_NODE, MAX_TREENODE>::template type>;
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t>
    using VectorSegSumTree2D = Seg2D::Tree<Seg2D::BaseNode<Tp>, Seg2D::Ignore, Complete, SizeType, Seg2D::VectorBuffer>;
}

#endif