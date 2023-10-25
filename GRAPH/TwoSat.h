/*
最后修改:
20231025
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TWOSAT__
#define __OY_TWOSAT__

#include "Tarjan_scc.h"

namespace OY {
    namespace TWOSAT {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX, size_type MAX_CLAUSE>
        struct Graph {
            static bool s_buffer[MAX_VERTEX];
            static size_type s_use_count;
            SCC::Graph<MAX_VERTEX << 1, MAX_CLAUSE << 1> m_scc;
            bool *m_state;
            Graph(size_type vertex_cnt = 0, size_type clause_cnt = 0) { resize(vertex_cnt, clause_cnt); }
            void resize(size_type vertex_cnt, size_type clause_cnt) {
                m_scc.resize(vertex_cnt << 1, clause_cnt << 1);
                m_state = s_buffer + s_use_count, s_use_count += vertex_cnt;
            }
            void add_clause(size_type i, bool i_val, size_type j, bool j_val) { m_scc.add_edge(i * 2 + !i_val, j * 2 + j_val), m_scc.add_edge(j * 2 + !j_val, i * 2 + i_val); }
            bool calc() const {
                auto sol = m_scc.calc();
                for (size_type i = 0; i != m_scc.m_vertex_cnt; i += 2) {
                    if (sol.m_info[i].m_id == sol.m_info[i ^ 1].m_id) return false;
                    m_state[i >> 1] = sol.m_info[i].m_id < sol.m_info[i ^ 1].m_id;
                }
                return true;
            }
            bool query(size_type i) const { return m_state[i]; }
        };
        template <size_type MAX_VERTEX, size_type MAX_CLAUSE>
        bool Graph<MAX_VERTEX, MAX_CLAUSE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_CLAUSE>
        size_type Graph<MAX_VERTEX, MAX_CLAUSE>::s_use_count;
    }
};

#endif