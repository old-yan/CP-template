/*
最后修改:
20240524
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINERECTADDRECTSUMCOUNTER2D__
#define __OY_OFFLINERECTADDRECTSUMCOUNTER2D__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace OFFLINERARSC2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType>
        struct Rect {
            SizeType m_x[2], m_y[2];
            WeightType m_w;
            WeightType weight() const { return m_w; }
        };
        template <typename SizeType>
        struct Rect<SizeType, bool> {
            SizeType m_x[2], m_y[2];
            static constexpr bool weight() { return true; }
        };
        template <typename SizeType>
        struct Query {
            SizeType m_x[2], m_y[2], m_y2[2];
        };
        template <typename SizeType, typename WeightType, typename SumType = WeightType>
        struct Solver {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using rect = Rect<SizeType, WeightType>;
            using query = Query<SizeType>;
            std::vector<rect> m_rects;
            std::vector<query> m_queries;
            std::vector<SizeType> m_sorted_ys;
            Solver(size_type rect_cnt = 0, size_type query_cnt = 0) { m_rects.reserve(rect_cnt), m_queries.reserve(query_cnt); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, WeightType w = 1) {
                if constexpr (is_bool)
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1});
                else
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, w});
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) { m_queries.push_back({x_min, x_max + 1, y_min, y_max + 1}); }
            std::vector<SumType> solve() {
                struct pair {
                    SizeType m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                struct node {
                    SumType m_val[4];
                    node &operator+=(const node &rhs) {
                        m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                        return *this;
                    }
                };
                std::vector<pair> ps(m_rects.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_y[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_y[1], i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                m_sorted_ys.reserve(ps.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    if (!i || ps[i].m_val != ps[i - 1].m_val) m_sorted_ys.push_back(ps[i].m_val);
                    m_rects[ps[i].m_index >> 1].m_y[ps[i].m_index & 1] = m_sorted_ys.size() - 1;
                }
                std::vector<pair> qs(m_queries.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_x[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_x[1], i * 2 + 1};
                for (size_type i = 0; i != m_queries.size(); i++) {
                    m_queries[i].m_y2[0] = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), m_queries[i].m_y[0]) - m_sorted_ys.begin();
                    m_queries[i].m_y2[1] = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), m_queries[i].m_y[1]) - m_sorted_ys.begin();
                    qs[i * 2] = {m_queries[i].m_x[0], i * 2}, qs[i * 2 + 1] = {m_queries[i].m_x[1], i * 2 + 1};
                }
                std::sort(ps.begin(), ps.end());
                std::sort(qs.begin(), qs.end());
                pair *p = ps.data(), *pend = ps.data() + ps.size();
                std::vector<node> sum(m_sorted_ys.size() + 1);
                std::vector<SumType> res(m_queries.size());
                auto add = [&](size_type i, const node &inc) {
                    for (; i < sum.size(); i += (i + 1) & (-i - 1)) sum[i] += inc;
                };
                auto presum = [&](size_type i) {
                    node res{};
                    for (; ~i; i -= (i + 1) & (-i - 1)) res += sum[i];
                    return res;
                };
                for (auto &q : qs) {
                    for (; p != pend && p->m_val < q.m_val; p++) {
                        auto &rect = m_rects[p->m_index >> 1];
                        size_type d = p->m_index & 1;
                        SumType l0 = m_sorted_ys[rect.m_y[0]], r0 = m_sorted_ys[rect.m_y[1]];
                        if (d) {
                            SumType w = -(SumType)rect.weight(), w2 = -w * rect.m_x[1];
                            add(rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                            add(rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                        } else {
                            SumType w = rect.weight(), w2 = -w * rect.m_x[0];
                            add(rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                            add(rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                        }
                    }
                    auto &qr = m_queries[q.m_index >> 1];
                    auto s1 = presum(qr.m_y2[0] - 1), s2 = presum(qr.m_y2[1] - 1);
                    SumType a = s2.m_val[0] + (SumType)s2.m_val[1] * qr.m_y[1] - (s1.m_val[0] + (SumType)s1.m_val[1] * qr.m_y[0]);
                    SumType b = s2.m_val[2] + (SumType)s2.m_val[3] * qr.m_y[1] - (s1.m_val[2] + (SumType)s1.m_val[3] * qr.m_y[0]);
                    if (q.m_index & 1)
                        res[q.m_index >> 1] += a + m_queries[q.m_index >> 1].m_x[1] * b;
                    else
                        res[q.m_index >> 1] -= a + m_queries[q.m_index >> 1].m_x[0] * b;
                }
                return res;
            }
        };
    }
}

#endif