#ifndef __OY_GOMORYHUTREE__
#define __OY_GOMORYHUTREE__

#include "FlowHelper.h"

namespace OY {
    template <typename _Tp, template <typename...> typename _Solver>
    struct GomoryHuTree : FlowHelper<_Tp, _Solver> {
        using FlowHelper<_Tp, _Solver>::m_vertexNum, FlowHelper<_Tp, _Solver>::m_starts, FlowHelper<_Tp, _Solver>::m_rawEdges, FlowHelper<_Tp, _Solver>::m_edges;
        std::vector<std::vector<_Tp>> m_minCut;
        GomoryHuTree(uint32_t __vertexNum, uint32_t __edgeNum) : FlowHelper<_Tp, _Solver>(__vertexNum, __edgeNum * 2) {}
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap) { FlowHelper<_Tp, _Solver>::addEdge(__a, __b, __cap), FlowHelper<_Tp, _Solver>::addEdge(__b, __a, __cap); }
        void prepare() { FlowHelper<_Tp, _Solver>::prepare(); }
        void calc(_Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            uint32_t vertexes[m_vertexNum], queue[m_vertexNum], depth[m_vertexNum];
            for (uint32_t i = 0; i < m_vertexNum; i++) vertexes[i] = i;
            m_minCut.resize(m_vertexNum, std::vector<_Tp>(m_vertexNum, __infiniteCap));
            auto dfs = [&](auto self, uint32_t l, uint32_t r) {
                if (l == r) return;
                uint32_t source = vertexes[l], target = vertexes[l + 1];
                auto flow = FlowHelper<_Tp, _Solver>::calc(source, target, __infiniteCap);
                std::fill_n(depth, m_vertexNum, -1);
                uint32_t head = 0, tail = 0;
                depth[vertexes[l]] = 0;
                queue[tail++] = vertexes[l];
                while (head < tail)
                    for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (m_edges[cur].cap && chmin(depth[m_edges[cur].to], depth[from] + 1)) queue[tail++] = m_edges[cur].to;
                auto split = std::partition(vertexes + l + 2, vertexes + r + 1, [&](uint32_t x) { return ~depth[x]; }) - vertexes - 1;
                std::swap(vertexes[l + 1], vertexes[split]);
                FlowHelper<_Tp, _Solver>::recover();
                self(self, l, split - 1), self(self, split, r);
                for (uint32_t i = l; i < split; i++)
                    for (uint32_t j = split; j <= r; j++) {
                        uint32_t a = vertexes[i], b = vertexes[j];
                        m_minCut[a][b] = m_minCut[b][a] = std::min({m_minCut[source][a], flow, m_minCut[target][b]});
                    }
            };
            dfs(dfs, 0, m_vertexNum - 1);
        }
        _Tp query(uint32_t __a, uint32_t __b) const { return m_minCut[__a][__b]; }
    };
}

#endif