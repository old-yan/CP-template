/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKDISJOINTUNION__
#define __OY_ROLLBACKDISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace RollbackDSU {
        using size_type = uint32_t;
        class Table {
            struct Record {
                size_type m_head_a, m_head_b;
            };
            std::vector<size_type> m_parent, m_group_size;
            std::vector<Record> m_records;
            size_type m_size, m_group_cnt;
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size), m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
            }
            size_type find(size_type i) const { return m_parent[i] == i ? i : find(m_parent[i]); }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            template <bool MakeRecord = true>
            void unite_to(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_b;
                m_group_size[head_b] += m_group_size[head_a];
                m_group_cnt--;
                if constexpr (MakeRecord) m_records.push_back({head_a, head_b});
            }
            template <bool MakeRecord = true>
            bool unite_by_size(size_type a, size_type b) {
                a = find(a), b = find(b);
                if (a == b) return false;
                if (m_group_size[a] > m_group_size[b]) std::swap(a, b);
                unite_to<MakeRecord>(a, b);
                return true;
            }
            void cancle(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            void cancle() {
                size_type head_a = m_records.back().m_head_a, head_b = m_records.back().m_head_b;
                m_records.pop_back();
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            bool in_same_group(size_type a, size_type b) const { return find(a) == find(b); }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i < m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() const {
                std::vector<std::vector<size_type>> ret(m_group_cnt);
                std::vector<size_type> index(m_size);
                for (size_type i = 0, j = 0; i != m_size; i++)
                    if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream>
        Ostream &operator<<(Ostream &out, const Table &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.m_parent[i];
            }
            return out << "]";
        }
    }
}

#endif