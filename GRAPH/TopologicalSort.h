#ifndef __OY_TOPOLOGICALSORT__
#define __OY_TOPOLOGICALSORT__

#include <algorithm>
#include <cstdint>
#include <string>
#include "Graph.h"

namespace OY {
    struct TopologicalSort {
        struct _RawEdge {
            uint32_t from, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<uint32_t> m_adjs;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_queue;
        uint32_t m_vertexNum;
        uint32_t m_sourceCount;
        uint32_t m_levelCount;
        TopologicalSort(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void build() {
            for (auto &[from, to] : m_rawEdges) m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_adjs.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to] = m_rawEdges[index];
                m_adjs[cursor[from]++] = to;
            }
        }
        bool calc() {
            build();
            uint32_t deg[m_vertexNum];
            std::fill(deg, deg + m_vertexNum, 0);
            for (uint32_t to : m_adjs) deg[to]++;
            m_queue.reserve(m_vertexNum);
            uint32_t cursor = 0;
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!deg[i]) m_queue.push_back(i);
            m_sourceCount = m_queue.size();
            m_levelCount = 0;
            while (uint32_t levelNum = m_queue.size() - cursor) {
                while (levelNum--)
                    for (uint32_t from = m_queue[cursor++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (uint32_t to = m_adjs[cur]; !--deg[to]) m_queue.push_back(to);
                m_levelCount++;
            }
            return m_queue.size() == m_vertexNum;
        }
        std::basic_string_view<uint32_t> getSources() const { return std::basic_string_view<uint32_t>(m_queue.data(), m_sourceCount); }
    };
};

#endif