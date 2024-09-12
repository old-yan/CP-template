/*
最后修改:
20240907
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGTREE2D__
#define __OY_SEGTREE2D__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace SEG2D {
        using size_type = uint32_t;
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseCommutativeMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            template <typename SizeType>
            static constexpr Tp get(SizeType, SizeType, SizeType, SizeType) { return Identity; }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeMonoid, typename SizeType, template <typename> typename BufferType1 = VectorBufferWithoutCollect, template <typename> typename BufferType2 = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val;
                size_type m_lc, m_rc;
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
            size_type m_tree_root;
            SizeType m_row, m_column;
            static node *_ptr1(size_type cur) { return buffer_type1::data() + cur; }
            static treenode *_ptr2(size_type cur) { return buffer_type2::data() + cur; }
            static size_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                size_type c = buffer_type1::newnode();
                _ptr1(c)->m_val = group::get(row_floor, row_ceil, column_floor, column_ceil);
                return c;
            }
            static size_type _newtreenode(SizeType row_floor, SizeType row_ceil) { return buffer_type2::newnode(); }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr1(cur)->m_lc) {
                    size_type c = _newnode(row_floor, row_ceil, column_floor, mid);
                    _ptr1(cur)->m_lc = c;
                }
                return _ptr1(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr1(cur)->m_rc) {
                    size_type c = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                    _ptr1(cur)->m_rc = c;
                }
                return _ptr1(cur)->m_rc;
            }
            static size_type _lchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if (!_ptr2(cur)->m_lc) {
                    size_type c = _newtreenode(row_floor, mid);
                    _ptr2(cur)->m_lc = c;
                }
                return _ptr2(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if (!_ptr2(cur)->m_rc) {
                    size_type c = _newtreenode(mid + 1, row_ceil);
                    _ptr2(cur)->m_rc = c;
                }
                return _ptr2(cur)->m_rc;
            }
            static void _pushup_left_right(size_type cur, size_type lc, size_type rc, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) { _ptr1(cur)->m_val = group::op(lc ? _ptr1(lc)->m_val : group::get(row_floor, row_ceil, column_floor, mid), rc ? _ptr1(rc)->m_val : group::get(row_floor, row_ceil, mid + 1, column_ceil)); }
            static void _pushup_up_down(size_type cur, size_type uc, size_type dc, SizeType row_floor, SizeType row_ceil, SizeType mid, SizeType column_floor, SizeType column_ceil) { _ptr1(cur)->m_val = group::op(uc ? _ptr1(uc)->m_val : group::get(row_floor, mid, column_floor, column_ceil), dc ? _ptr1(dc)->m_val : group::get(mid + 1, row_ceil, column_floor, column_ceil)); }
            static void _add(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &inc) {
                _ptr1(cur)->m_val = group::op(inc, _ptr1(cur)->m_val);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, column_floor, mid, j, inc);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, mid + 1, column_ceil, j, inc);
                }
            }
            static void _modify(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &val) {
                if (column_floor == column_ceil)
                    _ptr1(cur)->m_val = val;
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
            static value_type _query(SizeType i, SizeType j) { return group::get(i, i, j, j); }
            static value_type _query(size_type cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j) {
                if (!cur) return _query(row, j);
                node *p = _ptr1(cur);
                if (column_floor == column_ceil) return p->m_val;
                SizeType mid = (column_floor + column_ceil) >> 1;
                return j <= mid ? _query(p->m_lc, row, column_floor, mid, j) : _query(p->m_rc, row, mid + 1, column_ceil, j);
            }
            static value_type _query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) { return group::get(row1, row2, column1, column2); }
            static value_type _query(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) {
                if (!cur) return _query(row_floor, row_ceil, column1, column2);
                node *p = _ptr1(cur);
                if (column1 == column_floor && column2 == column_ceil) return p->m_val;
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column1 > mid) return _query(p->m_rc, row_floor, row_ceil, mid + 1, column_ceil, column1, column2);
                if (column2 <= mid) return _query(p->m_lc, row_floor, row_ceil, column_floor, mid, column1, column2);
                return group::op(_query(p->m_lc, row_floor, row_ceil, column_floor, mid, column1, mid), _query(p->m_rc, row_floor, row_ceil, mid + 1, column_ceil, mid + 1, column2));
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
                for (size_type i = 0; i < count; i++) cnt += trees[i]->lchild()->m_val;
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
            size_type _root(size_type cur, SizeType row_floor, SizeType row_ceil) {
                if (!_ptr2(cur)->m_id) {
                    size_type c = _newnode(row_floor, row_ceil, 0, m_column - 1);
                    _ptr2(cur)->m_id = c;
                }
                return _ptr2(cur)->m_id;
            }
            void _add_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const value_type &inc) {
                _add(_root(cur, row_floor, row_ceil), row_floor, row_ceil, 0, m_column - 1, j, inc);
                if (row_floor < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid)
                        _add_tree(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, i, j, inc);
                    else
                        _add_tree(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, i, j, inc);
                }
            }
            void _modify_tree(size_type cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const value_type &val) {
                if (row_floor == row_ceil)
                    _modify(_root(cur, row_floor, row_ceil), i, 0, m_column - 1, j, val);
                else {
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
                return group::op(_query_tree(p->m_lc, row_floor, mid, row1, mid, column1, column2), _query_tree(p->m_rc, mid + 1, row_ceil, mid + 1, row2, column1, column2));
            }
        public:
            Tree(SizeType row = 0, SizeType column = 0) { resize(row, column); }
            void resize(SizeType row, SizeType column) {
                m_row = row, m_column = column;
                m_tree_root = _newtreenode(0, m_row - 1);
            }
            SizeType row() const { return m_row; }
            SizeType column() const { return m_column; }
            void add(SizeType row, SizeType column, value_type inc) { _add_tree(m_tree_root, 0, m_row - 1, row, column, inc); }
            void modify(SizeType row, SizeType column, value_type val) { _modify_tree(m_tree_root, 0, m_row - 1, row, column, val); }
            value_type query(SizeType row, SizeType column) const { return _query_tree(m_tree_root, 0, m_row - 1, row, column); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query_tree(m_tree_root, 0, m_row - 1, row1, row2, column1, column2); }
            value_type query_all() const { return _root(_ptr2(m_tree_root)->m_id)->m_val; }
            SizeType kth(SizeType row1, SizeType row2, value_type k) const {
                static node *buffer[128];
                size_type count = 0;
                _get_trees(_ptr2(m_tree_root), 0, m_row - 1, row1, row2, buffer, count);
                return _kth(buffer, count, 0, m_column - 1, k);
            }
        };
        template <typename Ostream, typename SemiMonoid, typename SizeType, template <typename> typename BufferType1, template <typename> typename BufferType2>
        Ostream &operator<<(Ostream &out, const Tree<SemiMonoid, SizeType, BufferType1, BufferType2> &x) {
            out << "[";
            for (SizeType i = 0; i != x.row(); i++)
                for (SizeType j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t>
    using VectorMonoMaxSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, Minimum, SEG2D::ChoiceByCompare<Tp, std::less<Tp>>>, SizeType>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t>
    using VectorMonoMinSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, Maximum, SEG2D::ChoiceByCompare<Tp, std::greater<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorMonoGcdSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, 0, SEG2D::FpTransfer<Tp, std::gcd<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorMonoLcmSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, 1, SEG2D::FpTransfer<Tp, std::lcm<Tp>>>, SizeType>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t>
    using VectorSEGBITAndSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, OneMask, std::bit_and<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorSEGBITOrSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, ZeroMask, std::bit_or<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorSEGBITXorSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, ZeroMask, std::bit_xor<Tp>>, SizeType>;
    template <typename Tp, Tp Zero = Tp(), typename SizeType = uint64_t>
    using VectorSumSeg2D = SEG2D::Tree<SEG2D::BaseCommutativeMonoid<Tp, Zero, std::plus<Tp>>, SizeType>;
}

#endif