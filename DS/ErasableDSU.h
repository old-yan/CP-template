/*
最后修改:
20241206
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ERASABLEDISJOINTUNION__
#define __OY_ERASABLEDISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace EDSU {
        using size_type = uint32_t;
        class Table {
        public:
            size_type _size_of_head(size_type head) const { return m_group_size[head]; }
        private:
            mutable std::vector<size_type> m_parent, m_group_size;
            size_type m_size, m_group_cnt;
            size_type _find(size_type i) const { return m_parent[i] == i ? i : m_parent[i] = _find(m_parent[i]); }
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size * 2);
                for (size_type i = 0; i != m_size; i++) m_parent[i] = m_size + i;
                for (size_type i = 0; i != m_size; i++) m_parent[m_size + i] = m_size + i;
                m_group_size.assign(m_size, 1);
            }
            size_type find(size_type i) const { return _find(i) - m_size; }
            size_type extract(size_type i) {
                size_type head = find(i);
                if (m_group_size[head] == 1) return head;
                m_parent[i] = m_parent.size();
                m_parent.push_back(m_parent.size());
                m_group_size[head]--;
                m_group_size.push_back(1);
                m_group_cnt++;
                return m_parent[i] - m_size;
            }
            size_type size() const { return m_size; }
            size_type size(size_type i) const { return m_group_size[find(i)]; }
            void unite_to(size_type head_a, size_type head_b) {
                if (head_a == head_b) return;
                m_parent[m_size + head_a] = m_size + head_b;
                m_group_size[head_b] += m_group_size[head_a], m_group_size[head_a] = 0;
                m_group_cnt--;
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
            bool in_same_group(size_type a, size_type b) const { return find(a) == find(b); }
            size_type count() const { return m_group_cnt; }
        };
    }
}

#endif