/*
最后修改:
20240429
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
#include <vector>

namespace OY {
    namespace RMQLCA {
        using size_type = uint32_t;
        template <typename Tree, typename RMQ>
        struct Table {
            Tree *m_rooted_tree;
            std::vector<size_type> m_dfn, m_seq;
            RMQ m_rmq;
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_dfn.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt());
                std::vector<size_type> fa(m_rooted_tree->vertex_cnt());
                auto prework = [&, cursor = 0](size_type a, size_type p) mutable {
                    m_dfn[a] = cursor, m_seq[cursor] = a, fa[cursor] = ~p ? m_dfn[p] : p;
                    cursor++;
                };
                m_rooted_tree->tree_dp_vertex(m_rooted_tree->m_root, prework, {}, {});
                m_rmq.reset(fa.begin(), fa.end());
            }
            size_type calc(size_type a, size_type b) const {
                if (a == b) return a;
                if (m_dfn[a] > m_dfn[b]) std::swap(a, b);
                return m_seq[m_rmq.query(m_dfn[a] + 1, m_dfn[b])];
            }
        };
    }
}

#endif