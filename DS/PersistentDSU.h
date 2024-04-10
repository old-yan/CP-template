/*
最后修改:
20240410
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTDISJOINTUNION__
#define __OY_PERSISTENTDISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace PerDSU {
        using index_type = uint32_t;
        template <typename SizeType, bool MaintainGroupSize>
        struct TableBase {
            SizeType m_length, m_group_cnt;
        };
        template <typename SizeType>
        struct TableBase<SizeType, false> {
            SizeType m_length;
        };
        template <typename SizeType = uint32_t, bool MaintainGroupSize = false, index_type MAX_NODE = 1 << 22>
        struct Table : TableBase<SizeType, MaintainGroupSize> {
            using TableBase<SizeType, MaintainGroupSize>::m_length;
            struct root_index {
                index_type m_index;
            };
            struct info {
                SizeType m_head, m_size;
            };
            union node {
                struct {
                    index_type m_lchild, m_rchild;
                };
                info m_info;
            };
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            root_index m_root;
            static index_type _newnode() { return s_use_count++; }
            static index_type _newnode(index_type lc, index_type rc) {
                index_type res = _newnode();
                s_buffer[res].m_children = {lc, rc};
                return res;
            }
            static index_type _copynode(index_type rt) {
                s_buffer[s_use_count] = s_buffer[rt];
                return s_use_count++;
            }
            static info _find(index_type rt, SizeType floor, SizeType ceil, SizeType x) {
                if (!rt) return {x, 1};
                if (floor == ceil) return s_buffer[rt].m_info;
                SizeType mid = (floor + ceil) / 2;
                return x <= mid ? _find(s_buffer[rt].m_lchild, floor, mid, x) : _find(s_buffer[rt].m_rchild, mid + 1, ceil, x);
            }
            static void _modify(index_type &rt, SizeType floor, SizeType ceil, SizeType x, const info &i) {
                if (!rt)
                    rt = _newnode();
                else
                    rt = _copynode(rt);
                if (floor == ceil) return void(s_buffer[rt].m_info = i);
                SizeType mid = (floor + ceil) / 2;
                return x <= mid ? _modify(s_buffer[rt].m_lchild, floor, mid, x, i) : _modify(s_buffer[rt].m_rchild, mid + 1, ceil, x, i);
            }
            static void _modify(index_type &rt, SizeType floor, SizeType ceil, SizeType x1, SizeType x2, const info &i1, const info &i2) {
                if (!rt)
                    rt = _newnode();
                else
                    rt = _copynode(rt);
                SizeType mid = (floor + ceil) / 2;
                if (x2 <= mid) return _modify(s_buffer[rt].m_lchild, floor, mid, x1, x2, i1, i2);
                if (x1 > mid) return _modify(s_buffer[rt].m_rchild, mid + 1, ceil, x1, x2, i1, i2);
                _modify(s_buffer[rt].m_lchild, floor, mid, x1, i1), _modify(s_buffer[rt].m_rchild, mid + 1, ceil, x2, i2);
            }
            static root_index copy(root_index rt) { return {rt.m_index ? _copynode(rt.m_index) : 0}; }
            static info find(root_index rt, SizeType range, SizeType x) {
                info res = _find(rt.m_index, 0, range - 1, x);
                while (res.m_head != x) x = res.m_head, res = _find(rt.m_index, 0, range - 1, x);
                return res;
            }
            static SizeType size(root_index rt, SizeType range, SizeType x) { return find(rt, range, x).m_size; }
            static void unite_to(root_index &rt, SizeType range, const info &group_a, const info &group_b) {
                if (group_a.m_head == group_b.m_head) return;
                if (group_a.m_head < group_b.m_head)
                    _modify(rt.m_index, 0, range - 1, group_a.m_head, group_b.m_head, {group_b.m_head, 0}, {group_b.m_head, group_a.m_size + group_b.m_size});
                else
                    _modify(rt.m_index, 0, range - 1, group_b.m_head, group_a.m_head, {group_b.m_head, group_a.m_size + group_b.m_size}, {group_b.m_head, 0});
            }
            static bool unite_by_size(root_index &rt, SizeType range, SizeType a, SizeType b) {
                if (a == b) return false;
                info group_a = find(rt, range, a), group_b = find(rt, range, b);
                if (group_a.m_head == group_b.m_head) return false;
                if (group_a.m_size > group_b.m_size) std::swap(group_a, group_b);
                unite_to(rt, range, group_a, group_b);
                return true;
            }
            static bool in_same_group(root_index rt, SizeType range, SizeType a, SizeType b) { return a == b || find(rt, range, a).m_head == find(rt, range, b).m_head; }
            static bool is_head(root_index rt, SizeType range, SizeType i) { return find(rt, range, i).m_head == i; }
            Table() = default;
            Table(SizeType length) { resize(length); }
            Table<SizeType, MaintainGroupSize, MAX_NODE> copy() {
                Table<SizeType, MaintainGroupSize, MAX_NODE> res;
                res.m_root = copy(m_root), res.m_length = m_length;
                if constexpr (MaintainGroupSize) res.m_group_cnt = this->m_group_cnt;
                return res;
            }
            void resize(SizeType length) {
                m_root.m_index = 0, m_length = length;
                if constexpr (MaintainGroupSize) this->m_group_cnt = length;
            }
            info find(SizeType i) const { return find(m_root, m_length, i); }
            SizeType size(SizeType i) { return size(m_root, m_length, i); }
            void unite_to(const info &group_a, const info &group_b) {
                unite_to(m_root, m_length, group_a, group_b);
                if (group_a.m_head != group_b.m_head)
                    if constexpr (MaintainGroupSize) this->m_group_cnt--;
            }
            bool unite_by_size(SizeType a, SizeType b) {
                if (unite_by_size(m_root, m_length, a, b)) {
                    if constexpr (MaintainGroupSize) this->m_group_cnt--;
                    return true;
                }
                return false;
            }
            bool in_same_group(SizeType a, SizeType b) { return in_same_group(m_root, m_length, a, b); }
            bool is_head(SizeType i) { return is_head(m_root, m_length, i); }
            SizeType count() const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                return this->m_group_cnt;
            }
            std::vector<SizeType> heads() {
                std::vector<SizeType> ret;
                if constexpr (MaintainGroupSize) ret.reserve(this->m_group_cnt);
                for (SizeType i = 0; i < m_length; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
        };
        template <typename SizeType, bool MaintainGroupSize, index_type MAX_NODE>
        typename Table<SizeType, MaintainGroupSize, MAX_NODE>::node Table<SizeType, MaintainGroupSize, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename SizeType, bool MaintainGroupSize, index_type MAX_NODE>
        index_type Table<SizeType, MaintainGroupSize, MAX_NODE>::s_use_count = 1;
        template <typename Ostream, typename SizeType, bool MaintainGroupSize, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<SizeType, MaintainGroupSize, MAX_NODE> &x) {
            out << '[';
            for (SizeType i = 0; i != x.m_length; i++) {
                if (i) out << ", ";
                if constexpr (MaintainGroupSize)
                    out << '(' << x.find(i).m_head << "," << x.find(i).m_size << ')';
                else
                    out << x.find(i).m_head;
            }
            return out << ']';
        }
    }
    template <typename SizeType = uint32_t, bool MaintainGroupSize = false, PerDSU::index_type MAX_NODE = 1 << 22>
    using PerDSUTable = PerDSU::Table<SizeType, MaintainGroupSize, MAX_NODE>;
}

#endif