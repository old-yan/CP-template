/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FLATTREE__
#define __OY_FLATTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace FlatTree {
        using size_type = uint32_t;
        struct Ignore {
            template <typename... Args>
            void operator()(Args... args) const {}
        };
        template <typename Tp>
        struct Edge {
            size_type m_from, m_to;
            Tp m_dis;
        };
        template <>
        struct Edge<bool> {
            size_type m_from, m_to;
        };
        template <typename Tp>
        struct Adj {
            size_type m_to;
            Tp m_dis;
        };
        template <>
        struct Adj<bool> {
            size_type m_to;
        };
        template <typename Tp, size_type MAX_VERTEX>
        struct Tree {
            static Edge<Tp> s_edge_buffer[MAX_VERTEX << 1];
            static Adj<Tp> s_buffer[MAX_VERTEX << 1];
            static size_type s_start_buffer[MAX_VERTEX << 1], s_use_count;
            Edge<Tp> *m_edges;
            Adj<Tp> *m_adj;
            size_type *m_starts, m_root = -1, m_vertex_cnt, m_edge_cnt;
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0)), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_vertex(size_type a, size_type p, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p);
                else if (!pre_work(a, p))
                    return false;
                do_for_each_adj_vertex(a, [&](size_type to) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_vertex(to, a, pre_work, report, after_work)) report(a, to);
                    } else if (to != p)
                        _tree_dp_vertex(to, a, pre_work, report, after_work), report(a, to);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0, std::declval<Tp>())), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_edge(size_type a, size_type p, Tp up_dis, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p, up_dis);
                else if (!pre_work(a, p, up_dis))
                    return false;
                do_for_each_adj_edge(a, [&](size_type to, Tp dis) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_edge(to, a, dis, pre_work, report, after_work)) report(a, to, dis);
                    } else if (to != p)
                        _tree_dp_edge(to, a, dis, pre_work, report, after_work), report(a, to, dis);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            Tree(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                m_root = -1;
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_edge_buffer + s_use_count, m_adj = s_buffer + s_use_count, m_starts = s_start_buffer + s_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt << 1;
            }
            void add_edge(size_type a, size_type b, Tp dis = Tp()) {
                if constexpr (std::is_same<Tp, bool>::value)
                    m_edges[m_edge_cnt++] = {a, b};
                else
                    m_edges[m_edge_cnt++] = {a, b, dis};
            }
            void prepare() {
                std::fill_n(m_starts, m_vertex_cnt + 1, 0);
                for (size_type i = 0; i < m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    m_starts[from + 1]++, m_starts[to + 1]++;
                }
                std::partial_sum(m_starts, m_starts + m_vertex_cnt + 1, m_starts);
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i < m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if constexpr (std::is_same<Tp, bool>::value)
                        m_adj[cursor[from]++] = {to}, m_adj[cursor[to]++] = {from};
                    else {
                        Tp dis = m_edges[i].m_dis;
                        m_adj[cursor[from]++] = {to, dis}, m_adj[cursor[to]++] = {from, dis};
                    }
                }
            }
            void set_root(size_type root) { m_root = root; }
            size_type vertex_cnt() const { return m_vertex_cnt; }
            template <typename Callback>
            void do_for_each_adj_vertex(size_type a, Callback &&call) const {
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to);
            }
            template <typename Callback>
            void do_for_each_adj_edge(size_type a, Callback &&call) const {
                if constexpr (std::is_same<Tp, bool>::value)
                    for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to, 1);
                else
                    for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to, m_adj[cur].m_dis);
            }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_vertex(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_vertex(a, -1, pre_work, report, after_work); }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_edge(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_edge(a, -1, {}, pre_work, report, after_work); }
        };
        template <typename Tp, size_type MAX_VERTEX>
        Edge<Tp> Tree<Tp, MAX_VERTEX>::s_edge_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        Adj<Tp> Tree<Tp, MAX_VERTEX>::s_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        size_type Tree<Tp, MAX_VERTEX>::s_start_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        size_type Tree<Tp, MAX_VERTEX>::s_use_count;
        template <typename Ostream, typename Tp, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const Tree<Tp, MAX_VERTEX> &tree) { // http://mshang.ca/syntree/
            tree.tree_dp_vertex(
                ~tree.m_root ? tree.m_root : 0, [&](size_type a, size_type) { out << '[' << a; }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
}

#endif
