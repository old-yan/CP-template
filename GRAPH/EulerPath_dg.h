#ifndef __OY_EULERPATH_DG__
#define __OY_EULERPATH_DG__

#include "Graph.h"

namespace OY {
    template <typename _DG>
    struct EulerPath_dg {
        _DG &m_graph;
        std::vector<bool> m_appear;
        uint32_t m_appearCount;
        std::vector<int> m_deg;
        uint32_t m_zeroDegCount;
        EulerPath_dg(_DG &__graph) : m_graph(__graph), m_appear(__graph.m_vertexNum, false), m_deg(__graph.m_vertexNum, 0) {
            for (auto [from, to, val] : m_graph.getEdgesData()) {
                m_appear[from] = m_appear[to] = true;
                m_deg[from]--;
                m_deg[to]++;
            }
            m_appearCount = std::count(m_appear.begin(), m_appear.end(), true);
            m_zeroDegCount = std::count(m_deg.begin(), m_deg.end(), 0);
        }
        bool isEulerGraph() const {
            if (m_zeroDegCount != m_graph.m_vertexNum) return false;
            uint32_t queue[m_appearCount], head = 0, tail = 0;
            std::vector<bool> visit(m_graph.m_vertexNum, false);
            uint32_t i = std::find(m_appear.begin(), m_appear.end(), true) - m_appear.begin();
            visit[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (auto [to, value] : m_graph.getEdgesAdjOf(queue[head++]))
                    if (!visit[to]) {
                        visit[to] = true;
                        queue[tail++] = to;
                    }
            return tail == m_appearCount;
        }
        uint32_t getHalfEulerSource() const {
            if (m_zeroDegCount + 2 < m_graph.m_vertexNum) return -1;
            uint32_t i = 0;
            while (i < m_appear.size() && (!m_appear[i] || m_deg[i] == 0)) i++;
            if (i == m_appear.size()) i = std::find(m_appear.begin(), m_appear.end(), true) - m_appear.begin();
            if (m_deg[i] < -1) return -1;
            uint32_t queue[m_appearCount], head = 0, tail = 0;
            std::vector<bool> visit(m_graph.m_vertexNum, false);
            visit[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (auto [to, value] : m_graph.getEdgesAdjOf(queue[head++]))
                    if (!visit[to]) {
                        visit[to] = true;
                        queue[tail++] = to;
                    }
            return tail == m_appearCount ? i : -1;
        }
        template <bool _Getpath>
        std::vector<uint32_t> getPath(uint32_t __source) const {
            using iterator = decltype(m_graph.getEdgesInfoOf(__source).begin());
            iterator it[m_graph.m_vertexNum], end[m_graph.m_vertexNum];
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) {
                auto edges = m_graph.getEdgesInfoOf(i);
                it[i] = edges.begin();
                end[i] = edges.end();
            }
            std::vector<uint32_t> path;
            path.reserve(_Getpath ? m_graph.m_edgeNum : m_graph.m_edgeNum + 1);
            auto dfs = [&](auto self, uint32_t cur, uint32_t from) -> void {
                while (it[cur] != end[cur]) {
                    auto [index, from, to, value] = *it[cur];
                    ++it[cur];
                    self(self, to, index);
                }
                if constexpr (!_Getpath)
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