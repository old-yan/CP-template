#ifndef __OY_VECTORTREETRIE__
#define __OY_VECTORTREETRIE__

#include <algorithm>
#include <cstdint>
#include <map>

#include "VectorTree.h"

namespace OY {
    struct _VectorTreeTrieNode {
        uint32_t m_id;
        std::map<uint32_t, _VectorTreeTrieNode *> m_sons;
        _VectorTreeTrieNode(uint32_t __id) : m_id(__id) {}
    };
    _VectorTreeTrieNode g_vectorTreeTrie(-1);
    uint32_t g_vectorTreeTrieIdCount = -1;
    template <typename _Tree>
    std::vector<uint32_t> getVectorTreeTrieId(_Tree &__tree, uint32_t __root) {
        __tree.setRoot(__root);
        std::vector<uint32_t> res(__tree.m_vertexNum);
        uint32_t deg[__tree.m_vertexNum], queue[__tree.m_vertexNum], head = 0, tail = 0;
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++) deg[i] = __tree.m_adj[i].size() - (i != __root);
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++)
            if (!deg[i]) queue[tail++] = i;
        while (head < tail) {
            uint32_t i = queue[head++], childNum = __tree.m_adj[i].size() - (i != __root), childId[childNum], index = 0;
            for (auto &adj : __tree.m_adj[i])
                if (uint32_t to = adj.to; to != __tree.m_parent[i]) childId[index++] = res[to];
            std::sort(childId, childId + childNum);
            _VectorTreeTrieNode *cur = &g_vectorTreeTrie;
            for (uint32_t a : childId) {
                auto it = cur->m_sons.try_emplace(a, nullptr).first;
                if (!it->second) it->second = new _VectorTreeTrieNode(-1);
                cur = it->second;
            }
            if (!~cur->m_id) cur->m_id = ++g_vectorTreeTrieIdCount;
            res[i] = cur->m_id;
            if (uint32_t p = __tree.m_parent[i]; ~p && !--deg[p]) queue[tail++] = p;
        }
        return res;
    }
    template <typename _Tree>
    std::pair<uint32_t, uint32_t> getVectorTreeTrieId(_Tree &__tree) {
        auto centroid = __tree.getCentroid();
        auto &[r1, r2] = centroid;
        r1 = getVectorTreeTrieId(__tree, r1)[r1];
        if (~r2) r2 = getVectorTreeTrieId(__tree, r2)[r2];
        if (r1 > r2) std::swap(r1, r2);
        return centroid;
    }
}

#endif