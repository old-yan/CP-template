#ifndef __OY_TARJAN_CUT__
#define __OY_TARJAN_CUT__

#include "Graph.h"

namespace OY {
    template <typename _UDG>
    struct Tarjan_cut {
        _UDG &m_graph;
        std::vector<uint32_t> m_dfn;
        uint32_t m_dfnCount;
        std::vector<uint32_t> m_low;
        std::vector<uint32_t> m_adj;
        uint32_t m_cutCount;
        std::vector<uint32_t> m_stack;
        std::vector<std::vector<uint32_t>> m_vbcc;
        Tarjan_cut(_UDG &__graph) : m_graph(__graph), m_dfn(__graph.m_vertexNum, -1), m_low(__graph.m_vertexNum, -1), m_dfnCount(0), m_adj(__graph.m_vertexNum, 0), m_cutCount(0) {
            m_vbcc.reserve(m_graph.m_vertexNum);
            m_stack.reserve(m_graph.m_vertexNum);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!~m_dfn[i]) dfs(i, -1);
            m_cutCount = std::count_if(m_adj.begin(), m_adj.end(), [](uint32_t x) { return x > 1; });
        }
        void dfs(uint32_t __i, uint32_t __fromIndex) {
            m_dfn[__i] = m_low[__i] = m_dfnCount++;
            uint32_t stackLength = m_stack.size();
            m_stack.push_back(__i);
            if (~__fromIndex) m_adj[__i]++;
            for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(__i))
                if (!~m_dfn[to]) {
                    dfs(to, index);
                    if (m_low[to] < m_dfn[__i])
                        chmin(m_low[__i], m_low[to]);
                    else
                        m_adj[__i]++;
                } else if (!m_graph.isReversed(index, __fromIndex))
                    chmin(m_low[__i], m_dfn[to]);
            if (!~__fromIndex) {
                if (!m_adj[__i]) m_vbcc.emplace_back<uint32_t *, uint32_t *>(m_stack.data() + stackLength, m_stack.data() + m_stack.size());
                m_stack.resize(stackLength);
            } else if (uint32_t parent = m_graph.getEdge(__fromIndex).from; m_low[__i] >= m_dfn[parent]) {
                std::swap(m_stack[stackLength - 1], parent);
                m_vbcc.emplace_back<uint32_t *, uint32_t *>(m_stack.data() + stackLength - 1, m_stack.data() + m_stack.size());
                std::swap(m_stack[stackLength - 1], parent);
                m_stack.resize(stackLength);
            }
        }
        bool isCut(uint32_t __i) const { return m_adj[__i] > 1; }
        std::vector<uint32_t> getCuts() const {
            std::vector<uint32_t> cuts;
            cuts.reserve(m_cutCount);
            for (uint32_t i = 0; i < m_adj.size(); i++)
                if (m_adj[i] > 1) cuts.push_back(i);
            return cuts;
        }
    };
}

#endif