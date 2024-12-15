/*
最后修改:
20241126
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SPIRAL__
#define __OY_SPIRAL__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    struct SpiralSquare {
        using side_type = uint32_t;
        using size_type = uint64_t;
        struct coordinate {
            side_type m_i, m_j;
        };
        side_type m_side;
        SpiralSquare() = default;
        SpiralSquare(side_type side) : m_side(side) {}
        void resize(side_type side) { m_side = side; }
        side_type side() const { return m_side; }
        size_type size() const { return size_type(m_side) * m_side; }
        coordinate operator[](size_type index) const {
            size_type k = (size_type(side()) - 1 - size_type(sqrtl(size() - index - 1))) >> 1, e = side() - 1 - k * 2, pre = (side() - k) * k * 4;
            if (index < pre + e) return {k, side_type(k + index - pre)};
            if (index < pre + e * 2) return {side_type(k + index - pre - e), side() - k - 1};
            if (index < pre + e * 3) return {side() - k - 1, side_type(side() - k - 1 - (index - pre - e * 2))};
            return {side_type(side() - k - 1 - (index - pre - e * 3)), k};
        }
        size_type index_of(coordinate c) const {
            size_type k = std::min({c.m_i, side() - 1 - c.m_i, c.m_j, side() - 1 - c.m_j}), e = side() - 1 - 2 * k;
            if (c.m_i == k) return size_type(side() - k) * k * 4 + c.m_j - k;
            if (c.m_j == side() - k - 1) return size_type(side() - k) * k * 4 + e + c.m_i - k;
            if (c.m_i == side() - k - 1) return size_type(side() - k) * k * 4 + e * 2 + (side() - k - 1) - c.m_j;
            return size_type(side() - k) * k * 4 + e * 3 + (side() - k - 1) - c.m_i;
        }
    };
    struct SpiralMatrix {
        using side_type = uint64_t;
        using size_type = uint64_t;
        struct coordinate {
            side_type m_i, m_j;
        };
        side_type m_row, m_column;
        SpiralMatrix() = default;
        SpiralMatrix(side_type row, side_type column) : m_row(row), m_column(column) {}
        void resize(side_type row, side_type column) { m_row = row, m_column = column; }
        side_type row() const { return m_row; }
        side_type column() const { return m_column; }
        size_type size() const { return size_type(m_row) * m_column; }
        coordinate operator[](size_type index) const {
            size_type k = (size_type(row()) + column() - 1 - size_type(sqrtl(size_type(row() + column()) * (row() + column()) - 4 * index - 4))) >> 2, e0 = column() - 1 - 2 * k, e1 = row() - 1 - 2 * k, pre = size_type(row() * 2 + column() * 2 - k * 4) * k;
            if (index < pre + e0) return {k, side_type(k + index - pre)};
            if (index < pre + e0 + e1) return {side_type(k + index - pre - e0), column() - k - 1};
            if (index < pre + e0 * 2 + e1) return {row() - k - 1, side_type(column() - k - 1 - (index - pre - e0 - e1))};
            return {side_type(row() - k - 1 - (index - pre - e0 * 2 - e1)), k};
        }
        size_type index_of(coordinate c) const {
            size_type k = std::min({c.m_i, row() - 1 - c.m_i, c.m_j, column() - 1 - c.m_j}), e0 = column() - 1 - 2 * k, e1 = row() - 1 - 2 * k;
            if (c.m_i == k) return size_type(row() * 2 + column() * 2 - k * 4) * k + c.m_j - k;
            if (c.m_j == column() - k - 1) return size_type(row() * 2 + column() * 2 - k * 4) * k + e0 + c.m_i - k;
            if (c.m_i == row() - k - 1) return size_type(row() * 2 + column() * 2 - k * 4) * k + e0 + e1 + (column() - k - 1) - c.m_j;
            return size_type(row() * 2 + column() * 2 - k * 4) * k + e0 * 2 + e1 + (row() - k - 1) - c.m_i;
        }
    };
}

#endif