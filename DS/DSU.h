/*
最后修改:
20240331
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DISJOINTUNION__
#define __OY_DISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace DSU {
        using size_type = uint32_t;
        template <bool MaintainGroupSize>
        struct Table {
            mutable std::vector<size_type> m_parent, m_group_size;
            size_type m_size, m_group_cnt;
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size);
                if constexpr (MaintainGroupSize) m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
            }
            size_type find(size_type i) const { return m_parent[i] == i ? i : m_parent[i] = find(m_parent[i]); }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            void unite_to(size_type head_a, size_type head_b) {
                if (head_a == head_b) return;
                m_parent[head_a] = head_b;
                if constexpr (MaintainGroupSize) m_group_size[head_b] += m_group_size[head_a];
                m_group_cnt--;
            }
            bool unite_by_size(size_type a, size_type b) {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                a = find(a), b = find(b);
                if (a == b) return false;
                if (m_group_size[a] > m_group_size[b]) std::swap(a, b);
                unite_to(a, b);
                return true;
            }
            bool unite_by_ID(size_type a, size_type b) {
                a = find(a), b = find(b);
                if (a == b) return false;
                if (a < b) std::swap(a, b);
                unite_to(a, b);
                return true;
            }
            bool in_same_group(size_type a, size_type b) const { return find(a) == find(b); }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i != m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() const {
                if constexpr (MaintainGroupSize) {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                    return ret;
                } else {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size), cnt(m_group_cnt);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) cnt[index[find(i)]]++;
                    for (size_type i = 0; i != m_group_cnt; i++) ret[i].reserve(cnt[i]);
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                    return ret;
                }
            }
        };
        template <typename Ostream, bool MaintainGroupSize>
        Ostream &operator<<(Ostream &out, const Table<MaintainGroupSize> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.m_parent[i];
            }
            return out << "]";
        }
    }
    template <bool MaintainGroupSize = false>
    using DSUTable = DSU::Table<MaintainGroupSize>;
}

#endif