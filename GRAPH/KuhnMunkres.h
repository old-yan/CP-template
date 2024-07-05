/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KUHNMUNKRES__
#define __OY_KUHNMUNKRES__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace KM {
        using size_type = uint32_t;
        template <typename CostType>
        struct Graph {
            struct node {
                size_type m_from_vertex;
                bool m_left_visit, m_right_visit;
            };
            size_type m_vertex_cnt;
            std::vector<size_type> m_left_match, m_right_match;
            std::vector<node> m_label;
            std::vector<CostType> m_val, m_left_label, m_right_label, m_slack;
            Graph(size_type vertex_cnt = 0, const CostType &init_value = 0) { resize(vertex_cnt, init_value); }
            void resize(size_type vertex_cnt, const CostType &init_value = 0) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_left_match.assign(m_vertex_cnt, -1), m_right_match.assign(m_vertex_cnt, -1), m_label.assign(m_vertex_cnt, {}), m_val.assign(m_vertex_cnt * m_vertex_cnt, init_value);
                m_left_label.resize(m_vertex_cnt), m_right_label.assign(m_vertex_cnt, {}), m_slack.resize(m_vertex_cnt);
            }
            void add_edge(size_type left, size_type right, const CostType &val) {
                CostType &v = m_val[left * m_vertex_cnt + right];
                v = std::max(v, val);
            }
            CostType calc(const CostType &infinite = std::numeric_limits<CostType>::max() / 2) {
                std::vector<size_type> queue_buf(m_vertex_cnt);
                size_type *queue = queue_buf.data(), head, tail;
                auto aug = [&](size_type left) {
                    size_type right = m_left_match[left];
                    if (~right) {
                        m_label[left].m_left_visit = m_label[right].m_right_visit = true;
                        queue[tail++] = right;
                        return false;
                    }
                    while (~left) std::swap(left, m_right_match[m_left_match[left] = m_label[left].m_from_vertex]);
                    return true;
                };
                auto bfs = [&](size_type start) {
                    CostType tmp;
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_label[i].m_left_visit = m_label[i].m_right_visit = false, m_slack[i] = infinite;
                    head = 0, tail = 0;
                    m_label[start].m_right_visit = true, queue[tail++] = start;
                    while (true) {
                        while (head != tail)
                            for (size_type right = queue[head++], to = 0; to != m_vertex_cnt; to++)
                                if (!m_label[to].m_left_visit && m_slack[to] >= (tmp = m_left_label[to] + m_right_label[right] - m_val[m_vertex_cnt * to + right])) {
                                    m_label[to].m_from_vertex = right;
                                    if (tmp)
                                        m_slack[to] = tmp;
                                    else if (aug(to))
                                        return;
                                }
                        tmp = infinite;
                        for (size_type left = 0; left != m_vertex_cnt; left++)
                            if (!m_label[left].m_left_visit) tmp = std::min(tmp, m_slack[left]);
                        for (size_type a = 0; a != m_vertex_cnt; a++) {
                            if (m_label[a].m_left_visit)
                                m_left_label[a] += tmp;
                            else
                                m_slack[a] -= tmp;
                            if (m_label[a].m_right_visit) m_right_label[a] -= tmp;
                        }
                        for (size_type left = 0; left != m_vertex_cnt; left++)
                            if (!m_label[left].m_left_visit && !m_slack[left] && aug(left)) return;
                    }
                };
                for (size_type left = 0; left != m_vertex_cnt; left++) m_left_label[left] = *std::max_element(m_val.data() + m_vertex_cnt * left, m_val.data() + m_vertex_cnt * (left + 1));
                for (size_type right = 0; right != m_vertex_cnt; right++) bfs(right);
                CostType res{};
                for (size_type left = 0; left != m_vertex_cnt; left++) res += m_val[m_vertex_cnt * left + m_left_match[left]];
                return res;
            }
            size_type find_left(size_type right) const { return m_right_match[right]; }
            size_type find_right(size_type left) const { return m_left_match[left]; }
            const CostType &query(size_type left, size_type right) const { return m_val[m_vertex_cnt * left + right]; }
        };
    }
}

#endif