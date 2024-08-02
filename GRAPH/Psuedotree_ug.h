/*
最后修改:
20240726
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PSUEDOTREE_UG__
#define __OY_PSUEDOTREE_UG__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace PsuedoUG {
        using size_type = uint32_t;
        template <typename Tp>
        struct RawEdge {
            size_type m_from, m_to;
            Tp m_dis;
        };
        template <>
        struct RawEdge<bool> {
            size_type m_from, m_to;
        };
        template <typename Tp>
        struct Edge {
            size_type m_index, m_to;
            Tp m_dis;
        };
        template <>
        struct Edge<bool> {
            size_type m_index, m_to;
        };
        template <typename Tp, typename SumType>
        struct Info {
            size_type m_cycle_id, m_step_to_cycle, m_cycle_join;
            SumType m_dis_to_cycle;
            size_type cycle_id() const { return m_cycle_id; }
            size_type step_to_cycle() const { return m_step_to_cycle; }
            SumType dis_to_cycle() const { return m_dis_to_cycle; }
            size_type cycle_join() const { return m_cycle_join; }
        };
        template <>
        struct Info<bool, size_type> {
            size_type m_cycle_id, m_step_to_cycle, m_cycle_join;
            size_type cycle_id() const { return m_cycle_id; }
            size_type step_to_cycle() const { return m_step_to_cycle; }
            size_type dis_to_cycle() const { return m_step_to_cycle; }
            size_type cycle_join() const { return m_cycle_join; }
        };
        template <typename Tp, typename SumType>
        struct Cycle {
            size_type m_cycle_size, m_component_size;
            SumType m_length;
            size_type *m_seq;
            SumType *m_pos;
            size_type size() const { return m_cycle_size; }
            size_type component_size() const { return m_component_size; }
            SumType length() const { return m_length; }
            size_type operator[](size_type i) const { return m_seq[i]; }
            SumType position(size_type i) const { return m_pos[i]; }
            size_type *begin() const { return m_seq; }
            size_type *end() const { return m_seq + m_cycle_size; }
        };
        template <>
        struct Cycle<bool, size_type> {
            size_type m_cycle_size, m_component_size;
            size_type *m_seq;
            size_type size() const { return m_cycle_size; }
            size_type component_size() const { return m_component_size; }
            size_type length() const { return m_cycle_size; }
            size_type operator[](size_type i) const { return m_seq[i]; }
            size_type position(size_type i) const { return i; }
            size_type *begin() const { return m_seq; }
            size_type *end() const { return m_seq + m_cycle_size; }
        };
        template <bool Maintain>
        struct AncestorMaintainer {};
        template <>
        struct AncestorMaintainer<true> {
            std::vector<size_type> m_seq, m_id, m_buf, m_range_start;
            std::vector<std::pair<size_type, size_type>> m_ranges;
            void resize(size_type n) {
                m_seq.clear(), m_seq.reserve(n);
                m_ranges.clear(), m_ranges.reserve(n);
                m_id.resize(n), m_buf.resize(n), m_range_start.resize(n);
            }
        };
        template <typename Tp, typename SumType = typename std::conditional<std::is_same<Tp, bool>::value, size_type, Tp>::type, bool MaintainAncestor = false>
        struct Graph {
            static constexpr bool is_bool = std::is_same<Tp, bool>::value;
            using raw_edge = RawEdge<Tp>;
            using edge = Edge<Tp>;
            using vertex_info = Info<Tp, SumType>;
            using cycle_info = Cycle<Tp, SumType>;
            size_type m_vertex_cnt;
            std::vector<raw_edge> m_raw_edges;
            std::vector<edge> m_edges;
            std::vector<size_type> m_starts, m_cycle_buf;
            std::vector<edge> m_in_edge;
            std::vector<vertex_info> m_info;
            std::vector<cycle_info> m_cycle_info;
            std::vector<SumType> m_pos_buf;
            AncestorMaintainer<MaintainAncestor> m_am;
            void _prepare_ancestor(std::vector<size_type> &deg) {
                deg.assign(m_vertex_cnt + 1, {});
                std::vector<size_type> rev(m_vertex_cnt + 1);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_step_to_cycle) deg[m_in_edge[i].m_to + 1]++;
                for (size_type i = 0; i != m_vertex_cnt; i++) deg[i + 1] += deg[i];
                auto cursor = deg;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_step_to_cycle) rev[cursor[m_in_edge[i].m_to]++] = i;
                std::vector<size_type> level_cnt(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) level_cnt[m_info[i].m_step_to_cycle]++;
                while (!level_cnt.back()) level_cnt.pop_back();
                level_cnt[0] = 0;
                for (size_type i = 1; i != level_cnt.size(); i++) level_cnt[i] += level_cnt[i - 1];
                cursor.assign(level_cnt.size(), {});
                m_am.resize(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!m_info[i].m_step_to_cycle) {
                        auto call = [&, i](size_type a) {
                            if (a != i) {
                                size_type d = m_info[a].m_step_to_cycle;
                                cursor[d - 1]++, m_am.m_buf[level_cnt[d - 1]++] = m_am.m_id[a] = m_am.m_seq.size(), m_am.m_seq.push_back(a);
                            }
                        };
                        _dfs(i, deg.data(), rev.data(), call);
                        m_am.m_range_start[i] = m_am.m_ranges.size();
                        for (size_type d = 0; cursor[d]; cursor[d++] = 0) m_am.m_ranges.emplace_back(level_cnt[d] - cursor[d], level_cnt[d]);
                    }
            }
            template <typename Callback>
            void _dfs(size_type a, size_type *deg, size_type *rev, Callback &&call) {
                for (size_type it = deg[a], end = deg[a + 1]; it != end; it++) _dfs(rev[it], deg, rev, call);
                call(a);
            }
            void _dfs(size_type a) {
                size_type to = m_in_edge[a].m_to;
                if (!~m_info[to].m_cycle_id) _dfs(to);
                m_info[a].m_cycle_id = m_info[to].m_cycle_id;
                m_info[a].m_step_to_cycle = m_info[to].m_step_to_cycle + 1;
                m_info[a].m_cycle_join = m_info[to].m_cycle_join;
                if constexpr (!is_bool) m_info[a].m_dis_to_cycle = m_info[to].m_dis_to_cycle + m_in_edge[a].m_dis;
            }
            Graph(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_raw_edges.clear(), m_raw_edges.reserve(m_vertex_cnt);
            }
            void add_edge(size_type a, size_type b, Tp dis = 1) {
                if constexpr (is_bool)
                    m_raw_edges.push_back({a, b});
                else
                    m_raw_edges.push_back({a, b, dis});
            }
            void prepare() {
                std::vector<bool> self_loop(m_vertex_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
                for (auto &e : m_raw_edges) {
                    m_starts[e.m_from + 1]++;
                    if (e.m_from != e.m_to)
                        m_starts[e.m_to + 1]++;
                    else
                        self_loop[e.m_from] = true;
                }
                for (size_type i = 0; i != m_vertex_cnt; i++) m_starts[i + 1] += m_starts[i];
                auto _buf = m_starts;
                size_type *buf = _buf.data();
                m_edges.resize(m_starts.back());
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    auto &e = m_raw_edges[i];
                    if constexpr (is_bool) {
                        m_edges[buf[e.m_from]++] = {i, e.m_to};
                        if (e.m_from != e.m_to) m_edges[buf[e.m_to]++] = {i, e.m_from};
                    } else {
                        m_edges[buf[e.m_from]++] = {i, e.m_to, e.m_dis};
                        if (e.m_from != e.m_to) m_edges[buf[e.m_to]++] = {i, e.m_from, e.m_dis};
                    }
                }
                for (size_type i = m_vertex_cnt - 1; i; i--) buf[i] = buf[i] - buf[i - 1];
                std::vector<size_type> que_buf(m_vertex_cnt + 1);
                std::vector<bool> vis(m_vertex_cnt);
                size_type *queue = que_buf.data(), head = 0, tail = 0;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (buf[i] == 1 && !self_loop[i]) queue[tail++] = i, vis[i] = true;
                m_in_edge.resize(m_vertex_cnt);
                while (head != tail)
                    for (size_type x = queue[head++], cur = m_starts[x], end = m_starts[x + 1]; cur != end; cur++) {
                        size_type to = m_edges[cur].m_to;
                        if (--buf[to] == 1 && !self_loop[to]) queue[tail++] = to, vis[to] = true;
                        if (buf[to]) m_in_edge[x] = m_edges[cur];
                    }
                m_info.resize(m_vertex_cnt);
                m_cycle_buf.clear(), m_cycle_buf.reserve(m_vertex_cnt);
                if constexpr (!is_bool) m_pos_buf.clear(), m_pos_buf.reserve(m_vertex_cnt);
                m_cycle_info.clear(), m_cycle_info.reserve(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_cycle_id = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!vis[i]) {
                        size_type cur = 0;
                        auto push = [&](size_type x) { queue[cur] = x, vis[x] = true; };
                        push(i);
                        if (self_loop[i]) {
                            size_type e_cur = m_starts[i];
                            while (m_edges[e_cur].m_to != i) e_cur++;
                            m_in_edge[i] = m_edges[e_cur], cur++;
                        } else
                            while (true) {
                                size_type x = queue[cur++], e_cur = m_starts[x], e_end = m_starts[x + 1];
                                while (e_cur != e_end && vis[m_edges[e_cur].m_to]) e_cur++;
                                if (e_cur == e_end) {
                                    e_cur = m_starts[x];
                                    while (m_edges[e_cur].m_to != i) e_cur++;
                                    if (cur == 2 && m_edges[e_cur].m_index == m_in_edge[queue[0]].m_index)
                                        while (m_edges[++e_cur].m_to != i) {};
                                    m_in_edge[x] = m_edges[e_cur];
                                    break;
                                }
                                m_in_edge[x] = m_edges[e_cur];
                                push(m_edges[e_cur].m_to);
                            }
                        m_cycle_info.push_back({});
                        m_cycle_info.back().m_cycle_size = cur;
                        m_cycle_info.back().m_component_size = cur;
                        m_cycle_info.back().m_seq = m_cycle_buf.data() + m_cycle_buf.size();
                        if constexpr (!is_bool) m_cycle_info.back().m_pos = m_pos_buf.data() + m_pos_buf.size();
                        for (size_type i = 0; i != cur; i++) {
                            size_type x = queue[i];
                            if constexpr (!is_bool) m_pos_buf.push_back(i ? m_pos_buf.back() + m_in_edge[m_cycle_buf.back()].m_dis : 0);
                            m_cycle_buf.push_back(x);
                            m_info[x].m_cycle_id = m_cycle_info.size() - 1;
                            m_info[x].m_step_to_cycle = 0;
                            m_info[x].m_cycle_join = i;
                            if constexpr (!is_bool) m_info[x].m_dis_to_cycle = {}, m_cycle_info.back().m_length += m_in_edge[x].m_dis;
                        }
                    }
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_cycle_id) _dfs(i);
                if constexpr (MaintainAncestor) _prepare_ancestor(que_buf);
            }
            size_type next(size_type v) const { return m_in_edge[v].m_to; }
            size_type next(size_type v, uint64_t step) const {
                static_assert(MaintainAncestor, "MaintainAncestor Must Be True");
                if (!step) return v;
                size_type d = m_info[v].m_step_to_cycle;
                if (step < d) {
                    size_type rt = m_cycle_info[m_info[v].m_cycle_id][m_info[v].m_cycle_join], l = m_am.m_ranges[m_am.m_range_start[rt] + d - step - 1].first, r = m_am.m_ranges[m_am.m_range_start[rt] + d - step - 1].second;
                    return m_am.m_seq[*std::lower_bound(m_am.m_buf.data() + l, m_am.m_buf.data() + r, m_am.m_id[v])];
                } else {
                    auto &cycle = m_cycle_info[m_info[v].m_cycle_id];
                    return cycle[(m_info[v].m_cycle_join + step - d) % cycle.m_cycle_size];
                }
            }
            size_type join(size_type a_off_cycle, size_type b_off_cycle) const {
                auto &info_a = m_info[a_off_cycle], &info_b = m_info[b_off_cycle];
                size_type step1 = info_a.step_to_cycle(), step2 = info_b.step_to_cycle();
                size_type base1 = step1 > step2 ? step1 - step2 : 0, base2 = step1 < step2 ? step2 - step1 : 0;
                size_type low = 0, high = std::min(step1, step2);
                while (low != high) {
                    uint32_t mid = (low + high) >> 1;
                    (next(a_off_cycle, base1 + mid) == next(b_off_cycle, base2 + mid)) ? (high = mid) : (low = mid + 1);
                }
                return next(a_off_cycle, base1 + low);
            }
            size_type cycle_count() const { return m_cycle_info.size(); }
            const vertex_info &get_vertex_info(size_type v) const { return m_info[v]; }
            const cycle_info &get_cycle_info(size_type cycle_id) const { return m_cycle_info[cycle_id]; }
            template <typename Callback>
            void do_for_edges(size_type a, Callback &&call) const {
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) {
                    size_type to = m_edges[cur].m_to;
                    if constexpr (is_bool)
                        call(to);
                    else
                        call(to, m_edges[cur].m_dis);
                }
            }
            template <typename Callback>
            void do_for_out_edges(size_type a, Callback &&call) const {
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) {
                    size_type to = m_edges[cur].m_to;
                    if (!m_info[to].step_to_cycle() || to == m_in_edge[a].m_to) continue;
                    if constexpr (is_bool)
                        call(to);
                    else
                        call(to, m_edges[cur].m_dis);
                }
            }
        };
    }
}

#endif