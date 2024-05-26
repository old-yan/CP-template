/*
最后修改:
20240526
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINERECTADDRECTSUMMAINTAINER2D__
#define __OY_OFFLINERECTADDRECTSUMMAINTAINER2D__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace OFFLINERARSM2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType>
        struct Rect {
            SizeType m_x[2], m_y[2];
            size_type m_time;
            WeightType m_w;
            WeightType weight() const { return m_w; }
        };
        template <typename SizeType>
        struct Rect<SizeType, bool> {
            SizeType m_x[2], m_y[2];
            size_type m_time;
            static constexpr bool weight() { return true; }
        };
        template <typename SizeType, typename WeightType, typename SumType = WeightType>
        struct Solver {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using rect = Rect<SizeType, WeightType>;
            using query = Rect<SizeType, bool>;
            struct node {
                SumType m_val[4];
                node &operator+=(const node &rhs) {
                    m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                    return *this;
                }
            };
            size_type m_time;
            mutable std::vector<rect> m_rects;
            std::vector<query> m_queries;
            Solver(size_type rect_cnt = 0, size_type query_cnt = 0) : m_time(0) { m_rects.reserve(rect_cnt), m_queries.reserve(query_cnt); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, WeightType w = 1) {
                if constexpr (is_bool)
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, m_time++});
                else
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, m_time++, w});
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) { m_queries.push_back({x_min, x_max + 1, y_min, y_max + 1, m_time++}); }
            template <typename BIT2D>
            std::vector<SumType> solve(BIT2D &bit) const {
                struct pair {
                    SizeType m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                std::vector<pair> ps;
                ps.resize(m_rects.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_y[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_y[1], i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                std::vector<SizeType> sorted_xs, sorted_ys;
                sorted_ys.reserve(ps.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    if (!i || ps[i].m_val != ps[i - 1].m_val) sorted_ys.push_back(ps[i].m_val);
                    m_rects[ps[i].m_index >> 1].m_y[ps[i].m_index & 1] = sorted_ys.size() - 1;
                }
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_x[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_x[1], i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                sorted_xs.reserve(ps.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    if (!i || ps[i].m_val != ps[i - 1].m_val) sorted_xs.push_back(ps[i].m_val);
                    m_rects[ps[i].m_index >> 1].m_x[ps[i].m_index & 1] = sorted_xs.size() - 1;
                }
                size_type rid = 0;
                std::vector<SumType> res;
                res.reserve(m_queries.size());
                bit.resize(sorted_xs.size(), sorted_ys.size());
                for (auto &q : m_queries) {
                    while (rid != m_rects.size() && m_rects[rid].m_time < q.m_time) {
                        auto &_rect = m_rects[rid++];
                        SizeType x0 = sorted_xs[_rect.m_x[0]], x1 = sorted_xs[_rect.m_x[1]];
                        SumType l0 = sorted_ys[_rect.m_y[0]], r0 = sorted_ys[_rect.m_y[1]], w = _rect.weight(), w2 = -w * x0, w3 = w * x1;
                        bit.add(_rect.m_x[0], _rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                        bit.add(_rect.m_x[0], _rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                        bit.add(_rect.m_x[1], _rect.m_y[0], {-l0 * w3, w3, l0 * w, -w});
                        bit.add(_rect.m_x[1], _rect.m_y[1], {r0 * w3, -w3, -r0 * w, w});
                    }
                    size_type x0 = std::lower_bound(sorted_xs.begin(), sorted_xs.end(), q.m_x[0]) - sorted_xs.begin() - 1;
                    size_type x1 = std::lower_bound(sorted_xs.begin(), sorted_xs.end(), q.m_x[1]) - sorted_xs.begin() - 1;
                    size_type y0 = std::lower_bound(sorted_ys.begin(), sorted_ys.end(), q.m_y[0]) - sorted_ys.begin() - 1;
                    size_type y1 = std::lower_bound(sorted_ys.begin(), sorted_ys.end(), q.m_y[1]) - sorted_ys.begin() - 1;
                    auto s1 = bit.presum(x1, y0), s2 = bit.presum(x1, y1), s3 = bit.presum(x0, y0), s4 = bit.presum(x0, y1);
                    SumType a = s2.m_val[0] + (SumType)s2.m_val[1] * q.m_y[1] - (s1.m_val[0] + (SumType)s1.m_val[1] * q.m_y[0]);
                    SumType b = s2.m_val[2] + (SumType)s2.m_val[3] * q.m_y[1] - (s1.m_val[2] + (SumType)s1.m_val[3] * q.m_y[0]);
                    SumType c = s4.m_val[0] + (SumType)s4.m_val[1] * q.m_y[1] - (s3.m_val[0] + (SumType)s3.m_val[1] * q.m_y[0]);
                    SumType d = s4.m_val[2] + (SumType)s4.m_val[3] * q.m_y[1] - (s3.m_val[2] + (SumType)s3.m_val[3] * q.m_y[0]);
                    res.push_back(a + q.m_x[1] * b - (c + q.m_x[0] * d));
                }
                return res;
            }
        };
    }
}

#endif