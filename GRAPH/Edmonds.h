#ifndef __OY_EDMONDS__
#define __OY_EDMONDS__

#include "Graph.h"

namespace OY {
    template <typename _DG, typename _Tp = typename _DG::value_type>
    struct Edmonds {
        _DG &m_graph;
        _Tp m_initDistance;
        _Tp m_distance;
        std::vector<int> m_useCount;
        Edmonds(_DG &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_initDistance(__initDistance) {}
        template <bool _Getpath = false>
        bool calc(uint32_t __root) {
            struct Edge {
                uint32_t from, to;
                _Tp value;
            };
            struct Relate {
                uint32_t add, remove;
            };
            std::vector<Edge> edges;
            std::vector<Relate> relates;
            uint32_t n = m_graph.m_vertexNum, m_begin = 0, m_end = m_graph.m_edgeNum, fromEdge[n], visit[n], id[n], loopCount;
            _Tp in[n];
            edges.reserve(n * m_end);
            for (auto [from, to, value] : m_graph.getEdgesData()) edges.push_back({from, to, value});
            if constexpr (_Getpath) {
                relates.reserve(n * m_end);
                relates.resize(m_end);
                m_useCount.reserve(n * m_end);
                m_useCount.clear();
                m_useCount.resize(m_end, 0);
            }
            m_distance = 0;
            while (true) {
                std::fill(in, in + n, m_initDistance);
                std::fill(visit, visit + n, -1);
                std::fill(id, id + n, -1);
                loopCount = 0;
                for (uint32_t i = m_begin; i < m_end; i++)
                    if (auto &[from, to, value] = edges[i]; chmin(in[to], value)) fromEdge[to] = i;
                visit[__root] = __root;
                for (uint32_t i = 0; i < n; i++)
                    if (i != __root) {
                        if (in[i] == m_initDistance) return false;
                        if constexpr (_Getpath) m_useCount[fromEdge[i]]++;
                        m_distance += in[i];
                        uint32_t cur;
                        for (cur = i; ~cur && !~visit[cur]; cur = edges[fromEdge[cur]].from) visit[cur] = i;
                        if (~cur && visit[cur] == i) {
                            for (; !~id[cur]; cur = edges[fromEdge[cur]].from) id[cur] = loopCount;
                            loopCount++;
                        }
                    }
                if (!loopCount) break;
                for (uint32_t i = 0; i < n; i++)
                    if (!~id[i]) id[i] = loopCount++;
                for (uint32_t i = m_begin; i < m_end; i++)
                    if (auto &[from, to, value] = edges[i]; id[from] != id[to]) {
                        edges.push_back({id[from], id[to], value - in[to]});
                        if constexpr (_Getpath) relates.push_back({i, fromEdge[to]});
                    }
                m_begin = m_end;
                m_end = edges.size();
                n = loopCount;
                __root = id[__root];
            };
            if constexpr (_Getpath)
                for (uint32_t i = m_end - 1; i >= m_graph.m_edgeNum; i--) {
                    m_useCount[relates[i].add] += m_useCount[i];
                    m_useCount[relates[i].remove] -= m_useCount[i];
                    m_useCount.pop_back();
                }
            return true;
        }
    };
}

#endif