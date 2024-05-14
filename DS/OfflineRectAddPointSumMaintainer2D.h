/*
最后修改:
20240508
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINERECTADDPOINTSUMMAINTAINER2D__
#define __OY_OFFLINERECTADDPOINTSUMMAINTAINER2D__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace OFFLINERAPSM2D {
        using size_type = uint32_t;
        template <typename Tp>
        struct SimpleBIT {
            size_type m_length;
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            SimpleBIT(size_type length) : m_length(length) { m_sum.resize(m_length); }
            void regard_as(size_type length) { m_length = length; }
            void add(size_type i, Tp inc) {
                while (i < m_length) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
        };
        template <typename SizeType, typename WeightType = bool>
        struct Solver {
            using weight_type = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type;
            struct rect_y {
                SizeType m_y[2];
            };
            struct event {
                bool m_isq;
                SizeType m_x;
                union {
                    struct {
                        size_type m_y2[2];
                        weight_type m_w;
                    };
                    struct {
                        size_type m_y3;
                        SizeType m_y;
                    };
                } m_data;
                size_type m_index;
            };
            struct pair_y {
                SizeType m_y;
                size_type m_index;
                bool operator<(const pair_y &rhs) const { return m_y < rhs.m_y; }
                operator size_type() const { return m_y; }
            };
            std::vector<rect_y> m_rects;
            std::vector<event> m_es;
            size_type m_query_cnt;
            void _add_rect(SizeType x, weight_type w, size_type index) {
                event e;
                e.m_isq = false, e.m_x = x, e.m_data.m_w = w, e.m_index = index;
                m_es.push_back(e);
            }
            SizeType _get_y0(size_type index) const { return m_rects[index / 2].m_y[index & 1]; }
            SizeType _get_y1(size_type index) const { return m_rects[index / 2].m_y[(index & 1) ^ 1]; }
            template <typename SumType, typename CountTree>
            void _solve(size_type l, size_type r, std::vector<SumType> &res, pair_y *yp, size_type *ys, event *buf, CountTree &cnt) {
                if (r - l < 2) return;
                size_type mid = (l + r) / 2;
                _solve(l, mid, res, yp, ys, buf, cnt), _solve(mid, r, res, yp, ys, buf, cnt);
                size_type ypcnt{}, lst = -1;
                for (size_type i = mid; i != r; i++)
                    if (m_es[i].m_isq) yp[ypcnt++] = {m_es[i].m_data.m_y, i}, lst = i;
                std::sort(yp, yp + ypcnt);
                for (size_type i = 0, j = 0; i != ypcnt; i++) {
                    if (i && yp[i].m_y != yp[i - 1].m_y) j++;
                    m_es[yp[i].m_index].m_data.m_y3 = j;
                }
                size_type ycnt = std::unique_copy(yp, yp + ypcnt, ys) - ys, l2 = l;
                if (~lst)
                    for (SizeType x_max = m_es[lst].m_x; l2 != mid; l2++)
                        if (!m_es[l2].m_isq) {
                            m_es[l2].m_data.m_y2[0] = std::lower_bound(ys, ys + ycnt, _get_y0(m_es[l2].m_index)) - ys;
                            m_es[l2].m_data.m_y2[1] = std::lower_bound(ys, ys + ycnt, _get_y1(m_es[l2].m_index)) - ys;
                        }
                if (!ycnt) {
                    auto comp = [](const event &e1, const event &e2) { return e1.m_x < e2.m_x; };
                    return std::inplace_merge(m_es.begin() + l, m_es.begin() + mid, m_es.begin() + r, comp);
                }
                cnt.regard_as(ycnt);
                size_type i1 = l, i2 = mid, i = 0;
                while (i1 != mid && i2 != r)
                    if (m_es[i1].m_x <= m_es[i2].m_x) {
                        if (!m_es[i1].m_isq) {
                            if (m_es[i1].m_data.m_y2[0] < ycnt) cnt.add(m_es[i1].m_data.m_y2[0], m_es[i1].m_data.m_w);
                            if (m_es[i1].m_data.m_y2[1] < ycnt) cnt.add(m_es[i1].m_data.m_y2[1], -(SumType)m_es[i1].m_data.m_w);
                        }
                        buf[i++] = m_es[i1++];
                    } else {
                        if (m_es[i2].m_isq) res[m_es[i2].m_index] += cnt.presum(m_es[i2].m_data.m_y3);
                        buf[i++] = m_es[i2++];
                    }
                for (; i2 != r; i2++)
                    if (m_es[i2].m_isq) res[m_es[i2].m_index] += cnt.presum(m_es[i2].m_data.m_y3);
                for (size_type i = l; i != i1; i++)
                    if (!m_es[i].m_isq) {
                        if (m_es[i].m_data.m_y2[0] < ycnt) cnt.add(m_es[i].m_data.m_y2[0], -(SumType)m_es[i].m_data.m_w);
                        if (m_es[i].m_data.m_y2[1] < ycnt) cnt.add(m_es[i].m_data.m_y2[1], m_es[i].m_data.m_w);
                    }
                std::move_backward(m_es.data() + i1, m_es.data() + mid, m_es.data() + r), std::move(buf, buf + i, m_es.data() + l);
            }
            Solver(size_type rect_cnt = 0, size_type query_cnt = 0) : m_query_cnt() { m_rects.reserve(rect_cnt), m_es.reserve(rect_cnt * 2 + query_cnt); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, weight_type w = 1) {
                _add_rect(x_min, w, m_rects.size() * 2), _add_rect(x_max + 1, w, m_rects.size() * 2 + 1);
                m_rects.push_back({{y_min, y_max + 1}});
            }
            void add_query(SizeType x, SizeType y) {
                event e;
                e.m_isq = true, e.m_x = x, e.m_data.m_y = y, e.m_index = m_query_cnt++;
                m_es.push_back(e);
            }
            template <typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, typename CountTree = SimpleBIT<SumType>>
            std::vector<SumType> solve() {
                std::vector<SumType> res(m_query_cnt);
                std::vector<pair_y> yp((m_es.size() + 1) / 2);
                std::vector<size_type> ys((m_es.size() + 1) / 2);
                std::vector<event> buf(m_es.size());
                CountTree cnt(m_es.size());
                _solve(0, m_es.size(), res, yp.data(), ys.data(), buf.data(), cnt);
                return res;
            }
        };
    };
}

#endif