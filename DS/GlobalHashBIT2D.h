/*
最后修改:
20240409
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALHASHBIT2D__
#define __OY_GLOBALHASHBIT2D__

#include "GlobalHashMap.h"

#include "../TEST/std_bit.h"

namespace OY {
    template <typename KeyType, typename MappedType>
    struct GHashBIT2DAdjacentNode {
        MappedType m_val[4];
        MappedType calc(KeyType i, KeyType j) const { return m_val[0] * (i + 1) * (j + 1) - m_val[1] * (j + 1) - m_val[2] * (i + 1) + m_val[3]; }
        GHashBIT2DAdjacentNode<KeyType, MappedType> &operator+=(const GHashBIT2DAdjacentNode<KeyType, MappedType> &rhs) {
            m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
            return *this;
        }
    };
    template <typename KeyType>
    struct Coordinate {
        KeyType m_i, m_j;
        bool operator==(const Coordinate<KeyType> &rhs) const { return m_i == rhs.m_i && m_j == rhs.m_j; }
    };
    namespace GHASH {
        template <size_type L>
        struct Hash<Coordinate<uint32_t>, L> {
            size_type operator()(const Coordinate<uint32_t> &x) const { return Hash<uint64_t, L>()(*(uint64_t *)&x); }
        };
        template <size_type L>
        struct Hash<Coordinate<uint64_t>, L> {
            size_type operator()(const Coordinate<uint64_t> &x) const { return Hash<uint64_t, L>()(x.m_i, x.m_j); }
        };
    }
    template <typename KeyType, typename MappedType, bool RangeUpdate, bool MakeRecord, uint32_t L>
    struct GHashBIT2D {
        using node = typename std::conditional<RangeUpdate, GHashBIT2DAdjacentNode<KeyType, MappedType>, MappedType>::type;
        GHASH::UnorderedMap<Coordinate<KeyType>, node, MakeRecord, L> m_map;
        static KeyType _lowbit(KeyType x) { return x & -x; }
        static KeyType _meet(KeyType a, KeyType b) { return ((a + 1) & -(KeyType(1) << std::bit_width<typename std::make_unsigned<KeyType>::type>((a + 1) ^ (b + 1)))) - 1; }
        KeyType m_row, m_column;
        void _add(KeyType i, KeyType j, const node &inc) {
            for (KeyType r = i; r < m_row; r += _lowbit(r + 1))
                for (KeyType c = j; c < m_column; c += _lowbit(c + 1)) m_map.insert({r, c}).m_ptr->m_mapped += inc;
        }
        GHashBIT2D() = default;
        GHashBIT2D(KeyType row, KeyType column) { resize(row, column); }
        void resize(KeyType row, KeyType column) { m_row = row, m_column = column; }
        void add(KeyType i, KeyType j, MappedType inc) {
            if constexpr (RangeUpdate) {
                _add(i, j, node{{inc, inc * i, inc * j, inc * i * j}});
                _add(i, j + 1, node{{-inc, -inc * i, -inc * (j + 1), -inc * i * (j + 1)}});
                _add(i + 1, j, node{{-inc, -inc * (i + 1), -inc * j, -inc * (i + 1) * j}});
                _add(i + 1, j + 1, node{{inc, inc * (i + 1), inc * (j + 1), inc * (i + 1) * (j + 1)}});
            } else
                _add(i, j, inc);
        }
        void add(KeyType row1, KeyType row2, KeyType column1, KeyType column2, MappedType inc) {
            static_assert(RangeUpdate, "RangeUpdate Must Be True");
            _add(row1, column1, node{{inc, inc * row1, inc * column1, inc * row1 * column1}});
            _add(row1, column2 + 1, node{{-inc, -inc * row1, -inc * (column2 + 1), -inc * row1 * (column2 + 1)}});
            _add(row2 + 1, column1, node{{-inc, -inc * (row2 + 1), -inc * column1, -inc * (row2 + 1) * column1}});
            _add(row2 + 1, column2 + 1, node{{inc, inc * (row2 + 1), inc * (column2 + 1), inc * (row2 + 1) * (column2 + 1)}});
        }
        MappedType presum(KeyType i, KeyType j) const {
            node ret{};
            for (KeyType r = i; ~r; r -= _lowbit(r + 1))
                for (KeyType c = j; ~c; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
            if constexpr (RangeUpdate)
                return ret.calc(i, j);
            else
                return ret;
        }
        MappedType query(KeyType i, KeyType j) const {
            if constexpr (RangeUpdate) {
                MappedType ret{};
                for (KeyType r = i; ~r; r -= _lowbit(r + 1))
                    for (KeyType c = j; ~c; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {}).m_val[0];
                return ret;
            } else {
                node ret{};
                const KeyType rend = i - _lowbit(i + 1), cend = j - _lowbit(j + 1);
                for (KeyType r = i; r != rend; r -= _lowbit(r + 1)) {
                    for (KeyType c = j; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                    for (KeyType c = j - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                }
                for (KeyType r = i - 1; r != rend; r -= _lowbit(r + 1)) {
                    for (KeyType c = j; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                    for (KeyType c = j - 1; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                }
                return ret;
            }
        }
        MappedType query(KeyType row1, KeyType row2, KeyType column1, KeyType column2) const {
            if constexpr (RangeUpdate)
                return presum(row2, column2) + presum(row1 - 1, column1 - 1) - presum(row2, column1 - 1) - presum(row1 - 1, column2);
            else {
                node ret{};
                const KeyType rend = _meet(row1 - 1, row2), cend = _meet(column1 - 1, column2);
                for (KeyType r = row2; r != rend; r -= _lowbit(r + 1)) {
                    for (KeyType c = column2; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                    for (KeyType c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                }
                for (KeyType r = row1 - 1; r != rend; r -= _lowbit(r + 1)) {
                    for (KeyType c = column2; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                    for (KeyType c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                }
                return ret;
            }
        }
        MappedType query_all() const { return presum(m_row - 1, m_column - 1); }
        void clear() {
            static_assert(MakeRecord, "MakeRecord Must Be True");
            m_map.do_for_each([&](typename decltype(m_map)::node *p) { p->m_mapped = {}; }), m_map.clear();
        }
    };
}

#endif