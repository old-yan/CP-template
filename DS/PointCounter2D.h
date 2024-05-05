/*
最后修改:
20240505
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_POINTCOUNTER2D__
#define __OY_POINTCOUNTER2D__

#include <algorithm>
#include <cstdint>
#include <vector>

#include "WaveLet.h"

namespace OY {
    namespace PC2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
        };
        template <typename SizeType>
        struct Point<SizeType, bool> {
            SizeType m_x, m_y;
        };
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using point = Point<SizeType, WeightType>;
            struct sum_table {
                std::vector<SumType> m_sum;
                template <typename InitMapping>
                void resize(size_type length, InitMapping mapping) {
                    m_sum.resize(length + 1);
                    for (size_type i = 0; i != length; i++) m_sum[i + 1] = m_sum[i] + mapping(i);
                }
                SumType query(size_type left, size_type right) const { return m_sum[right + 1] - m_sum[left]; }
            };
            using wavelet = WaveLet::Table<size_type, sum_table>;
            std::vector<point> m_points;
            std::vector<SizeType> m_sorted_xs, m_ys;
            wavelet m_table;
            Table() = default;
            Table(size_type point_cnt) { m_points.reserve(point_cnt); }
            void add_point(SizeType x, SizeType y, WeightType w = 1) {
                if constexpr (is_bool)
                    m_points.push_back({x, y});
                else
                    m_points.push_back({x, y, w});
            }
            void prepare() {
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                m_ys.reserve(m_points.size());
                std::vector<WeightType> wtable;
                if constexpr (!is_bool) wtable.reserve(m_points.size());
                for (auto &p : m_points) {
                    m_ys.push_back(p.m_y), p.m_y = m_ys.size() - 1;
                    if constexpr (!is_bool) wtable.push_back(p.m_w);
                }
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                auto mapping = [&](size_type i) { return m_points[i].m_y; };
                auto table_mapping = [&](size_type y) -> SumType {
                    if constexpr (is_bool)
                        return 1;
                    else
                        return wtable[y];
                };
                m_table.resize(m_points.size(), mapping, std::bit_width(m_points.size()), table_mapping);
                m_sorted_xs.reserve(m_points.size());
                for (auto &p : m_points) m_sorted_xs.push_back(p.m_x);
            }
            SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                auto y1 = std::lower_bound(m_ys.begin(), m_ys.end(), y_min) - m_ys.begin(), y2 = std::upper_bound(m_ys.begin(), m_ys.end(), y_max) - m_ys.begin() - 1;
                SumType res{};
                if (y1 != y2 + 1) m_table.do_for_value_range(std::lower_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_min) - m_sorted_xs.begin(), std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_max) - m_sorted_xs.begin() - 1, y1, y2, [&](SumType val) { res += val; });
                return res;
            }
        };
    };
}

#endif