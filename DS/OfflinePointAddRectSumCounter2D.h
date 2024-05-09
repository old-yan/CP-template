/*
最后修改:
20240504
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINEPOINTADDRECTSUMCOUNTER2D__
#define __OY_OFFLINEPOINTADDRECTSUMCOUNTER2D__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace OFFLINEPARSC2D {
        using size_type = uint32_t;
        template <typename Tp>
        struct SimpleBIT {
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            SimpleBIT(size_type length) : m_sum(length) {}
            void add(size_type i, Tp inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
        };
        template <typename SizeType, typename WeightType>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
        };
        template <typename SizeType>
        struct Point<SizeType, bool> {
            SizeType m_x, m_y;
        };
        template <typename SizeType, typename WeightType = bool>
        struct Solver {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using point = Point<SizeType, WeightType>;
            struct Query {
                SizeType m_x_min, m_x_max, m_y_min, m_y_max;
            };
            std::vector<point> m_points;
            std::vector<Query> m_queries;
            Solver() = default;
            Solver(size_type point_cnt, size_type query_cnt) { m_points.reserve(point_cnt), m_queries.reserve(query_cnt); }
            void add_point(SizeType x, SizeType y, WeightType w = 1) {
                if constexpr (is_bool)
                    m_points.push_back({x, y});
                else
                    m_points.push_back({x, y, w});
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) { m_queries.push_back({x_min, x_max, y_min, y_max}); }
            template <typename SumType = typename std::conditional<is_bool, size_type, WeightType>::type, typename CountTree = SimpleBIT<SumType>>
            std::vector<SumType> solve() {
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                std::vector<SizeType> ys;
                ys.reserve(m_points.size());
                for (auto &p : m_points) {
                    if (ys.empty() || ys.back() != p.m_y) ys.push_back(p.m_y);
                    p.m_y = ys.size() - 1;
                }
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                auto get_y = [&](SizeType y) { return std::lower_bound(ys.begin(), ys.end(), y) - ys.begin(); };
                struct query {
                    SizeType m_x;
                    size_type m_id, m_y_min, m_y_max;
                    bool m_isleft;
                };
                std::vector<query> qs;
                qs.reserve(m_queries.size() * 2);
                for (size_type i = 0; i != m_queries.size(); i++) {
                    auto &q = m_queries[i];
                    size_type y_min = std::lower_bound(ys.begin(), ys.end(), q.m_y_min) - ys.begin();
                    size_type y_max = std::upper_bound(ys.begin(), ys.end(), q.m_y_max) - ys.begin();
                    if (y_min == y_max) continue;
                    qs.push_back({q.m_x_min, i, y_min, y_max - 1, true});
                    qs.push_back({q.m_x_max + 1, i, y_min, y_max - 1, false});
                }
                std::sort(qs.begin(), qs.end(), [](const query &x, const query &y) { return x.m_x < y.m_x; });
                std::vector<SumType> res(m_queries.size());
                CountTree cnt(ys.size());
                auto query = [&](size_type l, size_type r) { return l ? cnt.presum(r) - cnt.presum(l - 1) : cnt.presum(r); };
                size_type cur = 0, n = m_points.size();
                for (auto &q : qs) {
                    while (cur != n && m_points[cur].m_x < q.m_x) {
                        auto &p = m_points[cur++];
                        if constexpr (is_bool)
                            cnt.add(p.m_y, 1);
                        else
                            cnt.add(p.m_y, p.m_w);
                    }
                    res[q.m_id] += q.m_isleft ? -query(q.m_y_min, q.m_y_max) : query(q.m_y_min, q.m_y_max);
                }
                return res;
            }
        };
    };
}

#endif