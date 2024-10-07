/*
最后修改:
20241001
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
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace TagSEG2D {
        using size_type = uint32_t;
        template <typename Tp>
        struct AddCommutativeMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp pow(const Tp &x, size_type n) { return x * n; }
        };
        template <typename Tp>
        struct BitxorMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp pow(const Tp &x, size_type n) { return n & 1 ? x : Tp{}; }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct ValLazyMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp pow(const Tp &x, size_type) { return x; }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeMonoid, typename SizeType = uint64_t, template <typename> typename BufferType1 = VectorBufferWithoutCollect, template <typename> typename BufferType2 = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
            struct Node {
                value_type m_val, m_tag;
            };
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
            using buffer_type1 = BufferType1<node>;
            using buffer_type2 = BufferType2<treenode>;
            static void _reserve(size_type node_capacity, size_type treenode_capacity) {
                static_assert(buffer_type1::is_vector_buffer && buffer_type2::is_vector_buffer, "Only In Vector Mode");
                buffer_type1::s_buf.reserve(node_capacity), buffer_type2::s_buf.reserve(treenode_capacity);
            }
        private:
            size_type m_root;
            SizeType m_row, m_column;
            static node *_ptr1(size_type cur) { return buffer_type1::data() + cur; }
            static treenode *_ptr2(size_type cur) { return buffer_type2::data() + cur; }
            static size_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = buffer_type1::newnode();
                _ptr1(c)->value_ptr()->m_val = _ptr1(c)->value_ptr()->m_tag = _ptr1(c)->tag_ptr()->m_val = _ptr1(c)->tag_ptr()->m_tag = group::identity();
                return c;
            }
            static size_type _new_treenode(SizeType row_floor, SizeType row_ceil) { return buffer_type2::newnode(); }
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
                if (column_floor == column_ceil)
                    _ptr1(cur)->value_ptr()->m_val = mapping(row, column_floor);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _init_node(_init_lchild(cur, row, row, column_floor, mid), row, column_floor, mid, mapping);
                    _init_node(_init_rchild(cur, row, row, mid + 1, column_ceil), row, mid + 1, column_ceil, mapping);
                    _ptr1(cur)->value_ptr()->m_val = group::op(_ptr1(cur)->lchild()->value_ptr()->m_val, _ptr1(cur)->rchild()->value_ptr()->m_val);
                }
            }
            static void _init_node(size_type cur, size_type uc, size_type dc, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                *_ptr1(cur)->value_ptr() = group::op(_ptr1(uc)->value_ptr()->m_val, _ptr1(dc)->value_ptr()->m_val);
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
                if (!_ptr2(cur)->m_id) {
                    size_type c = _newnode(row_floor, row_floor, 0, m_column - 1);
                    _ptr2(cur)->m_id = c;
                }
                return _ptr2(cur)->m_id;
            }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr1(cur)->m_lc) {
                    size_type c = _newnode(row_floor, row_floor, column_floor, mid);
                    _ptr1(cur)->m_lc = c;
                }
                return _ptr1(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr1(cur)->m_rc) {
                    size_type c = _newnode(row_floor, row_floor, mid + 1, column_ceil);
                    _ptr1(cur)->m_rc = c;
                }
                return _ptr1(cur)->m_rc;
            }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if (!_ptr2(cur)->m_lc) {
                    size_type c = _new_treenode(row_floor, mid);
                    _ptr2(cur)->m_lc = c;
                }
                return _ptr2(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if (!_ptr2(cur)->m_rc) {
                    size_type c = _new_treenode(mid + 1, row_ceil);
                    _ptr2(cur)->m_rc = c;
                }
                return _ptr2(cur)->m_rc;
            }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return group::pow(modify, column2 - column1 + 1);
                if (column_floor == column1 && column_ceil == column2) return group::op(group::pow(modify, column2 - column1 + 1), cur->value_ptr()->m_val);
                modify = group::op(modify, cur->value_ptr()->m_tag);
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return group::op(_query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return modify;
                if (column_floor == column_ceil) return group::op(modify, cur->value_ptr()->m_val);
                modify = group::op(modify, cur->value_ptr()->m_tag);
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_value(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return group::pow(modify, column2 - column1 + 1);
                if (column_floor == column1 && column_ceil == column2) return group::op(group::pow(modify, column2 - column1 + 1), cur->tag_ptr()->m_val);
                modify = group::op(modify, cur->tag_ptr()->m_tag);
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return group::op(_query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return modify;
                if (column_floor == column_ceil) return group::op(modify, cur->tag_ptr()->m_val);
                modify = group::op(modify, cur->tag_ptr()->m_tag);
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return group::pow(modify, row2 - row1 + 1);
                if (row_floor == row1 && row_ceil == row2) return group::op(group::pow(modify, row2 - row1 + 1), _query_value(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, group::identity()));
                modify = group::op(modify, _query_tag(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, group::identity()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row2 <= mid)
                    return _query(cur->lchild(), row_floor, mid, row1, row2, column1, column2, std::move(modify));
                else if (row1 > mid)
                    return _query(cur->rchild(), mid + 1, row_ceil, row1, row2, column1, column2, std::move(modify));
                else
                    return group::op(_query(cur->lchild(), row_floor, mid, row1, mid, column1, column2, value_type(modify)), _query(cur->rchild(), mid + 1, row_ceil, mid + 1, row2, column1, column2, value_type(modify)));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return modify;
                if (row_floor == row_ceil) return group::op(modify, _query_value(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, group::identity()));
                modify = group::op(modify, _query_tag(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, group::identity()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row <= mid)
                    return _query(cur->lchild(), row_floor, mid, row, column, std::move(modify));
                else
                    return _query(cur->rchild(), mid + 1, row_ceil, row, column, std::move(modify));
            }
            void _add_value(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                _ptr1(cur)->value_ptr()->m_val = group::op(group::pow(modify, column2 - column1 + 1), _ptr1(cur)->value_ptr()->m_val);
                if (column_floor == column1 && column_ceil == column2)
                    _ptr1(cur)->value_ptr()->m_tag = group::op(modify, _ptr1(cur)->value_ptr()->m_tag);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_value(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                _ptr1(cur)->value_ptr()->m_val = group::op(modify, _ptr1(cur)->value_ptr()->m_val);
                if (column_floor == column_ceil)
                    _ptr1(cur)->value_ptr()->m_tag = group::op(modify, _ptr1(cur)->value_ptr()->m_tag);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add_tag(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                _ptr1(cur)->tag_ptr()->m_val = group::op(group::pow(modify, column2 - column1 + 1), _ptr1(cur)->tag_ptr()->m_val);
                if (column_floor == column1 && column_ceil == column2)
                    _ptr1(cur)->tag_ptr()->m_tag = group::op(modify, _ptr1(cur)->tag_ptr()->m_tag);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_tag(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                _ptr1(cur)->tag_ptr()->m_val = group::op(modify, _ptr1(cur)->tag_ptr()->m_val);
                if (column_floor == column_ceil)
                    _ptr1(cur)->tag_ptr()->m_tag = group::op(modify, _ptr1(cur)->tag_ptr()->m_tag);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, group::pow(modify, row2 - row1 + 1));
                if (row_floor == row1 && row_ceil == row2)
                    _add_tag(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid) _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, row1, std::min(row2, mid), column1, column2, modify);
                    if (row2 > mid) _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, std::max(row1, mid + 1), row2, column1, column2, modify);
                }
            }
            void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row, 0, m_column - 1, column, modify);
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
        public:
            Tree() = default;
            Tree(SizeType row, SizeType column) { resize(row, column); }
            template <typename InitMapping>
            Tree(SizeType row, SizeType column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(SizeType row, SizeType column) {
                if ((m_row = row) && (m_column = column)) m_root = _new_treenode(0, m_row - 1);
            }
            template <typename InitMapping>
            void resize(SizeType row, SizeType column, InitMapping mapping) {
                if ((m_row = row) && (m_column = column)) {
                    m_root = _new_treenode(0, m_row - 1);
                    _init_treenode(m_root, 0, m_row - 1, mapping);
                }
            }
            SizeType row() const { return m_row; }
            SizeType column() const { return m_column; }
            value_type query(SizeType row, SizeType column) const { return _query(_ptr2(m_root), 0, m_row - 1, row, column, group::identity()); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query(_ptr2(m_root), 0, m_row - 1, row1, row2, column1, column2, group::identity()); }
            value_type query_all() { return _ptr1(_root(m_root, 0, m_row - 1))->value_ptr()->m_val; }
            void add(SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) { _add(m_root, 0, m_row - 1, row1, row2, column1, column2, modify); }
            void add(SizeType row, SizeType column, const value_type &modify) { _add(m_root, 0, m_row - 1, row, row, column, column, modify); }
        };
        template <typename Ostream, typename CommutativeMonoid, typename SizeType, template <typename> typename BufferType1, template <typename> typename BufferType2>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeMonoid, SizeType, BufferType1, BufferType2> &x) {
            out << "[";
            for (SizeType i = 0; i != x.row(); i++)
                for (SizeType j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t>
    using VectorTagMaxSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, Minimum, TagSEG2D::ChoiceByCompare<Tp, std::less<Tp>>>, SizeType>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t>
    using VectorTagMinSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, Maximum, TagSEG2D::ChoiceByCompare<Tp, std::greater<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagGcdSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, 0, TagSEG2D::FpTransfer<Tp, std::gcd<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagLcmSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, 0, TagSEG2D::FpTransfer<Tp, std::lcm<Tp>>>, SizeType>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t>
    using VectorTagBitAndSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, OneMask, std::bit_and<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorTagBitOrSeg2D = TagSEG2D::Tree<TagSEG2D::ValLazyMonoid<Tp, ZeroMask, std::bit_or<Tp>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagBitxorSeg2D = TagSEG2D::Tree<TagSEG2D::BitxorMonoid<Tp>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagSumSeg2D = TagSEG2D::Tree<TagSEG2D::AddCommutativeMonoid<Tp>, SizeType>;
}

#endif