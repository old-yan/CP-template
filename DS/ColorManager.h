/*
最后修改:
20241215
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_COLORMANAGER__
#define __OY_COLORMANAGER__

#include "ErasableDSU.h"

namespace OY {
    namespace CM {
        using size_type = uint32_t;
        class Table {
            EDSU::Table m_dsu;
            std::vector<size_type> m_color, m_pos;
        public:
            Table(size_type color_count) { resize(color_count); }
            void resize(size_type color_count) {
                m_dsu.resize(color_count * 2);
                m_color.resize(color_count);
                m_pos.resize(color_count);
                for (size_type i = 0; i != color_count; i++) m_color[i] = i;
                for (size_type i = 0; i != color_count; i++) m_pos[i] = i;
            }
            void modify(size_type source, size_type target) {
                if (source == target) return;
                if (!~m_pos[source]) return;
                if (!~m_pos[target]) {
                    m_color[m_pos[source]] = target, m_pos[target] = m_pos[source], m_pos[source] = -1;
                    return;
                }
                m_dsu.unite_to(m_pos[source], m_pos[target]);
                m_color[m_pos[source]] = -1, m_pos[source] = -1;
            }
            size_type query(size_type init_color) const { return m_color[m_dsu.find(init_color)]; }
            size_type query_count(size_type color) const { return ~m_pos[color] ? m_dsu._size_of_head(m_pos[color]) : 0; }
        };
    }
}

#endif