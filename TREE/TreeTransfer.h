/*
最后修改:
20240621
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TREETRANSFER__
#define __OY_TREETRANSFER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace TreeTransfer {
        using size_type = uint32_t;
        template <typename Tp, typename InitMapping>
        struct PreWork {
            std::vector<Tp> &m_dp;
            InitMapping &m_mapping;
            template <typename Fp>
            void operator()(size_type a, size_type p, Fp e) const { m_dp[a] = m_mapping(a); }
        };
        template <typename Tp, typename Merge>
        struct Report {
            std::vector<Tp> &m_dp;
            Merge &m_merge;
            template <typename Fp>
            void operator()(size_type a, size_type to, Fp e) const { m_merge(m_dp[a], m_dp[to], a, to, e); }
        };
        template <typename Tp, typename Exclude>
        struct ExcludeWork {
            std::vector<Tp> &m_dp;
            Exclude &m_exclude;
            template <typename Fp>
            void operator()(size_type a, size_type p, Fp e) const {
                if (~p)
                    m_exclude(m_dp[a], m_dp[p], a, p, e);
                else {
                    auto dp_null = Tp();
                    m_exclude(m_dp[a], dp_null, a, -1, e);
                }
            }
        };
        template <typename Tp, typename Tree, typename InitMapping, typename Merge, typename Exclude>
        inline std::vector<Tp> solve(const Tree &rooted_tree, InitMapping mapping, Merge &&merge, Exclude &&exclude) {
            size_type n = rooted_tree.vertex_cnt();
            std::vector<Tp> dp(n);
            rooted_tree.tree_dp_edge(rooted_tree.m_root, PreWork<Tp, InitMapping>{dp, mapping}, Report<Tp, Merge>{dp, merge}, {});
            rooted_tree.tree_dp_edge(rooted_tree.m_root, ExcludeWork<Tp, Exclude>{dp, exclude}, {}, {});
            return dp;
        }
    }
}

#endif