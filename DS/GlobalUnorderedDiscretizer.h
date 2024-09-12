/*
最后修改:
20240510
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALUNORDEREDDISCRETIZER__
#define __OY_GLOBALUNORDEREDDISCRETIZER__

#include "GlobalHashMap.h"

namespace OY {
    template <typename Tp, GHASH::size_type BUFFER>
    struct GlobalUnorderedDiscretizer {
        using hash_table = GHASH::UnorderedMap<Tp, GHASH::size_type, true, BUFFER>;
        using node = typename hash_table::node;
        hash_table m_hashmap;
        GHASH::size_type insert(const Tp &item) {
            auto res = m_hashmap.insert(item);
            if (res.m_flag) res.m_ptr->m_mapped = m_hashmap.size() - 1;
            return res.m_ptr->m_mapped;
        }
        GHASH::size_type find(const Tp &item) const { return m_hashmap.find(item)->m_mapped; }
        template <bool Check = true>
        GHASH::size_type rank(const Tp &item) {
            if constexpr (Check)
                return insert(item);
            else
                return find(item);
        }
        const Tp &operator[](GHASH::size_type i) const { return m_hashmap.m_pool[m_hashmap.m_recs[i]].m_key; }
        GHASH::size_type size() const { return m_hashmap.size(); }
        void clear() { m_hashmap.clear(); }
        template <typename Callback>
        void do_for_each(Callback &&call) {
            auto ptr_call = [&](node *p) { call(p->m_key); };
            m_hashmap.do_for_each(ptr_call);
        }
    };
}

#endif