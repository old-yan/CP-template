/*
最后修改:
20240507
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RECTADDPOINTSUMMAINTAINER2D__
#define __OY_RECTADDPOINTSUMMAINTAINER2D__

#include "RectAddPointSumCounter2D.h"

namespace OY {
    namespace RAPSM2D {
        using size_type = uint32_t;
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
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, size_type BATCH = 64>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using weight_type = typename std::conditional<is_bool, size_type, WeightType>::type;
            struct rect {
                SizeType m_x_min, m_x_max, m_y_min, m_y_max;
                weight_type m_w;
                size_type m_y2_min, m_y2_max;
            };
            struct static_table {
                struct pos {
                    size_type m_x_min, m_x_max;
                };
                using inner_table = AdjTable<SumType>;
                using wavelet = WaveLet::Table<size_type, inner_table>;
                std::vector<rect> m_rects;
                std::vector<SizeType> m_sorted_xs, m_sorted_ys;
                wavelet m_table;
                static_table() = default;
                static_table(size_type rect_cnt) { m_rects.reserve(rect_cnt); }
                void clear() { m_rects.clear(), m_sorted_xs.clear(), m_sorted_ys.clear(); }
                void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, weight_type w = 1) { m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, w}); }
                void prepare() {
                    struct y_pair {
                        SizeType m_y;
                        size_type m_index;
                        operator SizeType() const { return m_y; }
                    };
                    std::vector<y_pair> ys;
                    ys.reserve(m_rects.size() * 2);
                    for (size_type i = 0; i != m_rects.size(); i++) {
                        SizeType y_min = m_rects[i].m_y_min, y_max = m_rects[i].m_y_max;
                        ys.push_back({y_min, i});
                        if (y_min != y_max) ys.push_back({y_max, i});
                    }
                    std::sort(ys.begin(), ys.end(), [](const y_pair &x, const y_pair &y) { return x.m_y < y.m_y; });
                    m_sorted_ys.reserve(ys.size());
                    for (size_type i = 0; i != ys.size(); i++) {
                        auto &yp = ys[i];
                        if (m_sorted_ys.empty() || m_sorted_ys.back() != yp.m_y) m_sorted_ys.push_back(yp.m_y);
                        if (yp.m_y == m_rects[yp.m_index].m_y_min) m_rects[yp.m_index].m_y2_min = m_sorted_ys.size() - 1;
                        if (yp.m_y == m_rects[yp.m_index].m_y_max) m_rects[yp.m_index].m_y2_max = m_sorted_ys.size() - 1;
                    }
                    struct event {
                        SizeType m_x;
                        size_type m_y[2];
                        WeightType m_w;
                    };
                    std::vector<event> es;
                    es.reserve(m_rects.size() * 2);
                    for (size_type i = 0; i != m_rects.size(); i++) {
                        auto &rec = m_rects[i];
                        es.push_back({rec.m_x_min, {rec.m_y2_min, rec.m_y2_max}, rec.m_w}), es.push_back({rec.m_x_max, {rec.m_y2_max, rec.m_y2_min}, rec.m_w});
                    }
                    std::sort(es.begin(), es.end(), [](const event &x, const event &y) { return x.m_x < y.m_x; });
                    auto mapping = [&](size_type i) { return es[i >> 1].m_y[i & 1]; };
                    auto table_mapping = [&](size_type, size_type i) -> SumType { return (i & 1) ? -SumType(es[i >> 1].m_w) : es[i >> 1].m_w; };
                    m_table.resize_mapping_with_index(es.size() * 2, mapping, std::bit_width(m_sorted_ys.size()), table_mapping);
                    m_sorted_xs.reserve(es.size());
                    for (auto &e : es) m_sorted_xs.push_back(e.m_x);
                }
                SumType query(SizeType x, SizeType y) const {
                    auto y2 = std::upper_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y) - m_sorted_ys.begin() - 1;
                    SumType res{};
                    if (~y2) m_table.do_for_value_range(0, (std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x) - m_sorted_xs.begin()) * 2 - 1, 0, y2, [&](SumType val) { res += val; });
                    return res;
                }
            };
            std::vector<static_table> m_tables;
            std::vector<rect> m_rects;
            Table() { m_rects.reserve(BATCH), m_tables.emplace_back(BATCH); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, weight_type w = 1) {
                m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, w});
                if (m_rects.size() == BATCH) {
                    size_type i = 0;
                    while (i != m_tables.size() && !m_tables[i].m_rects.empty()) i++;
                    if (!i)
                        std::swap(m_rects, m_tables[0].m_rects);
                    else {
                        if (i == m_tables.size()) m_tables.emplace_back(BATCH << i);
                        for (size_type j = 0; j != i; j++) m_tables[i].m_rects.insert(m_tables[i].m_rects.end(), m_tables[j].m_rects.begin(), m_tables[j].m_rects.end()), m_tables[j].clear();
                        m_tables[i].m_rects.insert(m_tables[i].m_rects.end(), m_rects.begin(), m_rects.end()), m_rects.clear();
                    }
                    m_tables[i].prepare();
                }
            }
            SumType query(SizeType x, SizeType y) {
                SumType res{};
                for (auto &rec : m_rects)
                    if (rec.m_x_min <= x && rec.m_x_max > x && rec.m_y_min <= y && rec.m_y_max > y)
                        if constexpr (is_bool)
                            res++;
                        else
                            res += rec.m_w;
                for (auto &table : m_tables)
                    if (table.m_rects.size()) res += table.query(x, y);
                return res;
            }
        };
    };
}

#endif