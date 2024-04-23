/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RMQLCA__
#define __OY_RMQLCA__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace RMQLCA {
        using size_type = uint32_t;
        template <typename Tree, typename RMQ, size_type MAX_VERTEX>
        struct Table {
            static size_type s_buffer[MAX_VERTEX * 3], s_use_count;
            Tree *m_rooted_tree;
            size_type *m_dfn, *m_seq, *m_fa;
            RMQ m_rmq;
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_dfn = s_buffer + s_use_count * 3, m_seq = m_dfn + m_rooted_tree->vertex_cnt(), m_fa = m_seq + m_rooted_tree->vertex_cnt(), s_use_count += m_rooted_tree->vertex_cnt();
                size_type cursor = 0;
                auto prework = [&](size_type a, size_type p) {
                    m_dfn[a] = cursor, m_seq[cursor] = a, m_fa[cursor] = ~p ? m_dfn[p] : p;
                    cursor++;
                };
                m_rooted_tree->tree_dp_vertex(m_rooted_tree->m_root, prework, {}, {});
                m_rmq.reset(m_fa, m_fa + m_rooted_tree->vertex_cnt());
            }
            size_type calc(size_type a, size_type b) const {
                if (a == b) return a;
                if (m_dfn[a] > m_dfn[b]) std::swap(a, b);
                return m_seq[m_rmq.query(m_dfn[a] + 1, m_dfn[b])];
            }
        };
        template <typename Tree, typename RMQ, size_type MAX_VERTEX>
        size_type Table<Tree, RMQ, MAX_VERTEX>::s_buffer[MAX_VERTEX * 3];
        template <typename Tree, typename RMQ, size_type MAX_VERTEX>
        size_type Table<Tree, RMQ, MAX_VERTEX>::s_use_count;
    }
}

#endif