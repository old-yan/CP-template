#ifndef __OY_CACTUS__
#define __OY_CACTUS__

#include "BlockTree.h"

namespace OY {
    template <typename _Tp = bool>
    struct Cactus : BlockTree<_Tp> {
        using BlockTree<_Tp>::BlockTree, BlockTree<_Tp>::m_tarjan, BlockTree<_Tp>::m_tree;
        uint32_t maxIndependantSet() const {
            auto dfs = [&](auto self, uint32_t i) -> std::array<int, 2> {
                int notake = 0, take = 1;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        auto &loop = m_tarjan.m_vbcc[to - m_tarjan.m_vertexNum];
                        std::array<int, 4> dp{0, 0, INT_MIN, 0};
                        for (uint32_t j = 1; j < loop.size(); j++) {
                            auto [notake, take] = self(self, loop[j]);
                            dp = {std::max(dp[0], dp[1]) + notake, std::max(dp[1] + notake, dp[0] + take), std::max(dp[2], dp[3]) + notake, std::max(dp[3] + notake, dp[2] + take)};
                        }
                        notake += std::max(dp[0], dp[1]), take += dp[2];
                    }
                return {notake, take};
            };
            auto [notake, take] = dfs(dfs, 0);
            return std::max(notake, take);
        }
    };
    Cactus(uint32_t, uint32_t)->Cactus<bool>;
}

#endif