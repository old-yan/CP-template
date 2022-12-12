#ifndef __OY_BRONKERBOSCH__
#define __OY_BRONKERBOSCH__

#include <algorithm>
#include <bitset>
#include <cstdint>

#include "Graph.h"

namespace OY {
    template <uint32_t _MAXN>
    struct BronKerbosch_MaxClique {
        uint32_t m_vertexNum;
        std::bitset<_MAXN> m_adj[_MAXN];
        BronKerbosch_MaxClique(uint32_t __vertexNum) : m_vertexNum(__vertexNum) {}
        void addEdge(uint32_t __a, uint32_t __b) {
            if (__a > __b) std::swap(__a, __b);
            if (__a < __b) m_adj[__a].set(__b);
        }
        uint32_t calc() const {
            uint32_t suffixLongest[m_vertexNum], res = 0;
            for (uint32_t first = m_vertexNum - 1; ~first; first--) {
                auto dfs = [&](auto self, uint32_t cur, uint32_t curlen, std::bitset<_MAXN> next) -> bool {
                    if (++curlen + next.count() > res)
                        if (next.any())
                            for (uint32_t nx = next._Find_next(cur); nx < m_vertexNum && curlen + suffixLongest[nx] > res; nx = next._Find_next(nx)) {
                                if (self(self, nx, curlen, next & m_adj[nx]))
                                    return true;
                            }
                        else if (chmax(res, curlen))
                            return true;
                    return false;
                };
                dfs(dfs, first, 0, m_adj[first]);
                suffixLongest[first] = res;
            }
            return res;
        }
        std::vector<std::vector<uint32_t>> searchAll(uint32_t __maxLength) const {
            std::vector<std::vector<uint32_t>> res;
            uint32_t curMax = 0, path[m_vertexNum], suffixLongest[m_vertexNum];
            for (uint32_t first = m_vertexNum - 1; ~first; first--) {
                auto dfs = [&](auto self, uint32_t cur, uint32_t curlen, std::bitset<_MAXN> &&next) {
                    if (path[curlen++] = cur; curlen + next.count() < curMax) return;
                    if (next.any())
                        for (uint32_t nx = next._Find_next(cur); nx < m_vertexNum && curlen + suffixLongest[nx] >= curMax; nx = next._Find_next(nx)) self(self, nx, curlen, next & m_adj[nx]);
                    else if (chmax(curMax, curlen); curlen == __maxLength)
                        res.template emplace_back<uint32_t *>(path, path + curlen);
                };
                dfs(dfs, first, 0, m_adj[first]);
                suffixLongest[first] = curMax;
            }
            return res;
        }
    };
    template <uint32_t _MAXN>
    struct BronKerbosch_MaxIndependantSet {
        uint32_t m_vertexNum;
        std::bitset<_MAXN> m_adj[_MAXN];
        BronKerbosch_MaxIndependantSet(uint32_t __vertexNum) : m_vertexNum(__vertexNum) {
            for (uint32_t i = 0; i < m_vertexNum; i++)
                for (uint32_t j = i + 1; j < m_vertexNum; j++) m_adj[i].set(j);
        }
        void addEdge(uint32_t __a, uint32_t __b) {
            if (__a > __b) std::swap(__a, __b);
            m_adj[__a].reset(__b);
        }
        uint32_t calc() const {
            uint32_t suffixLongest[m_vertexNum], res = 0;
            for (uint32_t first = m_vertexNum - 1; ~first; first--) {
                auto dfs = [&](auto self, uint32_t cur, uint32_t curlen, std::bitset<_MAXN> next) -> bool {
                    if (++curlen + next.count() > res)
                        if (next.any())
                            for (uint32_t nx = next._Find_next(cur); nx < m_vertexNum && curlen + suffixLongest[nx] > res; nx = next._Find_next(nx)) {
                                if (self(self, nx, curlen, next & m_adj[nx]))
                                    return true;
                            }
                        else if (chmax(res, curlen))
                            return true;
                    return false;
                };
                dfs(dfs, first, 0, m_adj[first]);
                suffixLongest[first] = res;
            }
            return res;
        }
        std::vector<std::vector<uint32_t>> searchAll(uint32_t __maxLength) const {
            std::vector<std::vector<uint32_t>> res;
            uint32_t curMax = 0, path[m_vertexNum], suffixLongest[m_vertexNum];
            for (uint32_t first = m_vertexNum - 1; ~first; first--) {
                auto dfs = [&](auto self, uint32_t cur, uint32_t curlen, std::bitset<_MAXN> &&next) {
                    if (path[curlen++] = cur; curlen + next.count() < curMax) return;
                    if (next.any())
                        for (uint32_t nx = next._Find_next(cur); nx < m_vertexNum && curlen + suffixLongest[nx] >= curMax; nx = next._Find_next(nx)) self(self, nx, curlen, next & m_adj[nx]);
                    else if (chmax(curMax, curlen); curlen == __maxLength)
                        res.template emplace_back<uint32_t *>(path, path + curlen);
                };
                dfs(dfs, first, 0, m_adj[first]);
                suffixLongest[first] = curMax;
            }
            return res;
        }
    };
}

#endif