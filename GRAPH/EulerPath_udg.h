#ifndef __OY_EULERPATH_UDG__
#define __OY_EULERPATH_UDG__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    struct EulerPath_udg {
        struct _RawEdge {
            uint32_t from, to;
        };
        struct _Edge {
            uint32_t index, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        std::vector<bool> m_appear;
        std::vector<int> m_deg;
        uint32_t m_vertexNum;
        uint32_t m_appearCount;
        uint32_t m_zeroDegCount;
        EulerPath_udg(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum), m_appear(__vertexNum, false), m_deg(__vertexNum, 0) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void prepare() {
            for (auto &[from, to] : m_rawEdges) {
                m_appear[from] = m_appear[to] = true;
                m_starts[from + 1]++;
                if (from != to) m_starts[to + 1]++;
                m_deg[from] ^= 1;
                m_deg[to] ^= 1;
            }
            m_appearCount = std::count(m_appear.begin(), m_appear.end(), true);
            m_zeroDegCount = std::count(m_deg.begin(), m_deg.end(), 0);
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
        bool isEulerGraph() const {
            if (m_zeroDegCount != m_vertexNum) return false;
            uint32_t queue[m_appearCount], head = 0, tail = 0;
            std::vector<bool> visit(m_vertexNum, false);
            uint32_t i = std::find(m_appear.begin(), m_appear.end(), true) - m_appear.begin();
            visit[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; !visit[to]) {
                        visit[to] = true;
                        queue[tail++] = to;
                    }
            return tail == m_appearCount;
        }
        uint32_t getHalfEulerSource() const {
            if (m_zeroDegCount + 2 < m_vertexNum) return -1;
            uint32_t i = 0;
            while (i < m_appear.size() && (!m_appear[i] || m_deg[i] == 0)) i++;
            if (i == m_appear.size()) i = std::find(m_appear.begin(), m_appear.end(), true) - m_appear.begin();
            uint32_t queue[m_appearCount], head = 0, tail = 0;
            std::vector<bool> visit(m_vertexNum, false);
            visit[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; !visit[to]) {
                        visit[to] = true;
                        queue[tail++] = to;
                    }
            return tail == m_appearCount ? i : -1;
        }
        template <bool _EdgePath>
        std::vector<uint32_t> getPath(uint32_t __source) const {
            uint32_t it[m_vertexNum], end[m_vertexNum];
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                it[i] = m_starts[i];
                end[i] = m_starts[i + 1];
            }
            std::vector<uint32_t> path;
            path.reserve(_EdgePath ? m_edges.size() : m_edges.size() + 1);
            std::vector<bool> visit(m_rawEdges.size(), false);
            auto dfs = [&](auto self, uint32_t cur, uint32_t from) -> void {
                while (it[cur] != end[cur]) {
                    auto &[index, to] = m_edges[it[cur]++];
                    if (!visit[index]) {
                        visit[index] = true;
                        self(self, to, index);
                    }
                }
                if constexpr (!_EdgePath)
                    path.push_back(cur);
                else {
                    if (~from) path.push_back(from);
                }
            };
            dfs(dfs, __source, -1);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<uint32_t> getPath_edge(uint32_t __source) const { return getPath<true>(__source); }
        std::vector<uint32_t> getPath_vertex(uint32_t __source) const { return getPath<false>(__source); }
    };
}

#endif