/*
最后修改:
20240503
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALRANGECOUNTER__
#define __OY_GLOBALRANGECOUNTER__

#include "GlobalHashMap.h"

namespace OY {
    template <typename Tp, GHASH::size_type BUFFER>
    struct GlobalRangeCounter {
        using size_type = GHASH::size_type;
        struct Counter {
            size_type m_cnt, *m_indexes;
        };
        using hash_table = GHASH::UnorderedMap<Tp, Counter, true, BUFFER>;
        using node = typename hash_table::node;
        GHASH::UnorderedMap<Tp, Counter, true, BUFFER> m_map;
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            std::vector<Tp> items(length);
            for (size_type i = 0; i != length; i++) items[i] = mapping(i);
            reset(items.begin(), items.end());
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            for (auto it = first; it != last; ++it) m_map.insert(*it).m_ptr->m_mapped.m_cnt++;
            m_map.do_for_each([](node *p) { p->m_mapped.m_indexes = (size_type *)malloc(p->m_mapped.m_cnt * sizeof(size_type)), p->m_mapped.m_cnt = 0; });
            size_type index = 0;
            for (auto it = first; it != last; ++it) {
                auto ptr = m_map.find(*it);
                ptr->m_mapped.m_indexes[ptr->m_mapped.m_cnt++] = index++;
            }
        }
        size_type query(size_type left, size_type right, const Tp &x) const {
            auto it = m_map.find(x);
            if (!it) return 0;
            auto pos1 = std::lower_bound(it->m_mapped.m_indexes, it->m_mapped.m_indexes + it->m_mapped.m_cnt, left);
            auto pos2 = std::lower_bound(it->m_mapped.m_indexes, it->m_mapped.m_indexes + it->m_mapped.m_cnt, right + 1);
            return pos2 - pos1;
        }
        void clear() {
            m_map.do_for_each([](node *p) { free(p->m_mapped.m_indexes), p->m_mapped.m_cnt = 0; });
        }
    };
}

#endif