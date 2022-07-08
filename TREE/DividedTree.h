#ifndef __OY_DIVIDEDTREE__
#define __OY_DIVIDEDTREE__

#include <bitset>
#include "Tree.h"

namespace OY {
    template <typename _Tree>
    auto getDivideTree(_Tree &__tree) {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        Tree<_MAXN> res(__tree.m_vertexNum);
        std::bitset<_MAXN> banList;
        uint32_t queue[_MAXN];
        auto dfs = [&](auto self, uint32_t i) -> uint32_t {
            uint32_t head = 0, tail = 0;
            banList[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (uint32_t j = queue[head++], cur = __tree.m_starts[j], end = __tree.m_starts[j + 1]; cur != end; cur++)
                    if (uint32_t to = __tree.m_to[cur]; !banList[to]) {
                        banList[to] = true;
                        queue[tail++] = to;
                    }
            for (uint32_t i = 0; i < tail; i++) banList[queue[i]] = false;
            uint32_t centroid;
            auto traverse = [&](auto self, uint32_t i, uint32_t p) -> uint32_t {
                uint32_t size = 1, maxAdj = 0;
                for (uint32_t cur = __tree.m_starts[i], end = __tree.m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = __tree.m_to[cur]; to != p && !banList[to]) {
                        uint32_t subSize = self(self, to, i);
                        size += subSize;
                        chmax(maxAdj, subSize);
                    }
                chmax(maxAdj, tail - size);
                if (maxAdj <= tail / 2) centroid = i;
                return size;
            };
            traverse(traverse, i, -1);
            banList[centroid] = true;
            for (uint32_t cur = __tree.m_starts[centroid], end = __tree.m_starts[centroid + 1]; cur != end; cur++)
                if (uint32_t to = __tree.m_to[cur]; !banList[to]) res.addEdge(centroid, self(self, to));
            banList[centroid] = false;
            return centroid;
        };
        uint32_t root = dfs(dfs, 0);
        res.prepare();
        res.setRoot(root);
        return res;
    };
}

#endif