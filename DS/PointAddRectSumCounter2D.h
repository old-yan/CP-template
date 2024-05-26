/*
最后修改:
20240506
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_POINTADDRECTSUMCOUNTER2D__
#define __OY_POINTADDRECTSUMCOUNTER2D__

#include "WaveLet.h"

namespace OY {
    namespace PARSC2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType, bool HasModify>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
        };
        template <typename SizeType, typename WeightType>
        struct Point<SizeType, WeightType, true> {
            SizeType m_x, m_y;
            size_type m_index;
            WeightType m_w;
        };
        template <typename SizeType>
        struct Point<SizeType, bool, false> {
            SizeType m_x, m_y;
        };
        template <typename SizeType>
        struct Point<SizeType, bool, true> {
            SizeType m_x, m_y;
            size_type m_index;
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
        template <typename Tp>
        struct SimpleBIT {
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length);
                for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                for (size_type i = 0; i != length; i++) {
                    size_type j = i + _lowbit(i + 1);
                    if (j < length) m_sum[j] += m_sum[i];
                }
            }
            void add(size_type i, Tp inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
        };
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, bool HasModify = false>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using weight_type = typename std::conditional<is_bool, size_type, WeightType>::type;
            using point = Point<SizeType, weight_type, HasModify>;
            using inner_table = typename std::conditional<HasModify, SimpleBIT<SumType>, AdjTable<SumType>>::type;
            using wavelet = WaveLet::Table<size_type, inner_table>;
            std::vector<point> m_points;
            std::vector<SizeType> m_sorted_xs, m_sorted_ys;
            std::vector<size_type> m_point_pos;
            wavelet m_table;
            Table() = default;
            Table(size_type point_cnt) { m_points.reserve(point_cnt); }
            void add_point(SizeType x, SizeType y, weight_type w = {1}) {
                if constexpr (is_bool)
                    if constexpr (HasModify)
                        m_points.push_back({x, y, size_type(m_points.size())});
                    else
                        m_points.push_back({x, y});
                else if constexpr (HasModify)
                    m_points.push_back({x, y, size_type(m_points.size()), w});
                else
                    m_points.push_back({x, y, w});
            }
            void prepare() {
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                m_sorted_ys.reserve(m_points.size());
                for (auto &p : m_points) {
                    if (m_sorted_ys.empty() || m_sorted_ys.back() != p.m_y) m_sorted_ys.push_back(p.m_y);
                    p.m_y = m_sorted_ys.size() - 1;
                }
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                auto mapping = [&](size_type i) { return m_points[i].m_y; };
                auto table_mapping = [&](size_type, size_type i) -> SumType {
                    if constexpr (is_bool)
                        return 1;
                    else
                        return m_points[i].m_w;
                };
                m_table.resize_mapping_with_index(m_points.size(), mapping, std::bit_width(m_sorted_ys.size()), table_mapping);
                m_sorted_xs.reserve(m_points.size());
                for (auto &p : m_points) m_sorted_xs.push_back(p.m_x);
                if constexpr (HasModify) {
                    m_point_pos.resize(m_points.size());
                    for (size_type i = 0; i != m_points.size(); i++) m_point_pos[m_points[i].m_index] = i;
                }
            }
            SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                auto y1 = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_min) - m_sorted_ys.begin(), y2 = std::upper_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_max) - m_sorted_ys.begin() - 1;
                SumType res{};
                if (y1 != y2 + 1) m_table.do_for_value_range(std::lower_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_min) - m_sorted_xs.begin(), std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_max) - m_sorted_xs.begin() - 1, y1, y2, [&](SumType val) { res += val; });
                return res;
            }
            void add_point_value(size_type point_id, SumType w) {
                static_assert(HasModify, "HasModify Must Be True");
                m_table.do_in_table(m_point_pos[point_id], [w](inner_table &tr, size_type pos) { tr.add(pos, w); });
            }
        };
    };
}

#endif