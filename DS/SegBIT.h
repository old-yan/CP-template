/*
最后修改:
20240907
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGBIT__
#define __OY_SEGBIT__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace SEGBIT {
        using size_type = uint32_t;
        template <typename SizeType>
        inline SizeType lowbit(SizeType x) { return x & -x; }
        template <typename SizeType>
        inline SizeType meet(SizeType a, SizeType b) { return ((a + 1) & -(SizeType(1) << std::bit_width((a + 1) ^ (b + 1)))) - 1; }
        struct Self {
            template <typename Tp>
            Tp operator()(const Tp &x) const { return x; }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            template <typename SizeType>
            static constexpr Tp get(size_type, size_type, SizeType, SizeType) { return Identity; }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Inverse>
        struct BaseGroup {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            template <typename SizeType>
            static constexpr Tp get(size_type, size_type, SizeType, SizeType) { return Identity; }
            static value_type inverse(const value_type &x) { return Inverse()(x); }
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
        template <typename CommutativeGroup, typename SizeType, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_tree_root, m_row;
            SizeType m_column;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode(size_type row, SizeType column_floor, SizeType column_ceil) {
                size_type c = buffer_type::newnode();
                _ptr(c)->m_val = group::template get<SizeType>(row - lowbit(row + 1) + 1, row, column_floor, column_ceil);
                return c;
            }
            static size_type _lchild(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode(row, column_floor, mid);
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode(row, mid + 1, column_ceil);
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            static void _add(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType j, value_type modify) {
                _ptr(cur)->m_val = group::op(modify, _ptr(cur)->m_val);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row, column_floor, column_ceil, mid), row, column_floor, mid, j, modify);
                    else
                        _add(_rchild(cur, row, column_floor, column_ceil, mid), row, mid + 1, column_ceil, j, modify);
                }
            }
            static value_type _query(size_type row1, size_type row2, SizeType column1, SizeType column2) { return group::template get<SizeType>(row1, row2, column1, column2); }
            static value_type _query(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType j) {
                if (!cur) return _query(row - lowbit(row + 1) + 1, row, j, j);
                node *p = _ptr(cur);
                if (column_floor == column_ceil) return p->m_val;
                SizeType mid = (column_floor + column_ceil) >> 1;
                return j <= mid ? _query(p->m_lc, row, column_floor, mid, j) : _query(p->m_rc, row, mid + 1, column_ceil, j);
            }
            static value_type _query(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) {
                if (!cur) return _query(row - lowbit(row + 1) + 1, row, column1, column2);
                node *p = _ptr(cur);
                if (column1 == column_floor && column2 == column_ceil) return p->m_val;
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column1 > mid) return _query(p->m_rc, row, mid + 1, column_ceil, column1, column2);
                if (column2 <= mid) return _query(p->m_lc, row, column_floor, mid, column1, column2);
                return group::op(_query(p->m_lc, row, column_floor, mid, column1, mid), _query(p->m_rc, row, mid + 1, column_ceil, mid + 1, column2));
            }
        public:
            Tree(size_type row = 0, SizeType column = 0) { resize(row, column); }
            void resize(size_type row, SizeType column) {
                m_row = row, m_column = column;
                m_tree_root = buffer_type::newnode(m_row);
                for (size_type i = 0; i != m_row; i++) _ptr(m_tree_root + i)->m_val = group::template get<SizeType>(i - lowbit(i + 1) + 1, i, 0, m_column - 1);
            }
            size_type row() const { return m_row; }
            SizeType column() const { return m_column; }
            void add(size_type i, SizeType j, value_type inc) {
                while (i < m_row) _add(m_tree_root + i, i, 0, m_column - 1, j, inc), i += lowbit(i + 1);
            }
            value_type presum(size_type i, SizeType j) const {
                value_type res = group::identity();
                while (~i) res = group::op(res, _query(m_tree_root + i, i, 0, m_column - 1, j)), i -= lowbit(i + 1);
                return res;
            }
            value_type query(size_type i, SizeType j) const {
                const size_type rend = i - lowbit(i + 1);
                value_type resl = group::identity(), resr = group::identity();
                for (size_type k = i; k != rend; k -= lowbit(k + 1)) resr = group::op(resr, _query(m_tree_root + k, k, 0, m_column - 1, j));
                for (size_type k = i - 1; k != rend; k -= lowbit(k + 1)) resl = group::op(resl, _query(m_tree_root + k, k, 0, m_column - 1, j));
                return group::op(resr, group::inverse(resl));
            }
            value_type presum(size_type i, SizeType column1, SizeType column2) const {
                value_type res = group::identity();
                while (~i) res = group::op(res, _query(m_tree_root + i, i, 0, m_column - 1, column1, column2)), i -= lowbit(i + 1);
                return res;
            }
            value_type query(size_type row1, size_type row2, SizeType column1, SizeType column2) const {
                const size_type rend = (row1 & ~(std::bit_ceil((row1 ^ (row2 + 1)) + 1) - 1)) - 1;
                value_type resl = group::identity(), resr = group::identity();
                for (size_type k = row2; k != rend; k -= lowbit(k + 1)) resr = group::op(resr, _query(m_tree_root + k, k, 0, m_column - 1, column1, column2));
                for (size_type k = row1 - 1; k != rend; k -= lowbit(k + 1)) resl = group::op(resl, _query(m_tree_root + k, k, 0, m_column - 1, column1, column2));
                return group::op(resr, group::inverse(resl));
            }
            size_type kth(size_type row1, size_type row2, value_type k) const {
                static size_type buffer_plus[64], buffer_minus[64];
                size_type count_plus = 0, count_minus = 0, low = 0, high = m_column - 1, rend = (row1 & ~(std::bit_ceil((row1 ^ (row2 + 1)) + 1) - 1)) - 1;
                for (size_type k = row2; k != rend; k -= lowbit(k + 1)) buffer_plus[count_plus++] = m_tree_root + k;
                for (size_type k = row1 - 1; k != rend; k -= lowbit(k + 1)) buffer_minus[count_minus++] = m_tree_root + k;
                while (low < high) {
                    value_type cnt = 0;
                    for (size_type i = 0; i < count_plus; i++) cnt += _ptr(buffer_plus[i])->lchild()->m_val;
                    for (size_type i = 0; i < count_minus; i++) cnt -= _ptr(buffer_minus[i])->lchild()->m_val;
                    if (cnt <= k) {
                        k -= cnt, low = (low + high) / 2 + 1;
                        size_type i = 0;
                        for (size_type j = 0; j < count_plus; j++)
                            if (_ptr(buffer_plus[j])->m_rc) buffer_plus[i++] = _ptr(buffer_plus[j])->m_rc;
                        count_plus = i, i = 0;
                        for (size_type j = 0; j < count_minus; j++)
                            if (_ptr(buffer_minus[j])->m_rc) buffer_minus[i++] = _ptr(buffer_minus[j])->m_rc;
                        count_minus = i;
                    } else {
                        high = (low + high) / 2;
                        size_type i = 0;
                        for (size_type j = 0; j < count_plus; j++)
                            if (_ptr(buffer_plus[j])->m_lc) buffer_plus[i++] = _ptr(buffer_plus[j])->m_lc;
                        count_plus = i, i = 0;
                        for (size_type j = 0; j < count_minus; j++)
                            if (_ptr(buffer_minus[j])->m_lc) buffer_minus[i++] = _ptr(buffer_minus[j])->m_lc;
                        count_minus = i;
                    }
                }
                return low;
            }
        };
        template <typename Ostream, typename CommutativeGroup, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeGroup, SizeType, BufferType> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (SizeType j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t>
    using VectorMonoMaxSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, Minimum, SEGBIT::ChoiceByCompare<Tp, std::less<Tp>>>, SizeType>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t>
    using VectorMonoMinSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, Maximum, SEGBIT::ChoiceByCompare<Tp, std::greater<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorMonoGcdSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, 0, SEGBIT::FpTransfer<Tp, std::gcd<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorMonoLcmSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, 1, SEGBIT::FpTransfer<Tp, std::lcm<Tp>>>, SizeType>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t>
    using VectorSEGBITAndSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, OneMask, std::bit_and<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorSEGBITOrSegBIT = SEGBIT::Tree<SEGBIT::BaseMonoid<Tp, ZeroMask, std::bit_or<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorSEGBITXorSegBIT = SEGBIT::Tree<SEGBIT::BaseGroup<Tp, ZeroMask, std::bit_xor<Tp>, SEGBIT::Self>, SizeType>;
    template <typename Tp, Tp Zero = Tp(), typename SizeType = uint64_t>
    using VectorSumSegBIT = SEGBIT::Tree<SEGBIT::BaseGroup<Tp, Zero, std::plus<Tp>, std::negate<Tp>>, SizeType>;
}

#endif