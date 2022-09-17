#ifndef __OY_UNION__
#define __OY_UNION__

#include <algorithm>
#include <numeric>

namespace OY {
    struct UnionFind {
        uint32_t m_groupCnt;
        std::vector<uint32_t> m_parent, m_size;
        UnionFind(uint32_t __n = 0) { resize(__n); }
        void resize(uint32_t __n) {
            m_groupCnt = __n;
            m_parent.resize(__n);
            std::iota(m_parent.begin(), m_parent.end(), 0);
            m_size.resize(__n);
            std::fill(m_size.begin(), m_size.end(), 1);
        }
        uint32_t find(uint32_t __i) { return m_parent[__i] == __i ? __i : m_parent[__i] = find(m_parent[__i]); }
        uint32_t size(uint32_t __i) { return m_size[find(__i)]; }
        void uniteTo(uint32_t __headA, uint32_t __headB) {
            if (__headA == __headB) return;
            m_parent[__headA] = __headB;
            m_size[__headB] += m_size[__headA];
            m_groupCnt--;
        }
        bool uniteBySize(uint32_t __a, uint32_t __b) {
            if (__a = find(__a), __b = find(__b); __a == __b) return false;
            if (m_size[__a] > m_size[__b]) std::swap(__a, __b);
            uniteTo(__a, __b);
            return true;
        }
        bool uniteByID(uint32_t __a, uint32_t __b) {
            if (__a = find(__a), __b = find(__b); __a == __b) return false;
            if (__a < __b) std::swap(__a, __b);
            uniteTo(__a, __b);
            return true;
        }
        bool isSame(uint32_t __a, uint32_t __b) { return find(__a) == find(__b); }
        bool isHead(uint32_t __i) { return __i == m_parent[__i]; }
        uint32_t count() const { return m_groupCnt; }
        std::vector<uint32_t> heads() {
            std::vector<uint32_t> ret;
            ret.reserve(m_groupCnt);
            for (uint32_t i = 0; i < m_parent.size(); i++)
                if (isHead(i)) ret.push_back(i);
            return ret;
        }
        std::vector<std::vector<uint32_t>> groups() {
            std::vector<std::vector<uint32_t>> ret;
            ret.resize(m_groupCnt);
            uint32_t index[m_parent.size()];
            for (uint32_t i = 0, j = 0; i < m_parent.size(); i++)
                if (isHead(i)) {
                    ret[j].reserve(m_size[i]);
                    index[i] = j++;
                }
            for (uint32_t i = 0; i < m_parent.size(); i++)
                ret[index[find(i)]].push_back(i);
            return ret;
        }
    };
}

#endif