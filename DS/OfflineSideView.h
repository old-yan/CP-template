/*
最后修改:
20240820
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINESIDEVIEW__
#define __OY_OFFLINESIDEVIEW__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace OFFLINESV {
        using size_type = uint32_t;
        template <typename Tp, typename Compare = std::less<Tp>>
        class Solver {
            struct modify {
                size_type m_index, m_time;
                Tp m_val;
                bool operator<(const modify &rhs) const { return m_index < rhs.m_index; }
            };
            struct query {
                size_type m_index, m_time;
                bool operator<(const query &rhs) const { return m_index < rhs.m_index; }
            };
            size_type m_timestamp, m_range;
            std::vector<modify> m_ops;
            std::vector<query> m_queries;
        public:
            Solver(size_type range) { resize(range); }
            void resize(size_type range) {
                m_timestamp = 0, m_range = range;
                m_ops.clear(), m_queries.clear();
            }
            void add_modify(size_type i, const Tp &val) { m_ops.push_back(modify{i, m_timestamp, val}); }
            void add_query(size_type i) { m_queries.push_back(query{i, m_timestamp++}); }
            std::vector<size_type> solve(const Tp &min = std::numeric_limits<Tp>::min(), const Tp &max = std::numeric_limits<Tp>::max()) {
                struct node {
                    Tp m_min[2];
                    size_type m_change_cnt;
                };
                struct Zkw {
                    node *m_sub;
                    size_type m_cap, m_depth;
                    void _pushdown(size_type i) {
                        if (m_sub[i].m_change_cnt) {
                            if (Compare()(m_sub[i * 2].m_min[0], m_sub[i].m_min[0])) m_sub[i * 2].m_min[0] = m_sub[i].m_min[0], m_sub[i * 2].m_change_cnt += m_sub[i].m_change_cnt;
                            if (Compare()(m_sub[i * 2 + 1].m_min[0], m_sub[i].m_min[0])) m_sub[i * 2 + 1].m_min[0] = m_sub[i].m_min[0], m_sub[i * 2 + 1].m_change_cnt += m_sub[i].m_change_cnt;
                            m_sub[i].m_change_cnt = 0;
                        }
                    }
                    void _pushup(size_type i) {
                        m_sub[i].m_min[0] = std::min(m_sub[i * 2].m_min[0], m_sub[i * 2 + 1].m_min[0], Compare());
                        if (Compare()(m_sub[i * 2].m_min[0], m_sub[i * 2 + 1].m_min[0]))
                            m_sub[i].m_min[1] = std::min(m_sub[i * 2].m_min[1], m_sub[i * 2 + 1].m_min[0], Compare());
                        else if (Compare()(m_sub[i * 2 + 1].m_min[0], m_sub[i * 2].m_min[0]))
                            m_sub[i].m_min[1] = std::min(m_sub[i * 2].m_min[0], m_sub[i * 2 + 1].m_min[1], Compare());
                        else
                            m_sub[i].m_min[1] = std::min(m_sub[i * 2].m_min[1], m_sub[i * 2 + 1].m_min[1], Compare());
                    }
                    Zkw(size_type length, const Tp &min, const Tp &max) {
                        m_depth = std::max<size_type>(1, std::bit_width(length - 1)), m_cap = 1 << m_depth;
                        m_sub = new node[m_cap + length];
                        std::fill_n(m_sub + 1, m_cap + length - 1, node{{min, max}, {}});
                    }
                    ~Zkw() { delete[] m_sub; }
                    size_type query(size_type time) const {
                        size_type res = m_sub[time += m_cap].m_change_cnt;
                        for (Tp val = m_sub[time].m_min[0]; time >>= 1;)
                            if (Compare()(val, m_sub[time].m_min[0])) val = m_sub[time].m_min[0], res += m_sub[time].m_change_cnt;
                        return res;
                    }
                    void _chmax(size_type i, size_type floor, size_type ceil, size_type time_l, size_type time_r, const Tp &val) {
                        if (!Compare()(m_sub[i].m_min[0], val)) return;
                        if (time_l <= floor && time_r >= ceil && Compare()(val, m_sub[i].m_min[1])) {
                            m_sub[i].m_min[0] = val, m_sub[i].m_change_cnt++;
                            return;
                        }
                        _pushdown(i);
                        size_type mid = (floor + ceil) / 2;
                        if (time_l < mid) _chmax(i * 2, floor, mid, time_l, time_r, val);
                        if (mid < time_r) _chmax(i * 2 + 1, mid, ceil, time_l, time_r, val);
                        _pushup(i);
                    }
                    void chmax(size_type time_l, size_type time_r, const Tp &val) { _chmax(1, 0, m_cap, time_l, time_r, val); }
                };
                Zkw zkw(m_timestamp, min, max);
                std::vector<size_type> res(m_timestamp), op_deg(m_range + 1), q_deg(m_range + 1);
                for (auto &op : m_ops) op_deg[op.m_index + 1]++;
                for (size_type i = 1; i <= m_range; i++) op_deg[i] += op_deg[i - 1];
                struct modify_data {
                    size_type m_time;
                    Tp m_val;
                };
                std::vector<modify_data> ops(op_deg.back());
                for (auto &op : m_ops) ops[op_deg[op.m_index]++] = {op.m_time, op.m_val};
                for (auto &q : m_queries) q_deg[q.m_index + 1]++;
                for (size_type i = 1; i <= m_range; i++) q_deg[i] += q_deg[i - 1];
                std::vector<size_type> qs(q_deg.back());
                for (auto &q : m_queries) qs[q_deg[q.m_index]++] = q.m_time;
                modify_data *op_cur = ops.data();
                size_type *q_cur = qs.data();
                for (size_type i = 0; i != m_range; i++) {
                    modify_data *op_end = ops.data() + op_deg[i];
                    for (; op_cur != op_end; op_cur++) {
                        size_type time_l = op_cur->m_time, time_r = op_cur + 1 == op_end ? zkw.m_cap : op_cur[1].m_time;
                        if (time_l != time_r) zkw.chmax(time_l, time_r, op_cur->m_val);
                    }
                    size_type *q_end = qs.data() + q_deg[i];
                    for (; q_cur != q_end; q_cur++) res[*q_cur] = zkw.query(*q_cur);
                }
                return res;
            }
        };
    }
}

#endif