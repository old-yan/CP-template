/*
最后修改:
20241009
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
    namespace GBIT2D{
        using GHASH::size_type;
        template <typename Key, typename Mapped>
        struct GHashBIT2DAdjacentNode {
            Mapped m_val[4];
            Mapped calc(Key i, Key j) const { return m_val[0] * (i + 1) * (j + 1) - m_val[1] * (j + 1) - m_val[2] * (i + 1) + m_val[3]; }
            GHashBIT2DAdjacentNode<Key, Mapped> &operator+=(const GHashBIT2DAdjacentNode<Key, Mapped> &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                return *this;
            }
        };
        template <typename Key>
        struct Coordinate {
            Key m_i, m_j;
            bool operator==(const Coordinate<Key> &rhs) const { return m_i == rhs.m_i && m_j == rhs.m_j; }
        };
        template <typename Key, typename Mapped, bool RangeUpdate, bool MakeRecord, size_type BUFFER>
        class Tree {
            using node = typename std::conditional<RangeUpdate, GHashBIT2DAdjacentNode<Key, Mapped>, Mapped>::type;
            GHASH::UnorderedMap<Coordinate<Key>, node, MakeRecord, BUFFER> m_map;
            static Key _meet(Key a, Key b) { return ((a + 1) & -(Key(1) << std::bit_width<typename std::make_unsigned<Key>::type>((a + 1) ^ (b + 1)))) - 1; }
            static Key _meet2(Key a, Key b) { return (a | (std::bit_ceil<typename std::make_unsigned<Key>::type>((a ^ b) + 1) - 1)); }
            Key m_row, m_column;
            void _add(Key i, Key j, node inc) {
                for (Key r = i; r < m_row; r += _lowbit(r + 1))
                    for (Key c = j; c < m_column; c += _lowbit(c + 1)) m_map.insert({r, c}).m_ptr->m_mapped += inc;
            }
        public:
            static Key _lowbit(Key x) { return x & -x; }
            static void _special_add(Tree<Key, Mapped, RangeUpdate, MakeRecord, BUFFER> &x, Key r1, Key r2, Key c1, Key c2, Mapped inc) {
                static_assert(!RangeUpdate, "RangeUpadte Must Be False");
                Key rend = _meet2(r1, r2 + 1), cend = _meet2(c1, c2 + 1);
                for (Key r = r1; r != rend && r < x.row(); r += _lowbit(r + 1)) {
                    for (Key c = c1; c != cend && c < x.column(); c += _lowbit(c + 1)) x.m_map.insert({r, c}).m_ptr->m_mapped += inc;
                    for (Key c = c2 + 1; c != cend && c < x.column(); c += _lowbit(c + 1)) x.m_map.insert({r, c}).m_ptr->m_mapped -= inc;
                }
                for (Key r = r2 + 1; r != rend && r < x.row(); r += _lowbit(r + 1)) {
                    for (Key c = c1; c != cend && c < x.column(); c += _lowbit(c + 1)) x.m_map.insert({r, c}).m_ptr->m_mapped -= inc;
                    for (Key c = c2 + 1; c != cend && c < x.column(); c += _lowbit(c + 1)) x.m_map.insert({r, c}).m_ptr->m_mapped += inc;
                }
            }
            Tree() = default;
            Tree(Key row, Key column) { resize(row, column); }
            void resize(Key row, Key column) { m_row = row, m_column = column; }
            Key row() const { return m_row; }
            Key column() const { return m_column; }
            void add(Key i, Key j, Mapped inc) {
                if constexpr (RangeUpdate) {
                    _add(i, j, node{{inc, Mapped(inc * i), Mapped(inc * j), Mapped(inc * i * j)}});
                    _add(i, j + 1, node{{-inc, -Mapped(inc * i), -Mapped(inc * (j + 1)), -Mapped(inc * i * (j + 1))}});
                    _add(i + 1, j, node{{-inc, -Mapped(inc * (i + 1)), -Mapped(inc * j), -Mapped(inc * (i + 1) * j)}});
                    _add(i + 1, j + 1, node{{inc, Mapped(inc * (i + 1)), Mapped(inc * (j + 1)), Mapped(inc * (i + 1) * (j + 1))}});
                } else
                    _add(i, j, inc);
            }
            void add(Key row1, Key row2, Key column1, Key column2, Mapped inc) {
                static_assert(RangeUpdate, "RangeUpdate Must Be True");
                _add(row1, column1, node{{inc, Mapped(inc * row1), Mapped(inc * column1), Mapped(inc * row1 * column1)}});
                _add(row1, column2 + 1, node{{-inc, -Mapped(inc * row1), -Mapped(inc * (column2 + 1)), -Mapped(inc * row1 * (column2 + 1))}});
                _add(row2 + 1, column1, node{{-inc, -Mapped(inc * (row2 + 1)), -Mapped(inc * column1), -Mapped(inc * (row2 + 1) * column1)}});
                _add(row2 + 1, column2 + 1, node{{inc, Mapped(inc * (row2 + 1)), Mapped(inc * (column2 + 1)), Mapped(inc * (row2 + 1) * (column2 + 1))}});
            }
            Mapped presum(Key i, Key j) const {
                node ret{};
                for (Key r = i; ~r; r -= _lowbit(r + 1))
                    for (Key c = j; ~c; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                if constexpr (RangeUpdate)
                    return ret.calc(i, j);
                else
                    return ret;
            }
            Mapped query(Key i, Key j) const {
                if constexpr (RangeUpdate) {
                    Mapped ret{};
                    for (Key r = i; ~r; r -= _lowbit(r + 1))
                        for (Key c = j; ~c; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {}).m_val[0];
                    return ret;
                } else {
                    node ret{};
                    const Key rend = i - _lowbit(i + 1), cend = j - _lowbit(j + 1);
                    for (Key r = i; r != rend; r -= _lowbit(r + 1)) {
                        for (Key c = j; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                        for (Key c = j - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                    }
                    for (Key r = i - 1; r != rend; r -= _lowbit(r + 1)) {
                        for (Key c = j; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                        for (Key c = j - 1; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                    }
                    return ret;
                }
            }
            Mapped query(Key row1, Key row2, Key column1, Key column2) const {
                if constexpr (RangeUpdate)
                    return presum(row2, column2) + presum(row1 - 1, column1 - 1) - presum(row2, column1 - 1) - presum(row1 - 1, column2);
                else {
                    node ret{};
                    const Key rend = _meet(row1 - 1, row2), cend = _meet(column1 - 1, column2);
                    for (Key r = row2; r != rend; r -= _lowbit(r + 1)) {
                        for (Key c = column2; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                        for (Key c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                    }
                    for (Key r = row1 - 1; r != rend; r -= _lowbit(r + 1)) {
                        for (Key c = column2; c != cend; c -= _lowbit(c + 1)) ret -= m_map.get({r, c}, {});
                        for (Key c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret += m_map.get({r, c}, {});
                    }
                    return ret;
                }
            }
            Mapped query_all() const { return presum(m_row - 1, m_column - 1); }
            void clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                m_map.do_for_each([&](typename decltype(m_map)::node *p) { p->m_mapped = {}; }), m_map.clear();
            }
        };
    }
    namespace GHASH {
        template <>
        struct Hash<GBIT2D::Coordinate<uint32_t>> {
            uint64_t operator()(const GBIT2D::Coordinate<uint32_t> &x) const { return Hash<uint64_t>()(*(uint64_t *)&x); }
        };
        template <>
        struct Hash<GBIT2D::Coordinate<uint64_t>> {
            uint64_t operator()(const GBIT2D::Coordinate<uint64_t> &x) const { return HashCombine()(Hash<uint64_t>()(x.m_i), Hash<uint64_t>()(x.m_j)); }
        };
    }
}

#endif