/*
最后修改:
20240613
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
        using size_type = uint32_t;
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_use_cnt++; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename SizeType, bool MaintainGroupCount>
        struct TableBase {
            SizeType m_length, m_group_cnt;
        };
        template <typename SizeType>
        struct TableBase<SizeType, false> {
            SizeType m_length;
        };
        template <typename SizeType = uint32_t, bool MaintainGroupCount = false, template <typename> typename BufferType = VectorBuffer>
        struct Table : TableBase<SizeType, MaintainGroupCount> {
            using TableBase<SizeType, MaintainGroupCount>::m_length;
            struct root_index {
                size_type m_index;
            };
            struct info {
                SizeType m_head, m_size;
            };
            union node {
                struct {
                    size_type m_lc, m_rc;
                };
                info m_info;
            };
            using buffer_type = BufferType<node>;
            root_index m_root;
            static size_type _newnode() { return buffer_type::newnode(); }
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static size_type _newnode(size_type lc, size_type rc) {
                size_type res = _newnode();
                _ptr(res)->m_lc = lc, _ptr(res)->m_rc = rc;
                return res;
            }
            static size_type _copynode(size_type rt) {
                size_type x = _newnode();
                *_ptr(x) = *_ptr(rt);
                return x;
            }
            static info _find(size_type cur, SizeType floor, SizeType ceil, SizeType x) {
                if (!cur) return {x, 1};
                node *p = _ptr(cur);
                if (floor == ceil) return p->m_info;
                SizeType mid = (floor + ceil) / 2;
                return x <= mid ? _find(p->m_lc, floor, mid, x) : _find(p->m_rc, mid + 1, ceil, x);
            }
            static size_type _modify(size_type cur, SizeType floor, SizeType ceil, SizeType x, const info &i) {
                size_type rt = cur ? _copynode(cur) : _newnode();
                if (floor == ceil)
                    _ptr(rt)->m_info = i;
                else {
                    SizeType mid = (floor + ceil) / 2;
                    if (x <= mid) {
                        size_type c = _modify(_ptr(rt)->m_lc, floor, mid, x, i);
                        _ptr(rt)->m_lc = c;
                    } else {
                        size_type c = _modify(_ptr(rt)->m_rc, mid + 1, ceil, x, i);
                        _ptr(rt)->m_rc = c;
                    }
                }
                return rt;
            }
            static size_type _modify(size_type cur, SizeType floor, SizeType ceil, SizeType x1, SizeType x2, const info &i1, const info &i2) {
                size_type rt = cur ? _copynode(cur) : _newnode();
                SizeType mid = (floor + ceil) / 2;
                if (x2 <= mid) {
                    size_type c = _modify(_ptr(rt)->m_lc, floor, mid, x1, x2, i1, i2);
                    _ptr(rt)->m_lc = c;
                    return rt;
                }
                if (x1 > mid) {
                    size_type c = _modify(_ptr(rt)->m_rc, mid + 1, ceil, x1, x2, i1, i2);
                    _ptr(rt)->m_rc = c;
                    return rt;
                }
                size_type lc = _modify(_ptr(rt)->m_lc, floor, mid, x1, i1), rc = _modify(_ptr(rt)->m_rc, mid + 1, ceil, x2, i2);
                _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = rc;
                return rt;
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
                    rt.m_index = _modify(rt.m_index, 0, range - 1, group_a.m_head, group_b.m_head, {group_b.m_head, 0}, {group_b.m_head, group_a.m_size + group_b.m_size});
                else
                    rt.m_index = _modify(rt.m_index, 0, range - 1, group_b.m_head, group_a.m_head, {group_b.m_head, group_a.m_size + group_b.m_size}, {group_b.m_head, 0});
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
            Table<SizeType, MaintainGroupCount, BufferType> copy() {
                Table<SizeType, MaintainGroupCount, BufferType> res;
                res.m_root = copy(m_root), res.m_length = m_length;
                if constexpr (MaintainGroupCount) res.m_group_cnt = this->m_group_cnt;
                return res;
            }
            void resize(SizeType length) {
                m_root.m_index = 0, m_length = length;
                if constexpr (MaintainGroupCount) this->m_group_cnt = length;
            }
            info find(SizeType i) const { return find(m_root, m_length, i); }
            SizeType size(SizeType i) { return size(m_root, m_length, i); }
            void unite_to(const info &group_a, const info &group_b) {
                unite_to(m_root, m_length, group_a, group_b);
                if (group_a.m_head != group_b.m_head)
                    if constexpr (MaintainGroupCount) this->m_group_cnt--;
            }
            bool unite_by_size(SizeType a, SizeType b) {
                if (unite_by_size(m_root, m_length, a, b)) {
                    if constexpr (MaintainGroupCount) this->m_group_cnt--;
                    return true;
                }
                return false;
            }
            bool in_same_group(SizeType a, SizeType b) { return in_same_group(m_root, m_length, a, b); }
            bool is_head(SizeType i) { return is_head(m_root, m_length, i); }
            SizeType count() const {
                static_assert(MaintainGroupCount, "MaintainGroupCount Must Be True");
                return this->m_group_cnt;
            }
            std::vector<SizeType> heads() {
                std::vector<SizeType> ret;
                if constexpr (MaintainGroupCount) ret.reserve(this->m_group_cnt);
                for (SizeType i = 0; i != m_length; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
        };
        template <typename Ostream, typename SizeType, bool MaintainGroupCount, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<SizeType, MaintainGroupCount, BufferType> &x) {
            out << '[';
            for (SizeType i = 0; i != x.m_length; i++) {
                if (i) out << ", ";
                if constexpr (MaintainGroupCount)
                    out << '(' << x.find(i).m_head << "," << x.find(i).m_size << ')';
                else
                    out << x.find(i).m_head;
            }
            return out << ']';
        }
    }
    template <typename SizeType = uint32_t, bool MaintainGroupCount = false, PerDSU::size_type MAX_BUFFER = 1 << 22>
    using StaticPerDSUTable = PerDSU::Table<SizeType, MaintainGroupCount, PerDSU::StaticBufferWrap<MAX_BUFFER>::template type>;
    template <typename SizeType = uint32_t, bool MaintainGroupCount = false>
    using VectorPerDSUTable = PerDSU::Table<SizeType, MaintainGroupCount, PerDSU::VectorBuffer>;
}

#endif