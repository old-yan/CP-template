/*
最后修改:
20231104
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PRUFER__
#define __OY_PRUFER__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace PRUFER {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX>
        struct Tree {
            struct node {
                size_type m_xor, m_deg;
            };
            static node s_buffer[MAX_VERTEX];
            static size_type s_use_count;
            size_type m_vertex_cnt;
            node *m_info;
            template <typename Iterator>
            static std::vector<size_type> decode(Iterator first, Iterator last) {
                size_type n = last - first + 2;
                std::vector<size_type> res(n), deg(n);
                for (auto it = first; it != last; ++it) deg[*it]++;
                auto it = first;
                for (size_type i = 0; i != n; i++)
                    if (!deg[i]) {
                        size_type cur = i;
                        do {
                            if (it == last)
                                res[cur] = n - 1;
                            else
                                res[cur] = *it++;
                            cur = res[cur];
                        } while (!--deg[cur] && cur <= i);
                    }
                res[n - 1] = -1;
                return res;
            }
            Tree(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_info = s_buffer + s_use_count, s_use_count += m_vertex_cnt;
            }
            void add_edge(size_type a, size_type b) { m_info[a].m_deg++, m_info[b].m_deg++, m_info[a].m_xor ^= b, m_info[b].m_xor ^= a; }
            std::vector<size_type> encode() {
                std::vector<size_type> res;
                res.reserve(m_vertex_cnt - 1);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_deg == 1) {
                        size_type cur = i;
                        do m_info[m_info[cur].m_xor].m_xor ^= cur, res.push_back(cur = m_info[cur].m_xor);
                        while (--m_info[cur].m_deg == 1 && cur <= i);
                    }
                res.pop_back();
                return res;
            }
        };
        template <size_type MAX_VERTEX>
        typename Tree<MAX_VERTEX>::node Tree<MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX>
        size_type Tree<MAX_VERTEX>::s_use_count;
    }
}

#endif