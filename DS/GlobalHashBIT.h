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

#include "../TEST/std_bit.h"

namespace OY {
    template <typename KeyType, typename MappedType>
    struct GHashBITAdjacentNode {
        MappedType m_val[2];
        MappedType calc(KeyType i) const { return m_val[0] * i - m_val[1]; }
        GHashBITAdjacentNode<KeyType, MappedType> &operator+=(const GHashBITAdjacentNode<KeyType, MappedType> &rhs) {
            m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
            return *this;
        }
    };
    template <typename KeyType, typename MappedType, bool RangeUpdate, bool MakeRecord, uint32_t L>
    struct GHashBIT {
        using node = typename std::conditional<RangeUpdate, GHashBITAdjacentNode<KeyType, MappedType>, MappedType>::type;
        GHASH::UnorderedMap<KeyType, node, MakeRecord, L> m_map;
        static KeyType _lowbit(KeyType x) { return x & -x; }
        static KeyType _meet(KeyType a, KeyType b) { return ((a + 1) & -(KeyType(1) << std::bit_width<typename std::make_unsigned<KeyType>::type>((a + 1) ^ (b + 1)))) - 1; }
        KeyType m_length;
        void _add(KeyType i, const node &inc) {
            while (i < m_length) m_map.insert(i).m_ptr->m_mapped += inc, i += _lowbit(i + 1);
        }
        GHashBIT() = default;
        GHashBIT(KeyType length) { resize(length); }
        void resize(KeyType length) {
            for (m_length = 1; m_length < length; m_length <<= 1) {}
        }
        void add(KeyType i, MappedType inc) {
            if constexpr (RangeUpdate) {
                _add(i, node{inc, MappedType(inc * i)});
                _add(i + 1, node{-inc, MappedType(-inc * (i + 1))});
            } else
                _add(i, inc);
        }
        void add(KeyType left, KeyType right, MappedType inc) {
            static_assert(RangeUpdate, "RangeUpdate Must Be True");
            _add(right + 1, node{-inc, MappedType(-inc * (right + 1))});
            _add(left, node{inc, MappedType(inc * left)});
        }
        MappedType presum(KeyType i) const {
            node res{};
            for (KeyType j = i; ~j; j -= _lowbit(j + 1)) res += m_map.get(j, {});
            if constexpr (RangeUpdate)
                return res.m_val[0] * (i + 1) - res.m_val[1];
            else
                return res;
        }
        MappedType query(KeyType i) const {
            if constexpr (RangeUpdate) {
                MappedType res{};
                for (KeyType j = i; ~j; j -= _lowbit(j + 1)) res += m_map.get(j, {}).m_val[0];
                return res;
            } else {
                MappedType res = m_map.get(i, {});
                for (KeyType j = i - 1, k = _lowbit(i + 1); k >>= 1; j -= _lowbit(j + 1)) res -= m_map.get(j, {});
                return res;
            }
        }
        MappedType query(KeyType left, KeyType right) const {
            if constexpr (RangeUpdate)
                return presum(right) - presum(left - 1);
            else {
                auto meet = _meet(left - 1, right);
                MappedType res{};
                for (KeyType j = right; j != meet; j -= _lowbit(j + 1)) res += m_map.get(j, 0);
                for (KeyType j = left - 1; j != meet; j -= _lowbit(j + 1)) res -= m_map.get(j, 0);
                return res;
            }
        }
        MappedType query_all() const { return presum(m_length - 1); }
        KeyType kth(MappedType k) const {
            if constexpr (RangeUpdate) {
                KeyType cursor = -1;
                node cur{};
                for (KeyType d = m_length >> 1; d; d >>= 1) {
                    auto val = m_map.get(cursor + d, {});
                    if ((cur.m_val[0] + val.m_val[0]) * (cursor + d + 1) - (cur.m_val[1] + val.m_val[1]) <= k) cur += val, cursor += d;
                }
                return cursor + 1;
            } else {
                KeyType cursor = -1;
                for (KeyType d = m_length >> 1; d; d >>= 1) {
                    auto val = m_map.get(cursor + d, {});
                    if (val <= k) k -= val, cursor += d;
                }
                return cursor + 1;
            }
        }
        void clear() {
            static_assert(MakeRecord, "MakeRecord Must Be True");
            m_map.do_for_each([&](typename decltype(m_map)::node *p) { p->m_mapped = {}; }), m_map.clear();
        }
    };
}

#endif