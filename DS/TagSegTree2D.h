/*
最后修改:
20231118
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGSEGTREE2D__
#define __OY_TAGSEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace TagSeg2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type applied(const value_type &x) { return x; }
            template <typename SizeType>
            static value_type applied(const value_type &x, SizeType row_width, SizeType column_width) { return x * row_width * column_width; }
            static value_type default_tag() { return value_type(); }
            value_type m_val, m_tag;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            const value_type &get_tag() const { return m_tag; }
            void set_tag(const value_type &tag) { m_tag = tag; }
            void clear_tag() {}
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
        template <typename Tp, typename NodePtr, typename ValueType, typename SizeType, typename = void>
        struct Has_applied : std::false_type {};
        template <typename Tp, typename NodePtr, typename ValueType>
        struct Has_applied<Tp, NodePtr, ValueType, void, void_t<decltype(Tp::applied(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ValueType, typename SizeType>
        struct Has_applied<Tp, NodePtr, ValueType, SizeType, void_t<decltype(Tp::applied(std::declval<ValueType>(), std::declval<SizeType>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_default_tag : std::false_type {};
        template <typename Tp>
        struct Has_default_tag<Tp, void_t<decltype(Tp::default_tag())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_clear_tag : std::false_type {};
        template <typename Tp>
        struct Has_clear_tag<Tp, void_t<decltype(std::declval<Tp>().clear_tag())>> : std::true_type {};
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
        template <typename Node, typename RangeMapping = Ignore, bool Complete = false, typename SizeType = uint64_t, template <typename, typename> typename BufferType = VectorBuffer>
        struct Tree {
            struct node {
                Node m_value, m_tag;
                size_type m_lc, m_rc;
                Node *value_ptr() { return &m_value; }
                const Node *value_ptr() const { return &m_value; }
                Node *tag_ptr() { return &m_tag; }
                const Node *tag_ptr() const { return &m_tag; }
                bool is_null() const { return this == _ptr1(0); }
                node *lchild() const { return _ptr1(m_lc); }
                node *rchild() const { return _ptr1(m_rc); }
            };
            struct treenode {
                size_type m_id, m_lc, m_rc;
                bool is_null() const { return this == _ptr2(0); }
                node *root() const { return _ptr1(m_id); }
                treenode *lchild() const { return _ptr2(m_lc); }
                treenode *rchild() const { return _ptr2(m_rc); }
            };
            using value_type = typename Node::value_type;
            using buffer_type = BufferType<node, treenode>;
            size_type m_root;
            SizeType m_row, m_column;
            static node *_ptr1(size_type cur) { return buffer_type::data1() + cur; }
            static treenode *_ptr2(size_type cur) { return buffer_type::data2() + cur; }
            static void _reserve(size_type node_capacity, size_type treenode_capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node, treenode>>::value, "Only In Vector Mode");
                buffer_type::s_buf1.reserve(node_capacity), buffer_type::s_buf2.reserve(treenode_capacity);
            }
            static size_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = buffer_type::newnode1();
                if constexpr (Has_clear_tag<Node>::value) _ptr1(c)->value_ptr()->clear_tag(), _ptr1(c)->tag_ptr()->clear_tag();
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) _ptr1(c)->value_ptr()->set(RangeMapping()(row_floor, row_ceil, column_floor, column_ceil));
                return c;
            }
            static size_type _new_treenode(SizeType row_floor, SizeType row_ceil) { return buffer_type::newnode2(); }
            static value_type _op(const value_type &x, const value_type &y) { return Node::op(x, y); }
            static value_type _default_tag() {
                if constexpr (Has_default_tag<Node>::value)
                    return Node::default_tag();
                else
                    return value_type();
            }
            static value_type _applied(const value_type &modify, SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, row2 - row1 + 1, column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_row(const value_type &modify, SizeType row1, SizeType row2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, row2 - row1 + 1, SizeType(1));
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_column(const value_type &modify, SizeType column1, SizeType column2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, SizeType(1), column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _value_of(SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return _ptr1(0)->value_ptr()->get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            static size_type _init_lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type lc = _newnode(row_floor, row_ceil, column_floor, column_ceil);
                return _ptr1(cur)->m_lc = lc;
            }
            static size_type _init_rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type rc = _newnode(row_floor, row_ceil, column_floor, column_ceil);
                return _ptr1(cur)->m_rc = rc;
            }
            static size_type _init_lchild(size_type cur, SizeType row_floor, SizeType row_ceil) {
                size_type lc = _new_treenode(row_floor, row_ceil);
                return _ptr2(cur)->m_lc = lc;
            }
            static size_type _init_rchild(size_type cur, SizeType row_floor, SizeType row_ceil) {
                size_type rc = _new_treenode(row_floor, row_ceil);
                return _ptr2(cur)->m_rc = rc;
            }
            template <typename InitMapping>
            static void _init_node(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, InitMapping &&mapping) {
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) _ptr1(cur)->value_ptr()->set(mapping(row, column_floor));
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _init_node(_init_lchild(cur, row, row, column_floor, mid), row, column_floor, mid, mapping);
                    _init_node(_init_rchild(cur, row, row, mid + 1, column_ceil), row, mid + 1, column_ceil, mapping);
                    _ptr1(cur)->value_ptr()->set(_op(_ptr1(cur)->lchild()->value_ptr()->get(), _ptr1(cur)->rchild()->value_ptr()->get()));
                }
            }
            static void _init_node(size_type cur, size_type uc, size_type dc, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                _ptr1(cur)->value_ptr()->set(_op(_ptr1(uc)->value_ptr()->get(), _ptr1(dc)->value_ptr()->get()));
                if (column_floor != column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _init_node(_init_lchild(cur, row_floor, row_ceil, column_floor, mid), _ptr1(uc)->m_lc, _ptr1(dc)->m_lc, row_floor, row_ceil, column_floor, mid);
                    _init_node(_init_rchild(cur, row_floor, row_ceil, mid + 1, column_ceil), _ptr1(uc)->m_rc, _ptr1(dc)->m_rc, row_floor, row_ceil, mid + 1, column_ceil);
                }
            }
            template <typename InitMapping>
            void _init_treenode(size_type cur, SizeType row_floor, SizeType row_ceil, InitMapping &&mapping) {
                _ptr2(cur)->m_id = _newnode(row_floor, row_ceil, 0, m_column - 1);
                if (row_floor == row_ceil)
                    _init_node(_ptr2(cur)->m_id, row_floor, 0, m_column - 1, mapping);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    _init_treenode(_init_lchild(cur, row_floor, mid), row_floor, mid, mapping);
                    _init_treenode(_init_rchild(cur, mid + 1, row_ceil), mid + 1, row_ceil, mapping);
                    _init_node(_ptr2(cur)->m_id, _ptr2(cur)->lchild()->m_id, _ptr2(cur)->rchild()->m_id, row_floor, row_ceil, 0, m_column - 1);
                }
            }
            size_type _root(size_type cur, SizeType row_floor, SizeType row_ceil) {
                if constexpr (!Complete)
                    if (!_ptr2(cur)->m_id) {
                        size_type c = _newnode(row_floor, row_floor, 0, m_column - 1);
                        _ptr2(cur)->m_id = c;
                    }
                return _ptr2(cur)->m_id;
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
                        size_type c = _new_treenode(row_floor, mid);
                        _ptr2(cur)->m_lc = c;
                    }
                return _ptr2(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr2(cur)->m_rc) {
                        size_type c = _new_treenode(mid + 1, row_ceil);
                        _ptr2(cur)->m_rc = c;
                    }
                return _ptr2(cur)->m_rc;
            }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column1, column2), _value_of(row1, row2, column1, column2));
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), cur->value_ptr()->get());
                modify = _op(modify, cur->value_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column, column), _value_of(row, row, column, column));
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), cur->value_ptr()->get());
                modify = _op(modify, cur->value_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_value(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column1, column2), _value_of(row1, row2, column1, column2));
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), cur->tag_ptr()->get());
                modify = _op(modify, cur->tag_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column, column), _value_of(row, row, column, column));
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), cur->tag_ptr()->get());
                modify = _op(modify, cur->tag_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, value_type &&row_modify) const {
                if (cur->is_null()) return _op(_applied_by_row(row_modify, row1, row2), _value_of(row1, row2, column1, column2));
                if (row_floor == row1 && row_ceil == row2) return _op(_applied_by_row(row_modify, row1, row2), _query_value(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _default_tag()));
                row_modify = _op(row_modify, _query_tag(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _default_tag()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row2 <= mid)
                    return _query(cur->lchild(), row_floor, mid, row1, row2, column1, column2, std::move(row_modify));
                else if (row1 > mid)
                    return _query(cur->rchild(), mid + 1, row_ceil, row1, row2, column1, column2, std::move(row_modify));
                else
                    return _op(_query(cur->lchild(), row_floor, mid, row1, mid, column1, column2, value_type(row_modify)), _query(cur->rchild(), mid + 1, row_ceil, mid + 1, row2, column1, column2, value_type(row_modify)));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, value_type &&row_modify) const {
                if (cur->is_null()) return _op(_applied_by_row(row_modify, row, row), _value_of(row, row, column, column));
                if (row_floor == row_ceil) return _op(_applied_by_row(row_modify, row, row), _query_value(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, _default_tag()));
                row_modify = _op(row_modify, _query_tag(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, _default_tag()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row <= mid)
                    return _query(cur->lchild(), row_floor, mid, row, column, std::move(row_modify));
                else
                    return _query(cur->rchild(), mid + 1, row_ceil, row, column, std::move(row_modify));
            }
            void _add_value(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                _ptr1(cur)->value_ptr()->set(_op(_applied_by_column(modify, column1, column2), _ptr1(cur)->value_ptr()->get()));
                if (column_floor == column1 && column_ceil == column2)
                    _ptr1(cur)->value_ptr()->set_tag(_op(modify, _ptr1(cur)->value_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_value(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                _ptr1(cur)->value_ptr()->set(_op(_applied_by_column(modify, column, column), _ptr1(cur)->value_ptr()->get()));
                if (column_floor == column_ceil)
                    _ptr1(cur)->value_ptr()->set_tag(_op(modify, _ptr1(cur)->value_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add_tag(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                _ptr1(cur)->tag_ptr()->set(_op(_applied_by_column(modify, column1, column2), _ptr1(cur)->tag_ptr()->get()));
                if (column_floor == column1 && column_ceil == column2)
                    _ptr1(cur)->tag_ptr()->set_tag(_op(modify, _ptr1(cur)->tag_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_tag(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                _ptr1(cur)->tag_ptr()->set(_op(_applied_by_column(modify, column), _ptr1(cur)->tag_ptr()->get()));
                if (column_floor == column_ceil)
                    _ptr1(cur)->tag_ptr()->set_tag(_op(modify, _ptr1(cur)->tag_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _applied_by_row(modify, row1, row2));
                if (row_floor == row1 && row_ceil == row2)
                    _add_tag(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid) _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, row1, std::min(row2, mid), column1, column2, modify);
                    if (row2 > mid) _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, std::max(row1, mid + 1), row2, column1, column2, modify);
                }
            }
            void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row, 0, m_column - 1, column, _applied_by_row(modify, row, row));
                if (row_floor == row_ceil)
                    _add_tag(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row, 0, m_column - 1, column, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, row, row, column, column, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, row, row, column, column, modify);
                }
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType row, SizeType column, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_root = _new_treenode(0, m_row - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _init_treenode(m_root, 0, m_row - 1, mapping);
                }
            }
            value_type query(SizeType row, SizeType column) const { return _query(_ptr2(m_root), 0, m_row - 1, row, column, _default_tag()); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query(_ptr2(m_root), 0, m_row - 1, row1, row2, column1, column2, _default_tag()); }
            value_type query_all() { return _ptr1(_root(m_root, 0, m_row - 1))->value_ptr()->get(); }
            void add(SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) { _add(m_root, 0, m_row - 1, row1, row2, column1, column2, modify); }
            void add(SizeType row, SizeType column, const value_type &modify) { _add(m_root, 0, m_row - 1, row, row, column, column, modify); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, template <typename, typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t, TagSeg2D::size_type NODE_BUFFER = 1 << 23, TagSeg2D::size_type TREE_BUFFER = 1 << 20>
    using StaticTagSegSumTree2D = TagSeg2D::Tree<TagSeg2D::BaseNode<Tp>, TagSeg2D::Ignore, Complete, SizeType, TagSeg2D::StaticBufferWrap<NODE_BUFFER, TREE_BUFFER>::template type>;
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t>
    using VectorTagSegSumTree2D = TagSeg2D::Tree<TagSeg2D::BaseNode<Tp>, TagSeg2D::Ignore, Complete, SizeType, TagSeg2D::VectorBuffer>;
}

#endif