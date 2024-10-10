/*
最后修改:
20241008
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RMQ2D__
#define __OY_RMQ2D__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RMQ2D {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <typename Tp, typename Compare, size_t MAX_LEVEL = 30>
        class Table {
            std::vector<Tp> m_frames[MAX_LEVEL];
            std::vector<mask_type> m_masks[MAX_LEVEL];
            std::vector<Tp> m_st[MAX_LEVEL];
            size_type m_row, m_column, m_st_col, m_st_stride;
            static size_type _inner_query(mask_type mask) { return std::countr_zero(mask); }
            static size_type _inner_query(mask_type mask, size_type l) { return std::countr_zero(mask & -(mask_type(1) << l)); }
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if ((m_row = row) && (m_column = column)) {
                    m_st_col = (m_column + MASK_SIZE - 1) / MASK_SIZE;
                    size_type rd = m_row == 1 ? 1 : std::bit_width(m_row - 1), cd = m_st_col == 1 ? 1 : std::bit_width(m_st_col - 1), stack[MASK_SIZE];
                    m_st_stride = m_st_col * cd;
                    auto make_mask_rmq = [&](Tp *raw, mask_type *mask_ptr, Tp *st) {
                        size_type id = 0;
                        for (id = 0; id + MASK_SIZE <= m_column; id += MASK_SIZE) {
                            auto cur_raw = raw + id;
                            mask_type *cur_mask = mask_ptr + id, mask = 0;
                            size_type len = 0;
                            for (size_type j = 0; j != MASK_SIZE; j++) {
                                while (len && Compare()(cur_raw[stack[len - 1]], cur_raw[j])) mask &= ~(mask_type(1) << stack[--len]);
                                stack[len++] = j, mask |= mask_type(1) << j;
                                cur_mask[j] = mask;
                            }
                        }
                        if (id != m_column) {
                            auto cur_raw = raw + id;
                            mask_type *cur_mask = mask_ptr + id, mask = 0;
                            size_type len = 0;
                            for (size_type j = 0, rem = m_column - id; j != rem; j++) {
                                while (len && Compare()(cur_raw[stack[len - 1]], cur_raw[j])) mask &= ~(mask_type(1) << stack[--len]);
                                stack[len++] = j, mask |= mask_type(1) << j;
                                cur_mask[j] = mask;
                            }
                        }
                        for (size_type i = 0; i != m_st_col; i++) st[i] = raw[i * MASK_SIZE + _inner_query(mask_ptr[std::min((i + 1) * MASK_SIZE, m_column) - 1])];
                        for (size_type e = 1; e != cd; e++) {
                            auto pre = st + m_st_col * (e - 1), cur = st + m_st_col * e;
                            for (size_type j1 = 0, j2 = 1 << (e - 1), k = m_st_col - (1 << e) + 1; j1 != k; j1++, j2++) cur[j1] = std::max(pre[j1], pre[j2], Compare());
                        }
                    };
                    m_frames[0].resize(m_row * m_column), m_masks[0].resize(m_row * m_column), m_st[0].resize(m_row * m_st_stride);
                    for (size_type i = 0; i != m_row; i++) {
                        auto raw = m_frames[0].data() + m_column * i;
                        for (size_type j = 0; j != m_column; j++) raw[j] = mapping(i, j);
                        make_mask_rmq(raw, m_masks[0].data() + m_column * i, m_st[0].data() + m_st_stride * i);
                    }
                    for (size_type d = 1; d != rd; d++) {
                        m_frames[d].resize((m_row - (1 << d) + 1) * m_column), m_masks[d].resize((m_row - (1 << d) + 1) * m_column), m_st[d].resize((m_row - (1 << d) + 1) * m_st_stride);
                        for (size_type i = 0, mx = m_row - (1 << d) + 1; i != mx; i++) {
                            auto raw = m_frames[d].data() + m_column * i, pre1 = m_frames[d - 1].data() + m_column * i, pre2 = m_frames[d - 1].data() + m_column * (i + (size_type(1) << (d - 1)));
                            for (size_type j = 0; j != m_column; j++) raw[j] = std::max(pre1[j], pre2[j], Compare());
                            make_mask_rmq(raw, m_masks[d].data() + m_column * i, m_st[d].data() + m_st_stride * i);
                        }
                    }
                }
            }
            Tp query(size_type i, size_type j) const { return m_frames[0][m_column * i + j]; }
            Tp query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                size_type d = std::bit_width((row2 - row1) >> 1), l = column1 / MASK_SIZE, r = column2 / MASK_SIZE;
                auto raw1 = m_frames[d].data() + m_column * row1, raw2 = m_frames[d].data() + m_column * (row2 - (size_type(1) << d) + 1);
                auto mask1 = m_masks[d].data() + m_column * row1, mask2 = m_masks[d].data() + m_column * (row2 - (size_type(1) << d) + 1);
                if (l == r)
                    return std::max(raw1[l * MASK_SIZE + _inner_query(mask1[column2], column1 % MASK_SIZE)], raw2[l * MASK_SIZE + _inner_query(mask2[column2], column1 % MASK_SIZE)], Compare());
                else if (l + 1 >= r)
                    return std::max({raw1[l * MASK_SIZE + _inner_query(mask1[(l + 1) * MASK_SIZE - 1], column1 % MASK_SIZE)], raw1[r * MASK_SIZE + _inner_query(mask1[column2])], raw2[l * MASK_SIZE + _inner_query(mask2[(l + 1) * MASK_SIZE - 1], column1 % MASK_SIZE)], raw2[r * MASK_SIZE + _inner_query(mask2[column2])]}, Compare());
                else {
                    auto st1 = m_st[d].data() + m_st_stride * row1, st2 = m_st[d].data() + m_st_stride * (row2 - (size_type(1) << d) + 1);
                    size_type e = std::bit_width((r - l - 2) >> 1);
                    return std::max({raw1[l * MASK_SIZE + _inner_query(mask1[(l + 1) * MASK_SIZE - 1], column1 % MASK_SIZE)], raw1[r * MASK_SIZE + _inner_query(mask1[column2])], raw2[l * MASK_SIZE + _inner_query(mask2[(l + 1) * MASK_SIZE - 1], column1 % MASK_SIZE)], raw2[r * MASK_SIZE + _inner_query(mask2[column2])], st1[m_st_col * e + l + 1], st1[m_st_col * e + r - (1 << e)], st2[m_st_col * e + l + 1], st2[m_st_col * e + r - (1 << e)]}, Compare());
                }
            }
            Tp query_all() const { return query(0, m_row - 1, 0, m_column - 1); }
        };
        template <typename Ostream, typename Tp, typename Compare>
        Ostream &operator<<(Ostream &out, const Table<Tp, Compare> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RMQMaxTable2D = RMQ2D::Table<Tp, std::less<Tp>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RMQMinTable2D = RMQ2D::Table<Tp, std::greater<Tp>, MAX_LEVEL>;
}

#endif