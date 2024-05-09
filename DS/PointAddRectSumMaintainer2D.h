/*
最后修改:
20240506
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_POINTADDRECTSUMMAINTAINER2D__
#define __OY_POINTADDRECTSUMMAINTAINER2D__

#include "WaveLet.h"

namespace OY {
    namespace PARSM2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
            size_type m_y2;
        };
        template <typename SizeType>
        struct Point<SizeType, bool> {
            SizeType m_x, m_y;
            size_type m_y2;
        };
        template <typename Tp>
        struct AdjTable {
            std::vector<Tp> m_sum;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length + 1);
                for (size_type i = 0; i != length; i++) m_sum[i + 1] = m_sum[i] + mapping(i);
            }
            Tp query(size_type left, size_type right) const { return m_sum[right + 1] - m_sum[left]; }
        };
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, size_type BATCH = 8>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using point = Point<SizeType, WeightType>;
            using inner_table = AdjTable<SumType>;
            using wavelet = WaveLet::Table<size_type, inner_table>;
            struct static_table {
                std::vector<point> m_points;
                std::vector<SizeType> m_sorted_xs, m_sorted_ys;
                wavelet m_table;
                static_table(size_type point_cnt) { m_points.reserve(point_cnt); }
                void clear() { m_points.clear(), m_sorted_xs.clear(), m_sorted_ys.clear(); }
                void add_point(SizeType x, SizeType y, WeightType w = 1) {
                    if constexpr (is_bool)
                        m_points.push_back({x, y});
                    else
                        m_points.push_back({x, y, w});
                }
                void prepare() {
                    std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                    m_sorted_ys.reserve(m_points.size());
                    for (auto &p : m_points) {
                        if (m_sorted_ys.empty() || m_sorted_ys.back() != p.m_y) m_sorted_ys.push_back(p.m_y);
                        p.m_y2 = m_sorted_ys.size() - 1;
                    }
                    std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                    auto mapping = [&](size_type i) { return m_points[i].m_y2; };
                    auto table_mapping = [&](size_type, size_type i) -> SumType {
                        if constexpr (is_bool)
                            return 1;
                        else
                            return m_points[i].m_w;
                    };
                    m_table.resize_mapping_with_index(m_points.size(), mapping, std::bit_width(m_sorted_ys.size()), table_mapping);
                    m_sorted_xs.reserve(m_points.size());
                    for (auto &p : m_points) m_sorted_xs.push_back(p.m_x);
                }
                SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                    auto y1 = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_min) - m_sorted_ys.begin(), y2 = std::upper_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_max) - m_sorted_ys.begin() - 1;
                    SumType res{};
                    if (y1 != y2 + 1) m_table.do_for_value_range(std::lower_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_min) - m_sorted_xs.begin(), std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_max) - m_sorted_xs.begin() - 1, y1, y2, [&](SumType val) { res += val; });
                    return res;
                }
            };
            std::vector<point> m_points;
            std::vector<static_table> m_tables;
            Table() { m_points.reserve(BATCH), m_tables.emplace_back(BATCH); }
            void add_point(SizeType x, SizeType y, WeightType w = 1) {
                if constexpr (is_bool)
                    m_points.push_back({x, y});
                else
                    m_points.push_back({x, y, w});
                if (m_points.size() == BATCH) {
                    size_type i = 0;
                    while (i != m_tables.size() && !m_tables[i].m_points.empty()) i++;
                    if (!i)
                        std::swap(m_points, m_tables[0].m_points);
                    else {
                        if (i == m_tables.size()) m_tables.emplace_back(BATCH << i);
                        for (size_type j = 0; j != i; j++) m_tables[i].m_points.insert(m_tables[i].m_points.end(), m_tables[j].m_points.begin(), m_tables[j].m_points.end()), m_tables[j].clear();
                        m_tables[i].m_points.insert(m_tables[i].m_points.end(), m_points.begin(), m_points.end()), m_points.clear();
                    }
                    m_tables[i].prepare();
                }
            }
            SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                SumType res{};
                for (auto &p : m_points)
                    if (p.m_x >= x_min && p.m_x <= x_max && p.m_y >= y_min && p.m_y <= y_max)
                        if constexpr (is_bool)
                            res++;
                        else
                            res += p.m_w;
                for (auto &table : m_tables)
                    if (table.m_points.size()) res += table.query(x_min, x_max, y_min, y_max);
                return res;
            }
        };
    };
}

#endif