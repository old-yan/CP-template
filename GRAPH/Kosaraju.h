#ifndef __OY_KOSARAJU__
#define __OY_KOSARAJU__

#include "Graph.h"

namespace OY {
    template <typename _DG>
    struct Kosaraju {
        _DG &m_graph;
        _DG m_reverseGraph;
        std::vector<uint32_t> m_stack;
        std::vector<uint32_t> m_id;
        uint32_t m_idCount;
        std::vector<uint32_t> m_topo;
        Kosaraju(_DG &__graph) : m_graph(__graph), m_reverseGraph(__graph.m_vertexNum, __graph.m_edgeNum), m_id(__graph.m_vertexNum, 0), m_idCount(0) {
            for (auto [from, to, value] : m_graph.getEdgesData()) m_reverseGraph.addEdge(to, from, value);
            m_reverseGraph.prepare();
            m_stack.reserve(m_graph.m_vertexNum);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!m_id[i]) dfs(i);
            m_topo.reserve(m_graph.m_vertexNum);
            uint32_t cursor = 0;
            while (m_stack.size()) {
                if (!~m_id[m_stack.back()]) {
                    m_id[m_stack.back()] = m_idCount;
                    m_topo.push_back(m_stack.back());
                    do {
                        for (auto [to, value] : m_reverseGraph.getEdgesAdjOf(m_topo[cursor++]))
                            if (!~m_id[to]) {
                                m_id[to] = m_idCount;
                                m_topo.push_back(to);
                            }
                    } while (cursor < m_topo.size());
                    m_idCount++;
                }
                m_stack.pop_back();
            }
        }
        void dfs(uint32_t __i) {
            m_id[__i] = -1;
            for (auto [to, value] : m_graph.getEdgesAdjOf(__i))
                if (!m_id[to]) dfs(to);
            m_stack.push_back(__i);
        }
        std::vector<std::vector<uint32_t>> groups() const {
            uint32_t count[m_idCount];
            std::fill(count, count + m_idCount, 0);
            std::vector<std::vector<uint32_t>> res(m_idCount);
            for (uint32_t i = 0; i < m_idCount; i++) res[i].reserve(count[i]);
            for (uint32_t i = 0; i < m_id.size(); i++) res[m_id[i]].push_back(i);
            return res;
        }
    };
}

#endif