/*
最后修改:
20231025
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TOPOLOGICALSORT__
#define __OY_TOPOLOGICALSORT__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace TOPO {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            static size_type s_buffer[MAX_VERTEX], s_queue_buffer[MAX_VERTEX], s_use_count;
            size_type m_vertex_cnt, *m_deg, *m_queue;
            Solver(size_type vertex_cnt) {
                m_vertex_cnt = vertex_cnt, m_deg = s_buffer + s_use_count, m_queue = s_queue_buffer + s_use_count, s_use_count += m_vertex_cnt;
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type head = 0, tail = 0;
                for (size_type from = 0; from != m_vertex_cnt; from++) traverser(from, [&](size_type to) { m_deg[to]++; });
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!m_deg[i]) m_queue[tail++] = i;
                while (head != tail) traverser(m_queue[head++], [&](size_type to) {if(!--m_deg[to])m_queue[tail++]=to; });
                return head == m_vertex_cnt;
            }
            template <typename Callback>
            void trace(Callback &&call) const {
                for (size_type i = 0; i != m_vertex_cnt; i++) call(m_queue[i]);
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<MAX_VERTEX, MAX_EDGE>::s_queue_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_to, m_next;
            };
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE];
            size_type *m_vertex, m_vertex_cnt, m_edge_cnt;
            edge *m_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next) call(m_edges[index].m_to);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_vertex = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
                std::fill_n(m_vertex, m_vertex_cnt, -1);
            }
            void add_edge(size_type a, size_type b) { m_edges[m_edge_cnt] = edge{b, m_vertex[a]}, m_vertex[a] = m_edge_cnt++; }
            std::pair<Solver<MAX_VERTEX, MAX_EDGE>, bool> calc() const {
                Solver<MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
            std::vector<size_type> get_path() const {
                std::vector<size_type> res;
                Solver<MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt);
                if (!sol.run(*this)) return res;
                res.reserve(m_vertex_cnt);
                sol.trace([&](size_type i) { res.push_back(i); });
                return res;
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
};

#endif