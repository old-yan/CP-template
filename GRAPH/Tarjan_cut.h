#ifndef __OY_TARJAN_CUT__
#define __OY_TARJAN_CUT__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>

#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct _TarjanCutEdgeWrap {
        struct _Edge {
            uint32_t a, b;
            _Tp distance;
            _Edge(uint32_t __a, uint32_t __b, _Tp __distance = _Tp()) : a(__a), b(__b), distance(__distance) {}
            const _Tp &getDis() const { return distance; }
        };
    };
    template <>
    struct _TarjanCutEdgeWrap<bool> {
        struct _Edge {
            uint32_t a, b;
            _Edge(uint32_t __a, uint32_t __b) : a(__a), b(__b) {}
            constexpr bool getDis() const { return true; }
        };
    };
    template <typename _Tp = bool>
    struct Tarjan_cut {
        using distance_type = std::conditional_t<std::is_same_v<_Tp, bool>, uint32_t, _Tp>;
        struct _Edge {
            uint32_t index, to;
        };
        std::vector<typename _TarjanCutEdgeWrap<_Tp>::_Edge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts, m_dfn, m_low, m_adj, m_stack;
        uint32_t m_vertexNum, m_dfnCount, m_cutCount;
        std::basic_string<uint32_t> m_vbccBuffer, m_edgeVbccBuffer;
        std::vector<std::basic_string_view<uint32_t>> m_vbcc;
        Tarjan_cut(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_dfn(__vertexNum, -1), m_low(__vertexNum, -1), m_adj(__vertexNum, 0), m_dfnCount(0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_rawEdges.push_back({__a, __b, __distance}); }
        template <bool _GetVbcc = false>
        void calc() {
            for (auto &edge : m_rawEdges) {
                m_starts[edge.a + 1]++;
                if (edge.a != edge.b) m_starts[edge.b + 1]++;
            }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &edge = m_rawEdges[index];
                m_edges[cursor[edge.a]++] = _Edge{index, edge.b};
                if (edge.a != edge.b) m_edges[cursor[edge.b]++] = _Edge{index, edge.a};
            }
            if constexpr (_GetVbcc) {
                m_vbccBuffer.reserve(m_edges.size() + m_vertexNum);
                m_vbcc.reserve(m_vertexNum);
                m_stack.reserve(m_vertexNum);
            }
            auto dfs = [this](auto self, uint32_t i, uint32_t fromIndex) -> void {
                m_dfn[i] = m_low[i] = m_dfnCount++;
                uint32_t stackLength = m_stack.size();
                if constexpr (_GetVbcc) m_stack.push_back(i);
                if (~fromIndex) m_adj[i]++;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; !~m_dfn[to]) {
                        self(self, to, index);
                        if (m_low[to] < m_dfn[i])
                            chmin(m_low[i], m_low[to]);
                        else
                            m_adj[i]++;
                    } else if (index != fromIndex)
                        chmin(m_low[i], m_dfn[to]);
                if constexpr (_GetVbcc) {
                    if (!~fromIndex) {
                        if (!m_adj[i]) {
                            uint32_t len = m_stack.size() - stackLength;
                            m_vbcc.emplace_back(m_vbccBuffer.data() + m_vbccBuffer.size(), len);
                            m_vbccBuffer.insert(m_vbccBuffer.end(), m_stack.data() + stackLength, m_stack.data() + m_stack.size());
                        }
                        m_stack.resize(stackLength);
                    } else if (uint32_t parent = m_rawEdges[fromIndex].a ^ m_rawEdges[fromIndex].b ^ i; m_low[i] >= m_dfn[parent]) {
                        std::swap(m_stack[stackLength - 1], parent);
                        uint32_t len = m_stack.size() - stackLength + 1;
                        m_vbcc.emplace_back(m_vbccBuffer.data() + m_vbccBuffer.size(), len);
                        m_vbccBuffer.insert(m_vbccBuffer.end(), m_stack.data() + stackLength - 1, m_stack.data() + m_stack.size());
                        std::swap(m_stack[stackLength - 1], parent);
                        m_stack.resize(stackLength);
                    }
                }
            };
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!~m_dfn[i]) dfs(dfs, i, -1);
            m_cutCount = std::count_if(m_adj.begin(), m_adj.end(), [](uint32_t x) { return x > 1; });
        }
        std::vector<std::basic_string_view<uint32_t>> getEdgeVBCC() {
            std::vector<std::basic_string_view<uint32_t>> res;
            std::vector<bool> visit(m_vertexNum, false);
            m_edgeVbccBuffer.reserve(m_rawEdges.size());
            m_stack.reserve(m_rawEdges.size());
            res.reserve(m_vertexNum);
            auto dfs = [&](auto self, uint32_t i, uint32_t fromIndex) -> void {
                visit[i] = true;
                uint32_t stackLength = m_stack.size();
                if (~fromIndex) m_stack.push_back(fromIndex);
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; !visit[to])
                        self(self, to, index);
                    else if (index != fromIndex && m_dfn[to] <= m_dfn[i])
                        m_stack.push_back(index);
                if (!~fromIndex) {
                    if (!m_adj[i]) {
                        uint32_t len = m_stack.size() - stackLength;
                        res.emplace_back(m_edgeVbccBuffer.data() + m_edgeVbccBuffer.size(), len);
                        m_edgeVbccBuffer.insert(m_edgeVbccBuffer.end(), m_stack.data() + stackLength, m_stack.data() + m_stack.size());
                    }
                    m_stack.resize(stackLength);
                } else if (uint32_t parent = m_rawEdges[fromIndex].a ^ m_rawEdges[fromIndex].b ^ i; m_low[i] >= m_dfn[parent]) {
                    uint32_t len = m_stack.size() - stackLength;
                    res.emplace_back(m_edgeVbccBuffer.data() + m_edgeVbccBuffer.size(), len);
                    m_edgeVbccBuffer.insert(m_edgeVbccBuffer.end(), m_stack.data() + stackLength, m_stack.data() + m_stack.size());
                    m_stack.resize(stackLength);
                }
            };
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!visit[i]) dfs(dfs, i, -1);
            return res;
        }
        std::pair<std::vector<distance_type>, std::vector<distance_type>> getCactusCyclePos() {
            std::pair<std::vector<distance_type>, std::vector<distance_type>> res;
            res.first.reserve(m_vertexNum);
            res.second.resize(m_vertexNum);
            std::vector<bool> visit(m_vertexNum, false);
            auto dfs = [&](auto self, uint32_t i, uint32_t fromIndex, distance_type d) -> distance_type {
                distance_type last = 0;
                res.second[i] = d, visit[i] = true;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; !visit[to])
                        last += self(self, to, index, d + m_rawEdges[index].getDis());
                    else if (index != fromIndex && m_dfn[to] < m_dfn[i])
                        last += d + m_rawEdges[index].getDis() - res.second[to];
                if (!~fromIndex) {
                    if (!m_adj[i]) res.first.push_back(last);
                    return 0;
                } else if (uint32_t parent = m_rawEdges[fromIndex].a ^ m_rawEdges[fromIndex].b ^ i; m_low[i] >= m_dfn[parent]) {
                    res.first.push_back(last);
                    return 0;
                } else
                    return last;
            };
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!visit[i]) dfs(dfs, i, -1, 0);
            return res;
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
    Tarjan_cut(uint32_t, uint32_t)->Tarjan_cut<bool>;
}

#endif