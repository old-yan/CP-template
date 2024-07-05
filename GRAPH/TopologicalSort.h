/*
最后修改:
20240705
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
        struct Solver {
            size_type m_vertex_cnt;
            std::vector<size_type> m_deg, m_queue;
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_deg(vertex_cnt), m_queue(vertex_cnt) {}
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
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            std::vector<raw_edge> m_raw_edges;
            mutable std::vector<size_type> m_starts, m_edges;
            void _prepare() const {
                m_prepared = true;
                m_starts.assign(m_vertex_cnt + 1, 0);
                for (auto &e : m_raw_edges) m_starts[e.m_from + 1]++;
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                auto cursor = m_starts;
                m_edges.resize(m_starts.back());
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_from]++] = e.m_to;
            }
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) call(m_edges[cur]);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to) { m_raw_edges.push_back({from, to}); }
            std::pair<Solver, bool> calc() const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver(m_vertex_cnt), false);
                res.second = res.first.run(*this);
                return res;
            }
            std::vector<size_type> get_path() const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver sol(m_vertex_cnt);
                if (!sol.run(*this)) return res;
                res.reserve(m_vertex_cnt);
                sol.trace([&](size_type i) { res.push_back(i); });
                return res;
            }
        };
    }
};

#endif