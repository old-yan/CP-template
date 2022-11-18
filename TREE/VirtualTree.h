#ifndef __OY_VIRTUALTREE__
#define __OY_VIRTUALTREE__

#include <algorithm>
#include <cstdint>

#include "../DS/Discretizer.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    template <typename _Tree>
    struct VirtualTreeFactory {
        struct _VirtualEdge {
            uint32_t from, to;
        };
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        static inline uint32_t s_vertexes[_MAXN], s_stack[_MAXN];
        HeavyLightDecomposition<_Tree> m_hld;
        _Tree &m_tree;
        VirtualTreeFactory(_Tree &__tree) : m_hld(__tree), m_tree(__tree) {}
        std::vector<_VirtualEdge> getVirtualEdges(const std::vector<uint32_t> &__keyPoints) const {
            const uint32_t n = __keyPoints.size();
            uint32_t stackLen = 0;
            std::vector<_VirtualEdge> res;
            std::copy(__keyPoints.begin(), __keyPoints.end(), s_vertexes);
            std::sort(s_vertexes, s_vertexes + n, [&](uint32_t x, uint32_t y) { return m_hld.m_pos[x] < m_hld.m_pos[y]; });
            s_stack[stackLen++] = m_tree.m_root;
            for (uint32_t i = 0; i < n; i++) {
                if (s_vertexes[i] == s_stack[stackLen - 1]) continue;
                uint32_t lca = m_hld.lca(s_vertexes[i], s_stack[stackLen - 1]), d = m_hld.m_depth[lca], d1 = m_hld.m_depth[s_stack[stackLen - 1]];
                while (d1 > d)
                    if (uint32_t d2 = m_hld.m_depth[s_stack[stackLen - 2]]; d2 >= d)
                        res.push_back({s_stack[stackLen - 1], s_stack[stackLen - 2]}), stackLen--, d1 = d2;
                    else
                        res.push_back({s_stack[stackLen - 1], lca}), s_stack[stackLen - 1] = lca, d1 = d;
                s_stack[stackLen++] = s_vertexes[i];
            }
            while (stackLen > 1) res.push_back({s_stack[stackLen - 1], s_stack[stackLen - 2]}), stackLen--;
            return res;
        }
        void fillVirtualTree(Tree<_MAXN, bool> &__tree, Discretizer<uint32_t> &__dis, const std::vector<uint32_t> &__keyPoints) const {
            __dis.clear();
            std::vector<_VirtualEdge> edges = getVirtualEdges(__keyPoints);
            for (auto &[a, b] : edges) __dis << a << b;
            __dis.prepare();
            __tree.resize(__dis.size());
            for (auto &[a, b] : edges) __tree.addEdge(__dis.rank(a), __dis.rank(b));
            __tree.prepare();
            __tree.setRoot(m_tree.m_root);
        }
    };
}

#endif