#ifndef __OY_HUNGARIAN__
#define __OY_HUNGARIAN__

#include <cstdint>
#include "Graph.h"

namespace OY {
    struct Hungarian {
        struct _RawEdge {
            uint32_t a, b;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<uint32_t> m_edges;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_leftMatch;
        std::vector<uint32_t> m_rightMatch;
        uint32_t m_leftNum;
        uint32_t m_rightNum;
        Hungarian(uint32_t __leftNum, uint32_t __rightNum) : m_starts(__leftNum + 1), m_leftNum(__leftNum), m_rightNum(__rightNum) {}
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void build() {
            for (auto &[a, b] : m_rawEdges) m_starts[a + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_leftNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_leftNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[a, b] = m_rawEdges[index];
                m_edges[cursor[a]++] = b;
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
                    if (uint32_t b = m_edges[cur]; !~m_rightMatch[b]) {
                        m_rightMatch[b] = a;
                        m_leftMatch[a] = b;
                        return true;
                    }
                for (uint32_t cur = m_starts[a], end = m_starts[a + 1]; cur < end; cur++)
                    if (uint32_t b = m_edges[cur]; visit[m_rightMatch[b]] != dfn && self(self, m_rightMatch[b])) {
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