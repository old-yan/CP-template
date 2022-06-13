#ifndef __OY_EDMONDS__
#define __OY_EDMONDS__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Tp>
    struct Edmonds {
        struct _Edge {
            uint32_t from, to;
            _Tp cost;
        };
        std::vector<_Edge> m_edges;
        uint32_t m_vertexNum;
        _Tp m_totalCost;
        std::vector<int> m_useCount;
        Edmonds(uint32_t __vertexNum, uint32_t __edgeNum) : m_vertexNum(__vertexNum), m_totalCost(0) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_edges.push_back({__a, __b, __cost}); }
        template <bool _Getpath = false>
        bool calc(uint32_t __root, _Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) {
            struct _relate {
                uint32_t add, remove;
            };
            std::vector<_relate> relates;
            uint32_t n = m_vertexNum, m = m_edges.size(), begin = 0, end = m, fromEdge[n], visit[n], id[n], loopCount;
            m_edges.reserve(n * m);
            _Tp in[n];
            if constexpr (_Getpath) {
                relates.reserve(n * m);
                relates.resize(m);
                m_useCount.reserve(n * m);
                m_useCount.resize(m, 0);
            }
            while (true) {
                std::fill(in, in + n, __infiniteCost);
                std::fill(visit, visit + n, -1);
                std::fill(id, id + n, -1);
                loopCount = 0;
                for (uint32_t i = begin; i < end; i++)
                    if (auto &[from, to, cost] = m_edges[i]; chmin(in[to], cost)) fromEdge[to] = i;
                visit[__root] = __root;
                for (uint32_t i = 0; i < n; i++)
                    if (i != __root) {
                        if (in[i] == __infiniteCost) return false;
                        if constexpr (_Getpath) m_useCount[fromEdge[i]]++;
                        m_totalCost += in[i];
                        uint32_t cur;
                        for (cur = i; ~cur && !~visit[cur]; cur = m_edges[fromEdge[cur]].from) visit[cur] = i;
                        if (~cur && visit[cur] == i) {
                            for (; !~id[cur]; cur = m_edges[fromEdge[cur]].from) id[cur] = loopCount;
                            loopCount++;
                        }
                    }
                if (!loopCount) break;
                for (uint32_t i = 0; i < n; i++)
                    if (!~id[i]) id[i] = loopCount++;
                for (uint32_t i = begin; i < end; i++)
                    if (auto &[from, to, cost] = m_edges[i]; id[from] != id[to]) {
                        m_edges.push_back({id[from], id[to], cost - in[to]});
                        if constexpr (_Getpath) relates.push_back({i, fromEdge[to]});
                    }
                begin = end;
                end = m_edges.size();
                n = loopCount;
                __root = id[__root];
            };
            if constexpr (_Getpath)
                for (uint32_t i = end - 1; i >= m; i--) {
                    m_useCount[relates[i].add] += m_useCount[i];
                    m_useCount[relates[i].remove] -= m_useCount[i];
                    m_useCount.pop_back();
                }
            return true;
        }
    };
}

#endif