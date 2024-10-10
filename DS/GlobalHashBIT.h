/*
最后修改:
20241009
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
    namespace GBIT{
        using GHASH::size_type;
        template <typename Key, typename Mapped>
        struct GHashBITAdjacentNode {
            Mapped m_val[2];
            Mapped calc(Key i) const { return m_val[0] * i - m_val[1]; }
            GHashBITAdjacentNode<Key, Mapped> &operator+=(const GHashBITAdjacentNode<Key, Mapped> &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
                return *this;
            }
        };
        template <typename Key, typename Mapped, bool RangeUpdate, bool MakeRecord, size_type BUFFER>
        class Tree {
            using node = typename std::conditional<RangeUpdate, GHashBITAdjacentNode<Key, Mapped>, Mapped>::type;
            GHASH::UnorderedMap<Key, node, MakeRecord, BUFFER> m_map;
            static Key _lowbit(Key x) { return x & -x; }
            static Key _meet(Key a, Key b) { return ((a + 1) & -(Key(1) << std::bit_width<typename std::make_unsigned<Key>::type>((a + 1) ^ (b + 1)))) - 1; }
            Key m_length, m_size;
            void _add(Key i, const node &inc) {
                while (i < m_length) m_map.insert(i).m_ptr->m_mapped += inc, i += _lowbit(i + 1);
            }
        public:
            Tree() = default;
            Tree(Key length) { resize(length); }
            Key size() const { return m_size; }
            void resize(Key length) {
                if (!(m_size = length)) return;
                m_length = std::bit_ceil(length);
            }
            void add(Key i, Mapped inc) {
                if constexpr (RangeUpdate) {
                    _add(i, node{inc, Mapped(inc * i)});
                    _add(i + 1, node{-inc, Mapped(-inc * (i + 1))});
                } else
                    _add(i, inc);
            }
            void add(Key left, Key right, Mapped inc) {
                static_assert(RangeUpdate, "RangeUpdate Must Be True");
                _add(right + 1, node{-inc, Mapped(-inc * (right + 1))});
                _add(left, node{inc, Mapped(inc * left)});
            }
            Mapped presum(Key i) const {
                node res{};
                for (Key j = i; ~j; j -= _lowbit(j + 1)) res += m_map.get(j, {});
                if constexpr (RangeUpdate)
                    return res.calc(i + 1);
                else
                    return res;
            }
            Mapped query(Key i) const {
                if constexpr (RangeUpdate) {
                    Mapped res{};
                    for (Key j = i; ~j; j -= _lowbit(j + 1)) res += m_map.get(j, {}).m_val[0];
                    return res;
                } else {
                    Mapped res = m_map.get(i, {});
                    for (Key j = i - 1, k = _lowbit(i + 1); k >>= 1; j -= _lowbit(j + 1)) res -= m_map.get(j, {});
                    return res;
                }
            }
            Mapped query(Key left, Key right) const {
                if constexpr (RangeUpdate)
                    return presum(right) - presum(left - 1);
                else {
                    auto meet = _meet(left - 1, right);
                    Mapped res{};
                    for (Key j = right; j != meet; j -= _lowbit(j + 1)) res += m_map.get(j, 0);
                    for (Key j = left - 1; j != meet; j -= _lowbit(j + 1)) res -= m_map.get(j, 0);
                    return res;
                }
            }
            Mapped query_all() const { return presum(m_length - 1); }
            Key kth(Mapped k) const {
                if constexpr (RangeUpdate) {
                    Key cursor = -1;
                    node cur{};
                    for (Key d = m_length >> 1; d; d >>= 1) {
                        auto val = m_map.get(cursor + d, {});
                        if ((cur.m_val[0] + val.m_val[0]) * (cursor + d + 1) - (cur.m_val[1] + val.m_val[1]) <= k) cur += val, cursor += d;
                    }
                    return cursor + 1;
                } else {
                    Key cursor = -1;
                    for (Key d = m_length >> 1; d; d >>= 1) {
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
}

#endif