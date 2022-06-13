#ifndef __OY_TARJAN_BRIDGE__
#define __OY_TARJAN_BRIDGE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include "Graph.h"

namespace OY {
    struct Tarjan_bridge {
        struct _RawEdge {
            uint32_t from, to;
        };
        struct _Edge {
            uint32_t index, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_dfn;
        std::vector<uint32_t> m_low;
        std::vector<bool> m_isBridge;
        uint32_t m_vertexNum;
        uint32_t m_dfnCount;
        uint32_t m_bridgeCount;
        uint32_t m_ebccCount;
        Tarjan_bridge(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_dfn(__vertexNum, -1), m_low(__vertexNum, -1), m_isBridge(__edgeNum, false), m_dfnCount(0), m_vertexNum(__vertexNum), m_bridgeCount(0), m_ebccCount(0) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void build() {
            for (auto &[from, to] : m_rawEdges) {
                m_starts[from + 1]++;
                if (from != to) m_starts[to + 1]++;
            }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to] = m_rawEdges[index];
                m_edges[cursor[from]++] = _Edge{index, to};
                if (from != to) m_edges[cursor[to]++] = _Edge{index, from};
            }
        }
        void calc() {
            build();
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!~m_dfn[i]) {
                    dfs(i, -1);
                    m_ebccCount++;
                }
            m_ebccCount += m_bridgeCount;
        }
        void dfs(uint32_t __i, uint32_t __fromIndex) {
            m_dfn[__i] = m_low[__i] = m_dfnCount++;
            for (uint32_t cur = m_starts[__i], end = m_starts[__i + 1]; cur < end; cur++)
                if (auto &[index, to] = m_edges[cur]; !~m_dfn[to]) {
                    dfs(to, index);
                    chmin(m_low[__i], m_low[to]);
                } else if (index != __fromIndex)
                    chmin(m_low[__i], m_dfn[to]);
            if (m_low[__i] == m_dfn[__i] && ~__fromIndex) {
                m_isBridge[__fromIndex] = true;
                m_bridgeCount++;
            }
        }
        bool isBridge(uint32_t __from, uint32_t __to) const { return m_low[__from] > m_dfn[__to] || m_low[__to] > m_dfn[__from]; }
        std::vector<uint32_t> getBridges() const {
            std::vector<uint32_t> bridges;
            bridges.reserve(m_bridgeCount);
            for (uint32_t i = 0; i < m_isBridge.size(); i++)
                if (m_isBridge[i]) bridges.push_back(i);
            return bridges;
        }
        std::vector<std::vector<uint32_t>> getEBCC() const {
            std::vector<std::vector<uint32_t>> res;
            res.reserve(m_ebccCount);
            uint32_t queue[m_vertexNum];
            std::vector<bool> visit(m_vertexNum, false);
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!visit[i]) {
                    uint32_t head = 0, tail = 0;
                    visit[i] = true;
                    queue[tail++] = i;
                    while (head < tail)
                        for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                            if (auto &[index, to] = m_edges[cur]; !m_isBridge[index] && !visit[to]) {
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