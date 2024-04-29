/*
最后修改:
20240429
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
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace DoubleLCA {
        using size_type = uint32_t;
        template <typename Tree, size_type MAX_LEVEL = 32>
        struct Table {
            Tree *m_rooted_tree;
            std::vector<size_type> m_fa[MAX_LEVEL], m_dep;
            size_type m_level;
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_fa[0].resize(m_rooted_tree->vertex_cnt()), m_dep.resize(m_rooted_tree->vertex_cnt());
                size_type *fa = m_fa[0].data();
                auto pre_work = [&](size_type a, size_type p) { fa[a] = p, m_dep[a] = ~p ? m_dep[p] + 1 : 0; };
                m_rooted_tree->tree_dp_vertex(m_rooted_tree->m_root, pre_work, {}, {});
                m_level = std::bit_width(*std::max_element(m_dep.begin(), m_dep.end()));
                for (size_type j = 1; j != m_level; j++) {
                    m_fa[j].resize(m_rooted_tree->vertex_cnt());
                    size_type *prev = m_fa[j - 1].data(), *cur = m_fa[j].data();
                    for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) cur[i] = ~prev[i] ? prev[prev[i]] : -1;
                }
            }
            size_type get_ancestor(size_type a, size_type n) const {
                if (n > m_dep[a]) return -1;
                while (n) {
                    size_type k = std::countr_zero(n);
                    a = m_fa[k][a], n -= 1 << k;
                }
                return a;
            }
            size_type find_parent(size_type a) const { return m_fa[0][a]; }
            size_type find_son(size_type a, size_type b) const { return get_ancestor(b, m_dep[b] - m_dep[a] - 1); }
            size_type get_depth(size_type a) const { return m_dep[a]; }
            size_type calc(size_type a, size_type b) const {
                const size_type *dep = m_dep.data();
                if (dep[a] > dep[b]) std::swap(a, b);
                size_type x = dep[b] - dep[a];
                while (x) {
                    size_type k = std::countr_zero(x);
                    b = m_fa[k][b], x -= 1 << k;
                }
                if (a == b) return a;
                for (size_type j = m_level - 1; ~j; j--) {
                    const size_type *cur = m_fa[j].data();
                    if (cur[a] != cur[b]) a = cur[a], b = cur[b];
                }
                return m_fa[0][a];
            }
        };
    }
}

#endif