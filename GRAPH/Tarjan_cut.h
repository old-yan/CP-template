#ifndef __OY_TARJAN_CUT__
#define __OY_TARJAN_CUT__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include "Graph.h"

namespace OY {
    struct Tarjan_cut {
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
        std::vector<uint32_t> m_adj;
        std::vector<uint32_t> m_stack;
        uint32_t m_vertexNum;
        uint32_t m_dfnCount;
        uint32_t m_cutCount;
        std::basic_string<uint32_t> m_vbccBuffer;
        std::vector<std::basic_string_view<uint32_t>> m_vbcc;
        Tarjan_cut(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_dfn(__vertexNum, -1), m_low(__vertexNum, -1), m_adj(__vertexNum, 0), m_dfnCount(0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
        template <bool _GetVbcc = false>
        void calc() {
            build();
            if constexpr (_GetVbcc) {
                m_vbccBuffer.reserve(m_edges.size() + m_vertexNum);
                m_vbcc.reserve(m_vertexNum);
                m_stack.reserve(m_vertexNum);
            }
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!~m_dfn[i]) dfs<_GetVbcc>(i, -1);
            m_cutCount = std::count_if(m_adj.begin(), m_adj.end(), [](uint32_t x) { return x > 1; });
        }
        template <bool _GetVbcc>
        void dfs(uint32_t __i, uint32_t __fromIndex) {
            m_dfn[__i] = m_low[__i] = m_dfnCount++;
            uint32_t stackLength = m_stack.size();
            if constexpr (_GetVbcc) m_stack.push_back(__i);
            if (~__fromIndex) m_adj[__i]++;
            for (uint32_t cur = m_starts[__i], end = m_starts[__i + 1]; cur < end; cur++)
                if (auto &[index, to] = m_edges[cur]; !~m_dfn[to]) {
                    dfs<_GetVbcc>(to, index);
                    if (m_low[to] < m_dfn[__i])
                        chmin(m_low[__i], m_low[to]);
                    else
                        m_adj[__i]++;
                } else if (index != __fromIndex)
                    chmin(m_low[__i], m_dfn[to]);
            if constexpr (_GetVbcc) {
                if (!~__fromIndex) {
                    if (!m_adj[__i]) {
                        uint32_t len = m_stack.size() - stackLength;
                        m_vbcc.emplace_back(m_vbccBuffer.data() + m_vbccBuffer.size(), len);
                        m_vbccBuffer.insert(m_vbccBuffer.end(), m_stack.data() + stackLength, m_stack.data() + m_stack.size());
                    }
                    m_stack.resize(stackLength);
                } else if (uint32_t parent = m_rawEdges[__fromIndex].from ^ m_rawEdges[__fromIndex].to ^ __i; m_low[__i] >= m_dfn[parent]) {
                    std::swap(m_stack[stackLength - 1], parent);
                    uint32_t len = m_stack.size() - stackLength + 1;
                    m_vbcc.emplace_back(m_vbccBuffer.data() + m_vbccBuffer.size(), len);
                    m_vbccBuffer.insert(m_vbccBuffer.end(), m_stack.data() + stackLength - 1, m_stack.data() + m_stack.size());
                    std::swap(m_stack[stackLength - 1], parent);
                    m_stack.resize(stackLength);
                }
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