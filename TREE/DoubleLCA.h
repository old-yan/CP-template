/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DOUBLELCA__
#define __OY_DOUBLELCA__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace DoubleLCA {
        using size_type = uint32_t;
        template <typename Tree, size_type MAX_VERTEX, size_type MAX_NODE>
        struct Table {
            static size_type s_buffer[MAX_NODE], s_dep_buffer[MAX_VERTEX], s_use_count, s_dep_use_count;
            Tree *m_rooted_tree;
            size_type *m_fa, *m_dep, m_level;
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_fa = s_buffer + s_use_count, m_dep = s_dep_buffer + s_dep_use_count, s_use_count += m_rooted_tree->vertex_cnt() * std::bit_width(m_rooted_tree->vertex_cnt() - 1), s_dep_use_count += m_rooted_tree->vertex_cnt();
                m_rooted_tree->tree_dp_vertex(m_rooted_tree->m_root, [&](size_type a, size_type p) { m_fa[a] = p, m_dep[a] = ~p ? m_dep[p] + 1 : 0; }, {}, {});
                size_type max_depth = *std::max_element(m_dep, m_dep + m_rooted_tree->vertex_cnt());
                m_level = std::bit_width(max_depth);
                for (size_type j = 1; j < m_level; j++) {
                    size_type *cur = m_fa + m_rooted_tree->vertex_cnt() * j, *prev = cur - m_rooted_tree->vertex_cnt();
                    for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) cur[i] = ~prev[i] ? prev[prev[i]] : -1;
                }
            }
            size_type get_ancestor(size_type a, size_type n) const {
                if (n > m_dep[a]) return -1;
                while (n) {
                    size_type k = std::countr_zero(n);
                    a = m_fa[m_rooted_tree->vertex_cnt() * k + a];
                    n -= 1 << k;
                }
                return a;
            }
            size_type find_parent(size_type a) const { return m_fa[a]; }
            size_type find_son(size_type a, size_type b) const { return get_ancestor(b, m_dep[b] - m_dep[a] - 1); }
            size_type get_depth(size_type a) const { return m_dep[a]; }
            size_type calc(size_type a, size_type b) {
                if (m_dep[a] > m_dep[b]) std::swap(a, b);
                size_type x = m_dep[b] - m_dep[a];
                while (x) {
                    size_type k = std::countr_zero(x);
                    b = m_fa[m_rooted_tree->vertex_cnt() * k + b], x -= 1 << k;
                }
                if (a == b) return a;
                size_type *cur = m_fa + m_rooted_tree->vertex_cnt() * m_level;
                for (size_type j = m_level - 1; ~j; j--) {
                    cur -= m_rooted_tree->vertex_cnt();
                    if (cur[a] != cur[b]) a = cur[a], b = cur[b];
                }
                return m_fa[a];
            }
        };
        template <typename Tree, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Table<Tree, MAX_VERTEX, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tree, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Table<Tree, MAX_VERTEX, MAX_NODE>::s_dep_buffer[MAX_VERTEX];
        template <typename Tree, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Table<Tree, MAX_VERTEX, MAX_NODE>::s_use_count;
        template <typename Tree, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Table<Tree, MAX_VERTEX, MAX_NODE>::s_dep_use_count;
    }
}

#endif