#ifndef __OY_HUNGARIAN__
#define __OY_HUNGARIAN__

#include <cstdint>
#include "Graph.h"

namespace OY {
    struct Hungarian {
        struct _Edge {
            uint32_t a, b;
        };
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_adj;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_leftMatch;
        std::vector<uint32_t> m_rightMatch;
        uint32_t m_leftNum;
        uint32_t m_rightNum;
        Hungarian(uint32_t __leftNum, uint32_t __rightNum, uint32_t __edgeNum) : m_starts(__leftNum + 1), m_leftNum(__leftNum), m_rightNum(__rightNum) {m_edges.reserve(__edgeNum);}
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
            m_leftMatch.resize(m_leftNum, -1);
            m_rightMatch.resize(m_rightNum, -1);
            std::vector<uint32_t> visit(m_leftNum, -1);
            uint32_t dfn = 0;
            auto dfs = [&](auto self, uint32_t a) {
                visit[a] = dfn;
                for (uint32_t cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                    if (uint32_t b = m_adj[cur]; !~m_rightMatch[b]) {
                        m_rightMatch[b] = a;
                        m_leftMatch[a] = b;
                        return true;
                    }
                for (uint32_t cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                    if (uint32_t b = m_adj[cur]; visit[m_rightMatch[b]] != dfn && self(self, m_rightMatch[b])) {
                        m_rightMatch[b] = a;
                        m_leftMatch[a] = b;
                        return true;
                    }
                return false;
            };
            uint32_t res = 0;
            while (true) {
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