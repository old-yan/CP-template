/*
最后修改:
20231104
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GUSFIELD__
#define __OY_GUSFIELD__

#include "Dinic.h"

namespace OY {
    namespace Gusfield {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        struct Graph {
            static size_type s_buffer[MAX_VERTEX * 3], s_use_count, s_node_use_count;
            static Tp s_table_buffer[MAX_NODE];
            DINIC::Graph<Tp, false, MAX_VERTEX, MAX_EDGE> m_dinic;
            size_type *m_vertexes, *m_queue, *m_dep;
            Tp *m_table;
            void _dfs(size_type first, size_type last, const Tp &infinite) {
                if (first + 1 == last) return;
                size_type source = m_vertexes[first], target = m_vertexes[first + 1], head = 0, tail = 0;
                Tp flow = m_dinic.calc(source, target, infinite);
                std::fill_n(m_dep, m_dinic.m_vertex_cnt, -1);
                m_dep[source] = 0, m_queue[tail++] = source;
                while (head != tail)
                    for (size_type from = m_queue[head++], cur = m_dinic._start_of(from), end = m_dinic._start_of(from + 1); cur != end; cur++)
                        if (m_dinic.m_adj[cur].m_cap) {
                            size_type to = m_dinic.m_adj[cur].m_to;
                            if (m_dep[from] + 1 < m_dep[to]) m_dep[to] = m_dep[from] + 1, m_queue[tail++] = to;
                        }
                size_type split = std::partition(m_vertexes + first + 2, m_vertexes + last, [&](size_type x) { return ~m_dep[x]; }) - m_vertexes - 1;
                std::swap(m_vertexes[first + 1], m_vertexes[split]);
                m_dinic.clear(), _dfs(first, split, infinite), _dfs(split, last, infinite);
                for (size_type i = first; i != split; i++)
                    for (size_type j = split; j != last; j++) {
                        size_type a = m_vertexes[i], b = m_vertexes[j];
                        m_table[m_dinic.m_vertex_cnt * a + b] = m_table[m_dinic.m_vertex_cnt * b + a] = std::min({m_table[m_dinic.m_vertex_cnt * source + a], flow, m_table[m_dinic.m_vertex_cnt * target + b]});
                    }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!vertex_cnt) return;
                m_dinic.resize(vertex_cnt, edge_cnt << 1);
                m_vertexes = s_buffer + s_use_count * 3, m_queue = s_buffer + s_use_count * 3 + vertex_cnt, m_dep = s_buffer + s_use_count * 3 + vertex_cnt * 2, m_table = s_table_buffer + s_node_use_count, s_use_count += vertex_cnt, s_node_use_count += vertex_cnt * vertex_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_dinic.add_edge(a, b, cost); }
            void calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                for (size_type i = 0; i != m_dinic.m_vertex_cnt; i++) m_vertexes[i] = i;
                std::fill_n(m_table, m_dinic.m_vertex_cnt * m_dinic.m_vertex_cnt, infinite);
                _dfs(0, m_dinic.m_vertex_cnt, infinite);
            }
            Tp query(size_type a, size_type b) const { return m_table[m_dinic.m_vertex_cnt * a + b]; }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_buffer[MAX_VERTEX * 3];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_node_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        Tp Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_table_buffer[MAX_NODE];
    }
}

#endif