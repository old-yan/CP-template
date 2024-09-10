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

#include <numeric>

#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace PerDSU {
        using size_type = uint32_t;
        template <typename SizeType, bool MaintainGroupCount>
        struct TableBase {
            SizeType m_size, m_group_cnt;
        };
        template <typename SizeType>
        struct TableBase<SizeType, false> {
            SizeType m_size;
        };
        template <typename SizeType = uint32_t, bool MaintainGroupCount = false, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Table : TableBase<SizeType, MaintainGroupCount> {
        public:
            using TableBase<SizeType, MaintainGroupCount>::m_size;
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
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            root_index m_root;
            static size_type _newnode() { return buffer_type::newnode(); }
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
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
            static root_index _copy(root_index rt) { return {rt.m_index ? _copynode(rt.m_index) : 0}; }
        public:
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
            SizeType size() const { return m_size; }
            Table<SizeType, MaintainGroupCount, BufferType> copy() {
                Table<SizeType, MaintainGroupCount, BufferType> res;
                res.m_root = _copy(m_root), res.m_size = m_size;
                if constexpr (MaintainGroupCount) res.m_group_cnt = this->m_group_cnt;
                return res;
            }
            void resize(SizeType length) {
                m_root.m_index = 0, m_size = length;
                if constexpr (MaintainGroupCount) this->m_group_cnt = length;
            }
            info find(SizeType i) const { return find(m_root, m_size, i); }
            SizeType size(SizeType i) { return size(m_root, m_size, i); }
            void unite_to(const info &group_a, const info &group_b) {
                unite_to(m_root, m_size, group_a, group_b);
                if (group_a.m_head != group_b.m_head)
                    if constexpr (MaintainGroupCount) this->m_group_cnt--;
            }
            bool unite_by_size(SizeType a, SizeType b) {
                if (unite_by_size(m_root, m_size, a, b)) {
                    if constexpr (MaintainGroupCount) this->m_group_cnt--;
                    return true;
                }
                return false;
            }
            bool in_same_group(SizeType a, SizeType b) { return in_same_group(m_root, m_size, a, b); }
            bool is_head(SizeType i) { return is_head(m_root, m_size, i); }
            SizeType count() const {
                static_assert(MaintainGroupCount, "MaintainGroupCount Must Be True");
                return this->m_group_cnt;
            }
            std::vector<SizeType> heads() {
                std::vector<SizeType> ret;
                if constexpr (MaintainGroupCount) ret.reserve(this->m_group_cnt);
                for (SizeType i = 0; i != m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
        };
        template <typename Ostream, typename SizeType, bool MaintainGroupCount, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<SizeType, MaintainGroupCount, BufferType> &x) {
            out << '[';
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                if constexpr (MaintainGroupCount)
                    out << '(' << x.find(i).m_head << "," << x.find(i).m_size << ')';
                else
                    out << x.find(i).m_head;
            }
            return out << ']';
        }
    }
    template <typename SizeType = uint32_t, bool MaintainGroupCount = false>
    using VectorPerDSUTable = PerDSU::Table<SizeType, MaintainGroupCount>;
}

#endif