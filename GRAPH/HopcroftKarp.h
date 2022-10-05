#ifndef __OY_HOPCROFTKARP__
#define __OY_HOPCROFTKARP__

#include <cstdint>
#include "Graph.h"

namespace OY {
    struct HopcroftKarp {
        struct _Edge {
            uint32_t a, b;
        };
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_adj, m_starts, m_leftMatch, m_rightMatch;
        uint32_t m_leftNum, m_rightNum;
        HopcroftKarp(uint32_t __leftNum, uint32_t __rightNum, uint32_t __edgeNum) : m_starts(__leftNum + 1), m_leftNum(__leftNum), m_rightNum(__rightNum) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_edges.push_back({__a, __b}); }
        void prepare() {
            for (auto &[a, b] : m_edges) m_starts[a + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_adj.resize(m_starts.back());
            uint32_t cursor[m_leftNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_leftNum, cursor);
            for (uint32_t index = 0; index < m_edges.size(); index++) {
                auto &[a, b] = m_edges[index];
                m_adj[cursor[a]++] = b;
            }
        }
        uint32_t calc() {
            std::vector<uint32_t> dist(m_leftNum), queue(m_leftNum), visit(m_leftNum, -1);
            uint32_t dfn = 0, res = 0;
            auto bfs = [&] {
                std::fill_n(dist.data(), m_leftNum, -1);
                uint32_t head = 0, tail = 0;
                for (uint32_t a = 0; a < m_leftNum; a++)
                    if (!~m_leftMatch[a]) dist[a] = 0, queue[tail++] = a;
                while (head < tail)
                    for (uint32_t a = queue[head++], cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                        if (uint32_t a2 = m_rightMatch[m_adj[cur]]; ~a2 && chmin(dist[a2], dist[a] + 1)) queue[tail++] = a2;
            };
            auto dfs = [&](auto &&self, uint32_t a) {
                visit[a] = dfn;
                for (uint32_t cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                    if (uint32_t b = m_adj[cur]; !~m_rightMatch[b]) {
                        m_rightMatch[b] = a;
                        m_leftMatch[a] = b;
                        return true;
                    }
                for (uint32_t cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                    if (uint32_t b = m_adj[cur]; visit[m_rightMatch[b]] != dfn && dist[m_rightMatch[b]] == dist[a] + 1 && self(self, m_rightMatch[b])) {
                        m_rightMatch[b] = a;
                        m_leftMatch[a] = b;
                        return true;
                    }
                return false;
            };
            m_leftMatch.resize(m_leftNum, -1);
            m_rightMatch.resize(m_rightNum, -1);
            while (true) {
                bfs();
                uint32_t augument = 0;
                for (uint32_t a = 0; a < m_leftNum; a++)
                    if (!~m_leftMatch[a] && dfs(dfs, a)) augument++;
                if (!augument) break;
                res += augument;
                dfn++;
            }
            return res;
        }
    };
}

#endif