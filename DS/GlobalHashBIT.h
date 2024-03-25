/*
最后修改:
20240325
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALHASHBIT__
#define __OY_GLOBALHASHBIT__

#include "GlobalHashMap.h"

namespace OY {
    template <typename KeyType, typename MappedType, uint32_t L, bool MakeRecord>
    struct GHashBIT {
        GHASH::UnorderedMap<KeyType, MappedType, L, MakeRecord> m_map;
        static KeyType _lowbit(KeyType x) { return x & -x; }
        static KeyType _meet(KeyType a, KeyType b) { return ((a + 1) & -(KeyType(1) << std::bit_width<typename std::make_unsigned<KeyType>::type>((a + 1) ^ (b + 1)))) - 1; }
        KeyType m_length;
        GHashBIT(KeyType length) {
            for (m_length = 1; m_length < length; m_length <<= 1) {}
        }
        void add(KeyType i, MappedType inc) {
            while (i < m_length) m_map.insert(i).m_ptr->m_mapped += inc, i += _lowbit(i + 1);
        }
        MappedType presum(KeyType i) const {
            MappedType res{};
            for (KeyType j = i; ~j; j -= _lowbit(j + 1)) res += m_map.get(j, 0);
            return res;
        }
        MappedType query(KeyType i) const { return query(i, i); }
        MappedType query(KeyType left, KeyType right) const {
            auto meet = _meet(left - 1, right);
            MappedType res{};
            for (KeyType j = right; j != meet; j -= _lowbit(j + 1)) res += m_map.get(j, 0);
            for (KeyType j = left - 1; j != meet; j -= _lowbit(j + 1)) res -= m_map.get(j, 0);
            return res;
        }
        MappedType query_all() const { return presum(m_length - 1); }
        KeyType kth(MappedType k) const {
            KeyType cursor = -1;
            for (KeyType d = m_length >> 1; d; d >>= 1) {
                auto val = m_map.get(cursor + d, 0);
                if (val <= k) k -= val, cursor += d;
            }
            return cursor + 1;
        }
        void clear() {
            static_assert(MakeRecord, "MakeRecord Must Be True");
            m_map.do_for_each([&](decltype(m_map)::node *p) { p->m_mapped = 0; }), m_map.clear();
        }
    };
}

#endif