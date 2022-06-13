#ifndef __OY_BIPARTITECHECK__
#define __OY_BIPARTITECHECK__

#include "Graph.h"

namespace OY {
    struct BipartiteCheck {
        struct _RawEdge {
            uint32_t from, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<uint32_t> m_adjs;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_color;
        uint32_t m_vertexNum;
        uint32_t m_colorCount;
        BipartiteCheck(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_color(__vertexNum, -1), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void build() {
            for (auto &[from, to] : m_rawEdges) {
                m_starts[from + 1]++;
                if (from != to) m_starts[to + 1]++;
            }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_adjs.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to] = m_rawEdges[index];
                m_adjs[cursor[from]++] = to;
                if (from != to) m_adjs[cursor[to]++] = from;
            }
        }
        bool calc() {
            build();
            uint32_t queue[m_vertexNum];
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!~m_color[i]) {
                    uint32_t head = 0, tail = 0;
                    m_color[i] = m_colorCount;
                    queue[tail++] = i;
                    while (head < tail)
                        for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                            if (uint32_t to = m_adjs[cur]; !~m_color[to]) {
                                m_color[to] = m_color[from] ^ 1;
                                queue[tail++] = to;
                            } else if (m_color[to] == m_color[from])
                                return false;
                    m_colorCount += 2;
                }
            return true;
        }
        std::vector<uint32_t> getColorMap() const {
            std::vector<uint32_t> res(m_colorCount, 0);
            for (uint32_t i = 0; i < m_vertexNum; i++) res[m_color[i]]++;
            return res;
        }
    };
}

#endif