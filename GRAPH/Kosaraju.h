#ifndef __OY_KOSARAJU__
#define __OY_KOSARAJU__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include "Graph.h"

namespace OY {
    struct Kosaraju {
        struct _Edge {
            uint32_t from, to;
        };
        std::vector<_Edge> m_rawEdges;
        std::vector<uint32_t> m_adj;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_reversedAdjs;
        std::vector<uint32_t> m_reversedStarts;
        std::vector<uint32_t> m_stack;
        std::vector<uint32_t> m_id;
        std::vector<uint32_t> m_topo;
        uint32_t m_vertexNum;
        uint32_t m_idCount;
        Kosaraju(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_reversedStarts(__vertexNum + 1, 0), m_vertexNum(__vertexNum), m_id(__vertexNum, 0), m_idCount(0) { m_rawEdges.reserve(__edgeNum); }
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
            for (auto &[from, to] : m_rawEdges) m_reversedStarts[to + 1]++;
            std::partial_sum(m_reversedStarts.begin(), m_reversedStarts.end(), m_reversedStarts.begin());
            m_reversedAdjs.resize(m_reversedStarts.back());
            std::copy(m_reversedStarts.begin(), m_reversedStarts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to] = m_rawEdges[index];
                m_reversedAdjs[cursor[to]++] = from;
            }
        }
        void calc() {
            m_stack.reserve(m_vertexNum);
            auto dfs = [this](auto self, uint32_t i) -> void {
                m_id[i] = -1;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (uint32_t to = m_adj[cur]; !m_id[to]) self(self, to);
                m_stack.push_back(i);
            };
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!m_id[i]) dfs(dfs, i);
            m_topo.reserve(m_vertexNum);
            uint32_t cursor = 0;
            while (m_stack.size()) {
                if (!~m_id[m_stack.back()]) {
                    m_id[m_stack.back()] = m_idCount;
                    m_topo.push_back(m_stack.back());
                    do {
                        for (uint32_t to = m_topo[cursor++], cur = m_reversedStarts[to], end = m_reversedStarts[to + 1]; cur < end; cur++)
                            if (uint32_t from = m_reversedAdjs[cur]; !~m_id[from]) {
                                m_id[from] = m_idCount;
                                m_topo.push_back(from);
                            }
                    } while (cursor < m_topo.size());
                    m_idCount++;
                }
                m_stack.pop_back();
            }
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