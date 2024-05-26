/*
最后修改:
20240524
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RECTADDRECTSUMCOUNTER2D__
#define __OY_RECTADDRECTSUMCOUNTER2D__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace RARSC2D {
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
        template <typename Tp, typename SizeType>
        struct BIT {
            struct node {
                Tp m_val[4];
                node &operator+=(const node &rhs) {
                    m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                    return *this;
                }
            };
            struct pair {
                SizeType m_ver;
                node m_val;
            };
            size_type m_length;
            std::vector<std::vector<pair>> m_sum;
            void resize(size_type length) { m_sum.resize(m_length = length); }
            void add(SizeType ver, size_type i, const node &inc) {
                for (; i < m_length; i += (i + 1) & (-i - 1))
                    if (m_sum[i].empty())
                        m_sum[i].push_back({ver, inc});
                    else {
                        if (m_sum[i].back().m_ver != ver) m_sum[i].push_back({ver, m_sum[i].back().m_val});
                        m_sum[i].back().m_val += inc;
                    }
            };
            node presum(SizeType ver, size_type i) const {
                node res{};
                for (; ~i; i -= (i + 1) & (-i - 1))
                    if (!m_sum[i].empty()) {
                        size_type l = 0, r = m_sum[i].size();
                        while (l != r) {
                            size_type mid = (l + r) / 2;
                            if (m_sum[i][mid].m_ver >= ver)
                                r = mid;
                            else
                                l = mid + 1;
                        }
                        if (l) res += m_sum[i][l - 1].m_val;
                    }
                return res;
            }
        };
        template <typename SizeType, typename WeightType, typename SumType = WeightType>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using rect = Rect<SizeType, WeightType>;
            std::vector<rect> m_rects;
            std::vector<SizeType> m_sorted_ys;
            BIT<SumType, SizeType> m_bit;
            Table(size_type rect_cnt = 0) { m_rects.reserve(rect_cnt); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, WeightType w = 1) {
                if constexpr (is_bool)
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1});
                else
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max + 1, w});
            }
            void prepare() {
                struct pair {
                    SizeType m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                std::vector<pair> ps(m_rects.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_y[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_y[1], i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                m_sorted_ys.reserve(ps.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    if (!i || ps[i].m_val != ps[i - 1].m_val) m_sorted_ys.push_back(ps[i].m_val);
                    m_rects[ps[i].m_index >> 1].m_y[ps[i].m_index & 1] = m_sorted_ys.size() - 1;
                }
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_x[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_x[1], i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                m_bit.resize(m_sorted_ys.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    auto &rect = m_rects[ps[i].m_index >> 1];
                    SumType l0 = m_sorted_ys[rect.m_y[0]], r0 = m_sorted_ys[rect.m_y[1]], w = ps[i].m_index & 1 ? -(SumType)rect.weight() : rect.weight(), w2 = -w * rect.m_x[ps[i].m_index & 1];
                    m_bit.add(ps[i].m_val, rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                    m_bit.add(ps[i].m_val, rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                }
            }
            SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                size_type y0 = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_min) - m_sorted_ys.begin();
                size_type y1 = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), y_max + 1) - m_sorted_ys.begin();
                auto s1 = m_bit.presum(x_max + 1, y0 - 1), s2 = m_bit.presum(x_max + 1, y1 - 1), s3 = m_bit.presum(x_min, y0 - 1), s4 = m_bit.presum(x_min, y1 - 1);
                SumType a = s2.m_val[0] + (SumType)s2.m_val[1] * (y_max + 1) - (s1.m_val[0] + (SumType)s1.m_val[1] * y_min);
                SumType b = s2.m_val[2] + (SumType)s2.m_val[3] * (y_max + 1) - (s1.m_val[2] + (SumType)s1.m_val[3] * y_min);
                SumType c = s4.m_val[0] + (SumType)s4.m_val[1] * (y_max + 1) - (s3.m_val[0] + (SumType)s3.m_val[1] * y_min);
                SumType d = s4.m_val[2] + (SumType)s4.m_val[3] * (y_max + 1) - (s3.m_val[2] + (SumType)s3.m_val[3] * y_min);
                return a + (x_max + 1) * b - (c + x_min * d);
            }
        };
    }
}

#endif