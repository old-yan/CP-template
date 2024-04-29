/*
最后修改:
20231015
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MENGHANIMATANI__
#define __OY_MENGHANIMATANI__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace MenghaniMatani {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            struct node {
                size_type m_dfn, m_dep;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq, m_level, m_level_cnt;
            void _tree_dfs(size_type a, size_type p, size_type &cursor) {
                if (~p) m_info[a].m_dep = m_info[p].m_dep + 1;
                m_level_cnt[m_info[a].m_dep + 1]++;
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) {
                    if (to != p) _tree_dfs(to, a, cursor);
                });
                m_seq[cursor] = a;
                m_info[a].m_dfn = cursor++;
            }
            size_type _get_dfn(size_type dfn, size_type dep) const { return *std::lower_bound(m_level.data() + m_level_cnt[dep], m_level.data() + m_level_cnt[dep + 1], dfn); }
            size_type _get_id(size_type dfn, size_type dep) const { return m_seq[_get_dfn(dfn, dep)]; }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt()), m_level.resize(m_rooted_tree->vertex_cnt()), m_level_cnt.assign(m_rooted_tree->vertex_cnt() * 2, {});
                size_type cursor = 0, max_dep = 0;
                _tree_dfs(m_rooted_tree->m_root, -1, cursor);
                m_level_cnt[m_rooted_tree->vertex_cnt()] = 0;
                std::vector<size_type> cur(m_rooted_tree->vertex_cnt());
                for (max_dep = 1; m_level_cnt[max_dep]; max_dep++) m_level_cnt[max_dep] += m_level_cnt[max_dep - 1], cur[max_dep] = m_level_cnt[max_dep];
                if (max_dep == m_rooted_tree->vertex_cnt()) m_level_cnt[max_dep] = m_rooted_tree->vertex_cnt();
                for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) m_level[cur[m_info[m_seq[i]].m_dep]++] = i;
            }
            size_type get_ancestor(size_type a, size_type n) const { return n <= m_info[a].m_dep ? _get_id(m_info[a].m_dfn, m_info[a].m_dep - n) : -1; }
            size_type find_parent(size_type a) const { return m_info[a].m_dep ? _get_id(m_info[a].m_dfn, m_info[a].m_dep - 1) : -1; }
            size_type find_son(size_type a, size_type b) const { return _get_id(m_info[b].m_dfn, m_info[a].m_dep + 1); }
            size_type get_depth(size_type a) const { return m_info[a].m_dep; }
            size_type calc(size_type a, size_type b) const {
                size_type a_dfn = m_info[a].m_dfn, b_dfn = m_info[b].m_dfn;
                if (a_dfn > b_dfn) std::swap(a_dfn, b_dfn);
                size_type low = 0, high = std::min(m_info[a].m_dep, m_info[b].m_dep);
                while (low != high) {
                    size_type mid = (low + high + 1) / 2;
                    _get_dfn(a_dfn, mid) >= b_dfn ? (low = mid) : (high = mid - 1);
                }
                return _get_id(a_dfn, low);
            }
        };
        ;
    }
}

#endif