/*
最后修改:
20240721
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PSUEDOTREE_IG__
#define __OY_PSUEDOTREE_IG__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace PsuedoIG {
        using size_type = uint32_t;
        template <typename Tp>
        struct Edge {
            size_type m_to;
            Tp m_dis;
        };
        template <>
        struct Edge<bool> {
            size_type m_to;
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
            std::vector<size_type> m_seq, m_id, m_buf, m_range_start, m_rev_starts, m_rev_edges;
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
            using edge = Edge<Tp>;
            using vertex_info = Info<Tp, SumType>;
            using cycle_info = Cycle<Tp, SumType>;
            size_type m_vertex_cnt;
            std::vector<edge> m_edge;
            std::vector<vertex_info> m_info;
            std::vector<cycle_info> m_cycle_info;
            std::vector<size_type> m_cycle_buf;
            std::vector<SumType> m_pos_buf;
            AncestorMaintainer<MaintainAncestor> m_am;
            void _prepare_ancestor(std::vector<size_type> &buf1, std::vector<size_type> &buf2) {
                m_am.m_rev_starts.assign(m_vertex_cnt + 1, {});
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_step_to_cycle) m_am.m_rev_starts[m_edge[i].m_to + 1]++;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_am.m_rev_starts[i + 1] += m_am.m_rev_starts[i];
                m_am.m_rev_edges.resize(m_am.m_rev_starts.back());
                buf1 = m_am.m_rev_starts;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_step_to_cycle) m_am.m_rev_edges[buf1[m_edge[i].m_to]++] = i;
                buf1.assign(m_vertex_cnt, {});
                for (size_type i = 0; i != m_vertex_cnt; i++) buf1[m_info[i].m_step_to_cycle]++;
                while (!buf1.back()) buf1.pop_back();
                buf1[0] = 0;
                for (size_type i = 1; i != buf1.size(); i++) buf1[i] += buf1[i - 1];
                buf2.assign(buf1.size(), {});
                m_am.resize(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!m_info[i].m_step_to_cycle) {
                        auto call = [&, i](size_type a) {
                            if (a != i) {
                                size_type d = m_info[a].m_step_to_cycle;
                                buf2[d - 1]++, m_am.m_buf[buf1[d - 1]++] = m_am.m_id[a] = m_am.m_seq.size(), m_am.m_seq.push_back(a);
                            }
                        };
                        _dfs(i, call);
                        m_am.m_range_start[i] = m_am.m_ranges.size();
                        for (size_type d = 0; buf2[d]; buf2[d++] = 0) m_am.m_ranges.emplace_back(buf1[d] - buf2[d], buf1[d]);
                    }
            }
            template <typename Callback>
            void _dfs(size_type a, Callback &&call) {
                for (size_type it = m_am.m_rev_starts[a], end = m_am.m_rev_starts[a + 1]; it != end; it++) _dfs(m_am.m_rev_edges[it], call);
                call(a);
            }
            Graph(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edge.resize(m_vertex_cnt);
            }
            void set_edge(size_type from, size_type to, Tp dis = 1) {
                if constexpr (is_bool)
                    m_edge[from] = {to};
                else
                    m_edge[from] = {to, dis};
            }
            void prepare() {
                m_info.resize(m_vertex_cnt);
                m_cycle_buf.clear(), m_cycle_buf.reserve(m_vertex_cnt);
                if constexpr (!is_bool) m_pos_buf.clear(), m_pos_buf.reserve(m_vertex_cnt);
                m_cycle_info.clear(), m_cycle_info.reserve(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_cycle_id = -1;
                std::vector<size_type> queue_buf(m_vertex_cnt + 1), pos_buf(m_vertex_cnt + 1, -1);
                size_type *queue = queue_buf.data(), *pos = pos_buf.data();
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_cycle_id) {
                        size_type cur = 0;
                        auto push = [&](size_type x) { queue[cur] = x, pos[x] = cur; };
                        push(i);
                        while (true) {
                            size_type to = m_edge[queue[cur++]].m_to;
                            if (~pos[to]) {
                                if (!~m_info[to].m_cycle_id) {
                                    m_cycle_info.push_back({});
                                    m_cycle_info.back().m_cycle_size = cur - pos[to];
                                    m_cycle_info.back().m_component_size = cur - pos[to];
                                    m_cycle_info.back().m_seq = m_cycle_buf.data() + m_cycle_buf.size();
                                    if constexpr (!is_bool) m_cycle_info.back().m_pos = m_pos_buf.data() + m_pos_buf.size();
                                    size_type mi = std::min_element(queue + pos[to], queue + cur) - queue;
                                    for (size_type i = 0, j = mi; i != cur - pos[to]; i++, j = j + 1 == cur ? pos[to] : j + 1) {
                                        size_type x = queue[j];
                                        if constexpr (!is_bool) m_pos_buf.push_back(j == mi ? 0 : m_pos_buf.back() + m_edge[m_cycle_buf.back()].m_dis);
                                        m_cycle_buf.push_back(x);
                                        m_info[x].m_cycle_id = m_cycle_info.size() - 1;
                                        m_info[x].m_step_to_cycle = 0;
                                        m_info[x].m_cycle_join = i;
                                        if constexpr (!is_bool) m_info[x].m_dis_to_cycle = {}, m_cycle_info.back().m_length += m_edge[x].m_dis;
                                    }
                                    cur = pos[to];
                                }
                                m_cycle_info[m_info[to].m_cycle_id].m_component_size += cur;
                                for (size_type nxt = to; ~--cur;) {
                                    size_type x = queue[cur];
                                    m_info[x].m_cycle_id = m_info[nxt].m_cycle_id;
                                    m_info[x].m_step_to_cycle = m_info[nxt].m_step_to_cycle + 1;
                                    m_info[x].m_cycle_join = m_info[nxt].m_cycle_join;
                                    if constexpr (!is_bool) m_info[x].m_dis_to_cycle = m_info[nxt].m_dis_to_cycle + m_edge[x].m_dis;
                                    nxt = x;
                                }
                                break;
                            }
                            push(to);
                        }
                    }
                if constexpr (MaintainAncestor) _prepare_ancestor(queue_buf, pos_buf);
            }
            const edge &in_edge(size_type v) const { return m_edge[v]; }
            template <typename Callback>
            void do_for_out_edges(size_type v, Callback &&call) const {
                static_assert(MaintainAncestor, "MaintainAncestor Must Be True");
                for (size_type it = m_am.m_rev_starts[v], end = m_am.m_rev_starts[v + 1]; it != end; it++)
                    if constexpr (is_bool)
                        call(m_am.m_rev_edges[it]);
                    else
                        call(m_am.m_rev_edges[it], m_edge[m_am.m_rev_edges[it]].m_dis);
            }
            template <typename Callback>
            void do_for_edges(size_type v) const {
                static_assert(MaintainAncestor, "MaintainAncestor Must Be True");
                for (size_type it = m_am.m_rev_starts[v], end = m_am.m_rev_starts[v + 1]; it != end; it++) call(m_am.m_rev_edges[it]);
            }
            size_type next(size_type v) const { return m_edge[v].m_to; }
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
                static_assert(MaintainAncestor, "MaintainAncestor Must Be True");
                if (a_off_cycle == b_off_cycle) return a_off_cycle;
                auto &info_a = m_info[a_off_cycle], &info_b = m_info[b_off_cycle];
                auto &cycle = m_cycle_info[info_a.cycle_id()];
                size_type step = std::min(info_a.step_to_cycle(), info_b.step_to_cycle()), low = 0, high = step, id_a = m_am.m_id[a_off_cycle], id_b = m_am.m_id[b_off_cycle];
                if (id_a > id_b) std::swap(id_a, id_b);
                const auto buf = m_am.m_buf.data();
                const auto ranges = m_am.m_ranges.data() + m_am.m_range_start[cycle[info_a.cycle_join()]];
                while (low != high) {
                    size_type mid = (low + high + 1) / 2, l = ranges[mid - 1].first, r = ranges[mid - 1].second;
                    (*std::lower_bound(buf + l, buf + r, id_a) >= id_b) ? (low = mid) : (high = mid - 1);
                }
                if (!low) return cycle[info_a.cycle_join()];
                size_type l = ranges[low - 1].first, r = ranges[low - 1].second;
                return m_am.m_seq[*std::lower_bound(buf + l, buf + r, id_a)];
            }
            size_type cycle_count() const { return m_cycle_info.size(); }
            const vertex_info &get_vertex_info(size_type v) const { return m_info[v]; }
            const cycle_info &get_cycle_info(size_type cycle_id) const { return m_cycle_info[cycle_id]; }
        };
    }
}

#endif