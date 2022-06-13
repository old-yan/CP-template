#ifndef __OY_KOSARAJU__
#define __OY_KOSARAJU__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include "Graph.h"

namespace OY {
    struct Kosaraju {
        struct _RawEdge {
            uint32_t from, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<uint32_t> m_adjs;
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
            build();
            m_stack.reserve(m_vertexNum);
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (!m_id[i]) dfs(i);
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
        void dfs(uint32_t __i) {
            m_id[__i] = -1;
            for (uint32_t cur = m_starts[__i], end = m_starts[__i + 1]; cur < end; cur++)
                if (uint32_t to = m_adjs[cur]; !m_id[to]) dfs(to);
            m_stack.push_back(__i);
        }
        std::vector<std::vector<uint32_t>> groups() const {
            uint32_t count[m_idCount];
            std::fill(count, count + m_idCount, 0);
            std::vector<std::vector<uint32_t>> res(m_idCount);
            for (uint32_t i = 0; i < m_idCount; i++) res[i].reserve(count[i]);
            for (uint32_t i = 0; i < m_id.size(); i++) res[m_id[i]].push_back(i);
            return res;
        }
    };
}

#endif