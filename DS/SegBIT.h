/*
最后修改:
20240613
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGBIT__
#define __OY_SEGBIT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace SegBIT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_use_cnt++; }
                static size_type newnode(size_type length) {
                    s_use_cnt += length;
                    return s_use_cnt - length;
                }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static size_type newnode(size_type length) {
                s_buf.resize(s_buf.size() + length);
                return s_buf.size() - length;
            }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, template <typename> typename BufferType>
        struct Tree {
            struct node : Node {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            using value_type = typename node::value_type;
            size_type m_tree_root, m_row;
            SizeType m_column;
            static SizeType _lowbit(SizeType x) { return x & -x; }
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _newnode(size_type row, SizeType column_floor, SizeType column_ceil) {
                size_type c = _newnode();
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) _ptr(c)->set(RangeMapping()(row - _lowbit(row + 1) + 1, row, column_floor, column_ceil));
                return c;
            }
            static size_type _lchild0(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode(row, column_floor, column_ceil);
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild0(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode(row, column_floor, column_ceil);
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            static size_type _lchild(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr(cur)->m_lc) {
                        size_type c = _newnode(row, column_floor, mid);
                        _ptr(cur)->m_lc = c;
                    }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!_ptr(cur)->m_rc) {
                        size_type c = _newnode(row, mid + 1, column_ceil);
                        _ptr(cur)->m_rc = c;
                    }
                return _ptr(cur)->m_rc;
            }
            template <typename InitMapping>
            static void _initnode(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, InitMapping &&mapping) {
                node *p = _ptr(cur);
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) p->set(p->get() + mapping(row, column_floor));
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _initnode(_lchild0(cur, row, column_floor, mid), row, column_floor, mid, mapping);
                    _initnode(_rchild0(cur, row, mid + 1, column_ceil), row, mid + 1, column_ceil, mapping);
                    p = _ptr(cur), p->set(p->lchild()->get() + p->rchild()->get());
                }
            }
            template <typename InitMapping>
            static void _initnode(size_type cur, size_type parent, size_type row, SizeType column_floor, SizeType column_ceil, InitMapping &&mapping) {
                node *p = _ptr(cur), *q = _ptr(parent);
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) p->set(p->get() + mapping(row, column_floor));
                    q->set(q->get() + p->get());
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    _initnode(_lchild0(cur, row, column_floor, mid), _lchild0(parent, row + _lowbit(row + 1), column_floor, mid), row, column_floor, mid, mapping);
                    _initnode(_rchild0(cur, row, mid + 1, column_ceil), _rchild0(parent, row + _lowbit(row + 1), mid + 1, column_ceil), row, mid + 1, column_ceil, mapping);
                    p = _ptr(cur), p->set(p->lchild()->get() + p->rchild()->get());
                }
            }
            static void _add(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &modify) {
                _ptr(cur)->set(_ptr(cur)->get() + modify);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row, column_floor, column_ceil, mid), row, column_floor, mid, j, modify);
                    else
                        _add(_rchild(cur, row, column_floor, column_ceil, mid), row, mid + 1, column_ceil, j, modify);
                }
            }
            static value_type _modify(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &val) {
                value_type inc;
                if (column_floor == column_ceil)
                    inc = val - _ptr(cur)->get();
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        inc = _modify(_lchild(cur, row, column_floor, column_ceil, mid), row, column_floor, mid, j, val);
                    else
                        inc = _modify(_rchild(cur, row, column_floor, column_ceil, mid), row, mid + 1, column_ceil, j, val);
                }
                _ptr(cur)->set(_ptr(cur)->get() + inc);
                return inc;
            }
            static value_type _query(size_type row1, size_type row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return _ptr(0)->get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            static value_type _query(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType j) {
                if (!cur) return _query(row - _lowbit(row + 1) + 1, row, j, j);
                node *p = _ptr(cur);
                if (column_floor == column_ceil) return p->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                return j <= mid ? _query(p->m_lc, row, column_floor, mid, j) : _query(p->m_rc, row, mid + 1, column_ceil, j);
            }
            static value_type _query(size_type cur, size_type row, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) {
                if (!cur) return _query(row - _lowbit(row + 1) + 1, row, column1, column2);
                node *p = _ptr(cur);
                if (column1 == column_floor && column2 == column_ceil) return p->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column1 > mid) return _query(p->m_rc, row, mid + 1, column_ceil, column1, column2);
                if (column2 <= mid) return _query(p->m_lc, row, column_floor, mid, column1, column2);
                return _query(p->m_lc, row, column_floor, mid, column1, mid) + _query(p->m_rc, row, mid + 1, column_ceil, mid + 1, column2);
            }
            template <typename InitMapping>
            void _inittreenode(InitMapping mapping) {
                for (size_type i = 0; i < m_row; i++) {
                    size_type j = i + _lowbit(i + 1);
                    if (j < m_row)
                        _initnode(m_tree_root + i, m_tree_root + j, i, 0, m_column - 1, mapping);
                    else
                        _initnode(m_tree_root + i, i, 0, m_column - 1, mapping);
                }
            }
            template <typename InitMapping = Ignore>
            Tree(size_type row = 0, SizeType column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_tree_root = buffer_type::newnode(m_row);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _inittreenode(mapping);
                }
            }
            void add(size_type i, SizeType j, const value_type &inc) {
                while (i < m_row) _add(m_tree_root + i, i, 0, m_column - 1, j, inc), i += _lowbit(i + 1);
            }
            void modify(size_type i, SizeType j, const value_type &val) {
                while (i < m_row) _modify(m_tree_root + i, i, 0, m_column - 1, j, val), i += _lowbit(i + 1);
            }
            value_type presum(size_type i, SizeType j) const {
                value_type res{};
                while (~i) res += _query(m_tree_root + i, i, 0, m_column - 1, j), i -= _lowbit(i + 1);
                return res;
            }
            value_type query(size_type i, SizeType j) const {
                const size_type rend = i - _lowbit(i + 1);
                value_type res{};
                for (size_type k = i; k != rend; k -= _lowbit(k + 1)) res += _query(m_tree_root + k, k, 0, m_column - 1, j);
                for (size_type k = i - 1; k != rend; k -= _lowbit(k + 1)) res -= _query(m_tree_root + k, k, 0, m_column - 1, j);
                return res;
            }
            value_type presum(size_type i, SizeType column1, SizeType column2) const {
                value_type res{};
                while (~i) res += _query(m_tree_root + i, i, 0, m_column - 1, column1, column2), i -= _lowbit(i + 1);
                return res;
            }
            value_type query(size_type row1, size_type row2, SizeType column1, SizeType column2) const {
                const size_type rend = (row1 & ~(std::bit_ceil((row1 ^ (row2 + 1)) + 1) - 1)) - 1;
                value_type res{};
                for (size_type k = row2; k != rend; k -= _lowbit(k + 1)) res += _query(m_tree_root + k, k, 0, m_column - 1, column1, column2);
                for (size_type k = row1 - 1; k != rend; k -= _lowbit(k + 1)) res -= _query(m_tree_root + k, k, 0, m_column - 1, column1, column2);
                return res;
            }
            size_type kth(size_type row1, size_type row2, value_type k) const {
                static size_type buffer_plus[64], buffer_minus[64];
                size_type count_plus = 0, count_minus = 0, low = 0, high = m_column - 1, rend = (row1 & ~(std::bit_ceil((row1 ^ (row2 + 1)) + 1) - 1)) - 1;
                for (size_type k = row2; k != rend; k -= _lowbit(k + 1)) buffer_plus[count_plus++] = m_tree_root + k;
                for (size_type k = row1 - 1; k != rend; k -= _lowbit(k + 1)) buffer_minus[count_minus++] = m_tree_root + k;
                while (low < high) {
                    value_type cnt = 0;
                    for (size_type i = 0; i < count_plus; i++) cnt += _ptr(buffer_plus[i])->lchild()->get();
                    for (size_type i = 0; i < count_minus; i++) cnt -= _ptr(buffer_minus[i])->lchild()->get();
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
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, BufferType> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t, SegBIT::size_type BUFFER = 1 << 22>
    using StaticSegBITSumTree = SegBIT::Tree<SegBIT::BaseNode<Tp>, SegBIT::Ignore, Complete, SizeType, SegBIT::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, bool Complete = false, typename SizeType = uint32_t>
    using VectorSegBITSumTree = SegBIT::Tree<SegBIT::BaseNode<Tp>, SegBIT::Ignore, Complete, SizeType, SegBIT::VectorBuffer>;
}

#endif