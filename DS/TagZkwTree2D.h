/*
最后修改:
20241001
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGZKWTREE2D__
#define __OY_TAGZKWTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace TagZKW2D {
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
        template <typename CommutativeMonoid>
        class Tree {
        public:
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val, m_tag;
            };
        private:
            std::vector<node> m_value, m_tag;
            size_type m_row, m_column, m_row_capacity, m_column_capacity, m_row_depth, m_column_depth;
            static void _add_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, const value_type &modify) {
                sub[cur].m_val = group::op(group::pow(modify, column2 - column1 + 1), sub[cur].m_val);
                if (column_floor == column1 && column_ceil == column2)
                    sub[cur].m_tag = group::op(modify, sub[cur].m_tag);
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column1 <= mid) _add_value(sub, cur * 2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_value(sub, cur * 2 + 1, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            static void _add_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, const value_type &modify) {
                sub[cur].m_val = group::op(modify, sub[cur].m_val);
                if (column_floor == column_ceil)
                    sub[cur].m_tag = group::op(modify, sub[cur].m_tag);
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column <= mid)
                        _add_value(sub, cur * 2, column_floor, mid, column, modify);
                    else
                        _add_value(sub, cur * 2 + 1, mid + 1, column_ceil, column, modify);
                }
            }
            static void _add_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, const value_type &modify) {
                sub[cur].m_val = group::op(group::pow(modify, column2 - column1 + 1), sub[cur].m_val);
                if (column_floor == column1 && column_ceil == column2)
                    sub[cur].m_tag = group::op(modify, sub[cur].m_tag);
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column1 <= mid) _add_tag(sub, cur * 2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_tag(sub, cur * 2 + 1, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            static void _add_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, const value_type &modify) {
                sub[cur].m_val = group::op(modify, sub[cur].m_val);
                if (column_floor == column_ceil)
                    sub[cur].m_tag = group::op(modify, sub[cur].m_tag);
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column <= mid)
                        _add_tag(sub, cur * 2, column_floor, mid, column, modify);
                    else
                        _add_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column, modify);
                }
            }
            void _add(node *value, node *tag, size_type cur, size_type row_floor, size_type row_ceil, size_type row1, size_type row2, size_type column1, size_type column2, const value_type &modify) {
                _add_value(value + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, group::pow(modify, row2 - row1 + 1));
                if (row_floor == row1 && row_ceil == row2)
                    _add_tag(tag + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, modify);
                else {
                    size_type mid = (row_floor + row_ceil) / 2;
                    if (row1 <= mid) _add(value, tag, cur * 2, row_floor, mid, row1, std::min(row2, mid), column1, column2, modify);
                    if (row2 > mid) _add(value, tag, cur * 2 + 1, mid + 1, row_ceil, std::max(row1, mid + 1), row2, column1, column2, modify);
                }
            }
            void _add(node *value, node *tag, size_type cur, size_type row_floor, size_type row_ceil, size_type row, size_type column, const value_type &modify) {
                _add_value(value + (cur << m_column_depth + 1), 0, m_column - 1, column, modify);
                if (row_floor == row_ceil)
                    _add_tag(tag + (cur << m_column_depth + 1), 0, m_column - 1, column, modify);
                else {
                    size_type mid = (row_floor + row_ceil) >> 1;
                    if (row <= mid)
                        _add(value, tag, cur * 2, row_floor, mid, row, row, column, column, modify);
                    else
                        _add(value, tag, cur * 2 + 1, mid + 1, row_ceil, row, row, column, column, modify);
                }
            }
            static value_type _query_value(const node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, value_type &&modify) {
                if (column_floor == column1 && column_ceil == column2) return group::op(group::pow(modify, column2 - column1 + 1), sub[cur].m_val);
                modify = group::op(modify, sub[cur].m_tag);
                size_type mid = (column_floor + column_ceil) / 2;
                if (column2 <= mid)
                    return _query_value(sub, cur * 2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return group::op(_query_value(sub, cur * 2, column_floor, mid, column1, mid, value_type(modify)), _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            static value_type _query_value(const node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, value_type &&modify) {
                if (column_floor == column_ceil) return group::op(modify, sub[cur].m_val);
                modify = group::op(modify, sub[cur].m_tag);
                size_type mid = (column_floor + column_ceil) / 2;
                if (column <= mid)
                    return _query_value(sub, cur * 2, column_floor, mid, column, std::move(modify));
                else
                    return _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, column, std::move(modify));
            }
            static value_type _query_tag(const node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, value_type &&modify) {
                if (column_floor == column1 && column_ceil == column2) return group::op(group::pow(modify, column2 - column1 + 1), sub[cur].m_val);
                modify = group::op(modify, sub[cur].m_tag);
                size_type mid = (column_floor + column_ceil) / 2;
                if (column2 <= mid)
                    return _query_tag(sub, cur * 2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return group::op(_query_tag(sub, cur * 2, column_floor, mid, column1, mid, value_type(modify)), _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            static value_type _query_tag(const node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, value_type &&modify) {
                if (column_floor == column_ceil) return group::op(modify, sub[cur].m_val);
                modify = group::op(modify, sub[cur].m_tag);
                size_type mid = (column_floor + column_ceil) / 2;
                if (column <= mid)
                    return _query_tag(sub, cur * 2, column_floor, mid, column, std::move(modify));
                else
                    return _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query(size_type cur, size_type row_floor, size_type row_ceil, size_type row1, size_type row2, size_type column1, size_type column2, value_type &&modify) const {
                if (row_floor == row1 && row_ceil == row2) return group::op(group::pow(modify, row2 - row1 + 1), _query_value(m_value.data() + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, group::identity()));
                modify = group::op(modify, _query_tag(m_tag.data() + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, group::identity()));
                size_type mid = (row_floor + row_ceil) / 2;
                if (row2 <= mid)
                    return _query(cur * 2, row_floor, mid, row1, row2, column1, column2, std::move(modify));
                else if (row1 > mid)
                    return _query(cur * 2 + 1, mid + 1, row_ceil, row1, row2, column1, column2, std::move(modify));
                else
                    return group::op(_query(cur * 2, row_floor, mid, row1, mid, column1, column2, value_type(modify)), _query(cur * 2 + 1, mid + 1, row_ceil, mid + 1, row2, column1, column2, value_type(modify)));
            }
            value_type _query(size_type cur, size_type row_floor, size_type row_ceil, size_type row, size_type column, value_type &&modify) const {
                if (row_floor == row_ceil) return group::op(modify, _query_value(m_value.data() + (cur << m_column_depth + 1), 1, 0, m_column - 1, column, group::identity()));
                modify = group::op(modify, _query_tag(m_tag.data() + (cur << m_column_depth + 1), 1, 0, m_column - 1, column, group::identity()));
                size_type mid = (row_floor + row_ceil) >> 1;
                if (row <= mid)
                    return _query(cur * 2, row_floor, mid, row, column, std::move(modify));
                else
                    return _query(cur * 2 + 1, mid + 1, row_ceil, row, column, std::move(modify));
            }
        public:
            Tree() = default;
            Tree(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Tree(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(size_type row, size_type column) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                    m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                    node id{group::identity(), group::identity()};
                    m_value.assign(m_row_capacity << (m_column_depth + 2), id), m_tag.assign(m_row_capacity << (m_column_depth + 2), id);
                }
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                    m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                    node id{group::identity(), group::identity()};
                    m_value.assign(m_row_capacity << (m_column_depth + 2), id), m_tag.assign(m_row_capacity << (m_column_depth + 2), id);
                    for (size_type i = 0; i != m_row; i++) {
                        auto cur = m_value.data() + ((m_row_capacity + i) << (m_column_depth + 1));
                        for (size_type j = 0; j != m_column; j++) cur[m_column_capacity + j].m_val = mapping(i, j);
                        for (size_type j = m_column_capacity; --j;) cur[j].m_val = group::op(cur[j * 2].m_val, cur[j * 2 + 1].m_val);
                    }
                    for (size_type i = m_row_capacity; --i;) {
                        auto cur = m_value.data() + m_column_capacity * 2 * i, *l = m_value.data() + m_column_capacity * 2 * i * 2, *r = m_value.data() + m_column_capacity * 2 * (i * 2 + 1);
                        for (size_type j = m_column_capacity; j != m_column_capacity * 2; j++) cur[j].m_val = group::op(l[j].m_val, r[j].m_val);
                        for (size_type j = m_column_capacity; --j;) cur[j].m_val = group::op(cur[j * 2].m_val, cur[j * 2 + 1].m_val);
                    }
                }
            }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            value_type query(size_type row, size_type column) const { return _query(1, 0, m_row - 1, row, column, group::identity()); }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const { return _query(1, 0, m_row - 1, row1, row2, column1, column2, group::identity()); }
            value_type query_all() const { return m_value[m_column_capacity * 2 + 1].m_val; }
            void add(size_type row, size_type column, const value_type &modify) { _add(m_value.data(), m_tag.data(), 1, 0, m_row - 1, row, row, column, column, modify); }
            void add(size_type row1, size_type row2, size_type column1, size_type column2, const value_type &modify) { _add(m_value.data(), m_tag.data(), 1, 0, m_row - 1, row1, row2, column1, column2, modify); }
        };
        template <typename Ostream, typename CommutativeMonoid>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeMonoid> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using TagMaxZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, Minimum, TagZKW2D::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using TagMinZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, Maximum, TagZKW2D::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using TagGcdZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, 0, TagZKW2D::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using TagLcmZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, 0, TagZKW2D::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using TagBitAndZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using TagBitOrZkw2D = TagZKW2D::Tree<TagZKW2D::ValLazyMonoid<Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp>
    using TagBitxorZkw2D = TagZKW2D::Tree<TagZKW2D::BitxorMonoid<Tp>>;
    template <typename Tp>
    using TagSumZkw2D = TagZKW2D::Tree<TagZKW2D::AddCommutativeMonoid<Tp>>;
}

#endif