/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FLOYDWARSHALL__
#define __OY_FLOYDWARSHALL__

#include <bitset>
#include <cstdint>
#include <vector>

namespace OY {
    namespace FloydWarshall {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX>
        struct Graph {
            std::vector<std::bitset<MAX_VERTEX>> m_can;
            size_type m_vertex_cnt;
            Graph(size_type vertex_cnt) : m_can(vertex_cnt), m_vertex_cnt(vertex_cnt) {}
            void add_edge(size_type a, size_type b) { m_can[a].set(b); }
            void calc() {
                for (size_type k = 0; k != m_vertex_cnt; k++)
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (m_can[i][k]) m_can[i] |= m_can[k];
            }
        };
    }
}

#endif