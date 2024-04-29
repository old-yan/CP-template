/*
最后修改:
20240429
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DFNCONTROLLER__
#define __OY_DFNCONTROLLER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace DFN {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            struct node {
                size_type m_dfn, m_size;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq;
            void _tree_dfs(size_type a, size_type p, size_type &cursor) {
                m_seq[cursor] = a;
                m_info[a].m_dfn = cursor++;
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p) _tree_dfs(to, a, cursor); });
                m_info[a].m_size = cursor - m_info[a].m_dfn;
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt());
                size_type cursor = 0;
                _tree_dfs(m_rooted_tree->m_root, -1, cursor);
            }
            template <typename Callback>
            void do_for_subtree(size_type a, Callback &&call) const { call(m_info[a].m_dfn, m_info[a].m_dfn + m_info[a].m_size - 1); }
            template <typename Callback>
            auto do_for_vertex(size_type a, Callback &&call) const -> decltype(call(0)) { return call(m_info[a].m_dfn); }
        };
    }
}

#endif