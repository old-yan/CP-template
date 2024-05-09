/*
最后修改:
20240507
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RECTADDPOINTSUMCOUNTER2D__
#define __OY_RECTADDPOINTSUMCOUNTER2D__

#include "WaveLet.h"

namespace OY {
    namespace RAPSC2D {
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
        template <typename SizeType, typename WeightType, bool HasModify>
        struct Event {
            SizeType m_x;
            size_type m_y[2];
            WeightType m_w;
        };
        template <typename SizeType, typename WeightType>
        struct Event<SizeType, WeightType, true> {
            SizeType m_x;
            size_type m_y[3];
            WeightType m_w;
        };
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, bool HasModify = false>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using weight_type = typename std::conditional<is_bool, size_type, WeightType>::type;
            struct rect {
                SizeType m_x_min, m_x_max, m_y_min, m_y_max;
                weight_type m_w;
            };
            struct pos {
                size_type m_x_min, m_x_max;
            };
            using inner_table = typename std::conditional<HasModify, SimpleBIT<SumType>, AdjTable<SumType>>::type;
            using wavelet = WaveLet::Table<size_type, inner_table>;
            std::vector<rect> m_rects;
            std::vector<SizeType> m_sorted_xs, m_sorted_ys;
            std::vector<pos> m_rect_pos;
            wavelet m_table;
            Table() = default;
            Table(size_type rect_cnt) { m_rects.reserve(rect_cnt); }
            void clear() { m_rects.clear(), m_sorted_xs.clear(), m_sorted_ys.clear(); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, weight_type w = 1) {
                m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, w});
            }
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
                    if (yp.m_y == m_rects[yp.m_index].m_y_min) m_rects[yp.m_index].m_y_min = m_sorted_ys.size() - 1;
                    if (yp.m_y == m_rects[yp.m_index].m_y_max) m_rects[yp.m_index].m_y_max = m_sorted_ys.size() - 1;
                }
                using event = Event<SizeType, weight_type, HasModify>;
                std::vector<event> es;
                es.reserve(m_rects.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) {
                    auto &rec = m_rects[i];
                    if constexpr (HasModify)
                        es.push_back({rec.m_x_min, {rec.m_y_min, rec.m_y_max, i}, rec.m_w}), es.push_back({rec.m_x_max, {rec.m_y_max, rec.m_y_min, i}, rec.m_w});
                    else
                        es.push_back({rec.m_x_min, {rec.m_y_min, rec.m_y_max}, rec.m_w}), es.push_back({rec.m_x_max, {rec.m_y_max, rec.m_y_min}, rec.m_w});
                }
                std::sort(es.begin(), es.end(), [](const event &x, const event &y) { return x.m_x < y.m_x; });
                auto mapping = [&](size_type i) { return es[i >> 1].m_y[i & 1]; };
                auto table_mapping = [&](size_type, size_type i) -> SumType { return (i & 1) ? -SumType(es[i >> 1].m_w) : es[i >> 1].m_w; };
                m_table.resize_mapping_with_index(es.size() * 2, mapping, std::bit_width(m_sorted_ys.size()), table_mapping);
                m_sorted_xs.reserve(es.size());
                for (auto &e : es) m_sorted_xs.push_back(e.m_x);
                if constexpr (HasModify) {
                    m_rect_pos.assign(m_rects.size(), {});
                    if (es.size())
                        for (size_type i = 1, j = es[0].m_y[2]; i != es.size(); i++) {
                            auto &e = es[i];
                            if (e.m_y[2] == j || m_rect_pos[e.m_y[2]].m_x_min)
                                m_rect_pos[e.m_y[2]].m_x_max = i;
                            else
                                m_rect_pos[e.m_y[2]].m_x_min = i;
                        }
                }
            }
            SumType query(SizeType x, SizeType y) const {
                auto y2 = std::upper_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y) - m_sorted_ys.begin() - 1;
                SumType res{};
                if (~y2) m_table.do_for_value_range(0, (std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x) - m_sorted_xs.begin()) * 2 - 1, 0, y2, [&](SumType val) { res += val; });
                return res;
            }
            void add_rect_value(size_type rect_id, SumType w) {
                static_assert(HasModify, "HasModify Must Be True");
                size_type x_min = m_rect_pos[rect_id].m_x_min, x_max = m_rect_pos[rect_id].m_x_max;
                m_table.do_in_table(x_min * 2, [w](inner_table &tr, size_type pos) { tr.add(pos, w); });
                m_table.do_in_table(x_min * 2 + 1, [w](inner_table &tr, size_type pos) { tr.add(pos, -w); });
                m_table.do_in_table(x_max * 2, [w](inner_table &tr, size_type pos) { tr.add(pos, w); });
                m_table.do_in_table(x_max * 2 + 1, [w](inner_table &tr, size_type pos) { tr.add(pos, -w); });
            }
        };
    };
}

#endif