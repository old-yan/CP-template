/*
最后修改:
20240622
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TREEDIAMETER__
#define __OY_TREEDIAMETER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace TREEDIAM {
        using size_type = uint32_t;
        template <typename Tp>
        struct DiameterInfo {
            size_type m_source, m_target;
            std::vector<Tp> m_distance;
            std::vector<size_type> m_parent;
            Tp length() const { return m_distance[m_target]; }
            template <typename Callback>
            void trace(Callback &&call) const {
                size_type leaf = m_target;
                do call(leaf);
                while (~(leaf = m_parent[leaf]));
            }
        };
        template <typename Tp, typename Tree>
        inline DiameterInfo<Tp> solve(const Tree &tree) {
            DiameterInfo<Tp> res;
            res.m_distance.resize(tree.vertex_cnt());
            res.m_parent.resize(tree.vertex_cnt());
            auto pre_work = [&](size_type a, size_type p, Tp e) { res.m_distance[a] = ~p ? res.m_distance[p] + e : 0; };
            tree.tree_dp_edge(0, pre_work, {}, {});
            res.m_source = std::max_element(res.m_distance.begin(), res.m_distance.end()) - res.m_distance.begin();
            auto pre_work2 = [&](size_type a, size_type p, Tp e) { res.m_parent[a] = p, res.m_distance[a] = ~p ? res.m_distance[p] + e : 0; };
            tree.tree_dp_edge(res.m_source, pre_work2, {}, {});
            res.m_target = std::max_element(res.m_distance.begin(), res.m_distance.end()) - res.m_distance.begin();
            return res;
        }
    }
}

#endif