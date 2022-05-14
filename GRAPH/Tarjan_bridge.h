#ifndef __OY_TARJAN_BRIDGE__
#define __OY_TARJAN_BRIDGE__

#include "Graph.h"

namespace OY {
    template <typename _UDG>
    struct Tarjan_bridge {
        _UDG &m_graph;
        std::vector<uint32_t> m_dfn;
        uint32_t m_dfnCount;
        std::vector<uint32_t> m_low;
        std::vector<bool> m_isBridge;
        uint32_t m_bridgeCount;
        uint32_t m_ebccCount;
        Tarjan_bridge(_UDG &__graph) : m_graph(__graph), m_dfn(__graph.m_vertexNum, -1), m_low(__graph.m_vertexNum, -1), m_dfnCount(0), m_isBridge(__graph.m_edgeNum), m_bridgeCount(0), m_ebccCount(0) {
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!~m_dfn[i]) {
                    dfs(i, -1);
                    m_ebccCount++;
                }
            m_ebccCount += m_bridgeCount;
        }
        void dfs(uint32_t __i, uint32_t __fromIndex) {
            m_dfn[__i] = m_low[__i] = m_dfnCount++;
            uint32_t reverse;
            for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(__i))
                if (!~m_dfn[to]) {
                    dfs(to, index);
                    if (m_low[to] < m_dfn[__i])
                        chmin(m_low[__i], m_low[to]);
                } else {
                    if (m_graph.isReversed(index, __fromIndex))
                        reverse = index;
                    else
                        chmin(m_low[__i], m_dfn[to]);
                }
            if (m_low[__i] == m_dfn[__i] && ~__fromIndex) {
                m_isBridge[__fromIndex] = m_isBridge[reverse] = true;
                m_bridgeCount++;
            }
        }
        bool isBridge(uint32_t __from, uint32_t __to) const { return m_low[__from] > m_dfn[__to] || m_low[__to] > m_dfn[__from]; }
        std::vector<uint32_t> getBridges() const {
            std::vector<uint32_t> bridges;
            bridges.reserve(m_bridgeCount * 2);
            for (uint32_t i = 0; i < m_isBridge.size(); i++)
                if (m_isBridge[i]) bridges.push_back(i);
            return bridges;
        }
        std::vector<std::vector<uint32_t>> getEBCC() const {
            std::vector<std::vector<uint32_t>> res;
            res.reserve(m_ebccCount);
            uint32_t queue[m_graph.m_vertexNum];
            std::vector<bool> visit(m_graph.m_vertexNum, false);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!visit[i]) {
                    uint32_t head = 0, tail = 0;
                    visit[i] = true;
                    queue[tail++] = i;
                    while (head < tail)
                        for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(queue[head++]))
                            if (!m_isBridge[index] && !visit[to]) {
                                visit[to] = true;
                                queue[tail++] = to;
                            }
                    res.emplace_back<uint32_t *, uint32_t *>(queue, queue + tail);
                }
            return res;
        }
    };
}

#endif