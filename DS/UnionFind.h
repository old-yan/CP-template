/*
最后修改:
20230825
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_UNIONFIND__
#define __OY_UNIONFIND__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace UF {
        using size_type = uint32_t;
        template <size_type MAX_NODE = 1 << 22>
        struct Table {
            static size_type s_buffer[MAX_NODE], s_size_buffer[MAX_NODE], s_use_count;
            size_type *m_parent, *m_group_size, m_size, m_group_count;
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_count = n)) return;
                m_parent = s_buffer + s_use_count, m_group_size = s_size_buffer + s_use_count;
                s_use_count += m_size;
                for (size_type i = 0; i < m_size; i++) m_parent[i] = i;
                for (size_type i = 0; i < m_size; i++) m_group_size[i] = 1;
            }
            size_type find(size_type i) { return m_parent[i] == i ? i : m_parent[i] = find(m_parent[i]); }
            size_type size(size_type i) { return m_group_size[find(i)]; }
            void unite_to(size_type head_a, size_type head_b) {
                if (head_a == head_b) return;
                m_parent[head_a] = head_b;
                m_group_size[head_b] += m_group_size[head_a];
                m_group_count--;
            }
            bool unite_by_size(size_type a, size_type b) {
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
            bool in_same_group(size_type a, size_type b) { return find(a) == find(b); }
            bool is_head(size_type i) { return i == m_parent[i]; }
            size_type count() const { return m_group_count; }
            std::vector<size_type> heads() {
                std::vector<size_type> ret;
                ret.reserve(m_group_count);
                for (size_type i = 0; i < m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() {
                std::vector<std::vector<size_type>> ret(m_group_count);
                std::vector<size_type> index(m_size);
                for (size_type i = 0, j = 0; i < m_size; i++)
                    if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                for (size_type i = 0; i < m_size; i++) ret[index[find(i)]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream, size_type MAX_NODE = 1 << 22>
        Ostream &operator<<(Ostream &out, const Table<MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.m_parent[i];
            }
            return out << "]";
        }
        template <size_type MAX_NODE>
        size_type Table<MAX_NODE>::s_buffer[MAX_NODE];
        template <size_type MAX_NODE>
        size_type Table<MAX_NODE>::s_size_buffer[MAX_NODE];
        template <size_type MAX_NODE>
        size_type Table<MAX_NODE>::s_use_count;
    }
    template <UF::size_type MAX_NODE = 1 << 22>
    using UnionFind = UF::Table<MAX_NODE>;
}

#endif