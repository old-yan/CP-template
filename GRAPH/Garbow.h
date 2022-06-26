#ifndef __OY_GARBOW__
#define __OY_GARBOW__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include "Graph.h"

namespace OY {
    struct Garbow {
        struct _Edge {
            uint32_t from, to;
        };
        std::vector<_Edge> m_rawEdges;
        std::vector<uint32_t> m_adj;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_dfn;
        std::vector<uint32_t> m_id;
        std::vector<uint32_t> m_stack1;
        std::vector<uint32_t> m_stack2;
        std::vector<uint32_t> m_topo;
        uint32_t m_vertexNum;
        uint32_t m_dfnCount;
        uint32_t m_idCount;
        Garbow(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum), m_dfn(__vertexNum, -1), m_id(__vertexNum, -1), m_dfnCount(0), m_idCount(0) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void prepare() {
            for (auto &[from, to] : m_rawEdges) m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_adj.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to] = m_rawEdges[index];
                m_adj[cursor[from]++] = to;
            }
        }
        void calc() {
            m_stack1.reserve(m_vertexNum);
            m_stack2.reserve(m_vertexNum);
            m_topo.reserve(m_vertexNum);
            auto dfs = [this](auto self, uint32_t i) -> void {
                m_stack1.push_back(i);
                m_stack2.push_back(i);
                m_dfn[i] = m_dfnCount++;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (uint32_t to = m_adj[cur]; !~m_dfn[to])
                        self(self, to);
                    else if (!~m_id[to])
                        while (m_dfn[m_stack2.back()] > m_dfn[to]) m_stack2.pop_back();
                if (m_stack2.back() == i) {
                    m_stack2.pop_back();
                    while (true) {
                        uint32_t back = m_stack1.back();
                        m_id[back] = m_idCount;
                        m_topo.push_back(back);
                        m_stack1.pop_back();
                        if (back == i) break;
                    }
                    m_idCount++;
                }
            };
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!~m_dfn[i]) dfs(dfs, i);
            for (uint32_t i = 0; i < m_vertexNum; i++) m_id[i] = m_idCount - 1 - m_id[i];
            std::reverse(m_topo.begin(), m_topo.end());
        }
        std::vector<std::basic_string_view<uint32_t>> groups() const {
            std::vector<std::basic_string_view<uint32_t>> res;
            res.reserve(m_idCount);
            for (int prev = 0, id = 0, cursor = 0; cursor < m_vertexNum; prev = cursor, id++) {
                while (cursor < m_vertexNum && m_id[m_topo[cursor]] == id) cursor++;
                res.emplace_back(m_topo.data() + prev, cursor - prev);
            }
            return res;
        }
    };
}

#endif
