#ifndef __OY_UNION__
#define __OY_UNION__

#include <numeric>
#include <vector>

namespace OY {
    class unionFind {
        int m_groupCnt;
        std::vector<int> m_parent, m_size;

    public:
        unionFind(int n = 1 << 20) {
            resize(n);
        }
        void resize(int n) {
            m_groupCnt = n;
            m_parent.resize(n);
            std::iota(m_parent.begin(), m_parent.end(), 0);
            m_size.resize(n);
            std::fill(m_size.begin(), m_size.end(), 1);
        }
        int find(int i) {
            return m_parent[i] == i ? i : m_parent[i] = find(m_parent[i]);
        }
        int size(int i) {
            return m_size[find(i)];
        }
        void uniteTo(int headA, int headB) {
            if (headA == headB) return;
            m_parent[headA] = headB;
            m_size[headB] += m_size[headA];
            m_groupCnt--;
        }
        bool uniteBySize(int a, int b) {
            if (a = find(a), b = find(b); a == b) return false;
            if (m_size[a] > m_size[b]) std::swap(a, b);
            uniteTo(a, b);
            return true;
        }
        bool uniteByID(int a, int b) {
            if (a = find(a), b = find(b); a == b) return false;
            if (a < b) std::swap(a, b);
            uniteTo(a, b);
            return true;
        }
        bool isSame(int a, int b) {
            return find(a) == find(b);
        }
        bool isHead(int i) {
            return i == m_parent[i];
        }
        std::vector<int> heads() {
            std::vector<int> ret;
            ret.reserve(m_groupCnt);
            for (int i = 0; i < m_parent.size(); i++)
                if (isHead(i)) ret.push_back(i);
            return ret;
        }
        std::vector<std::vector<int>> groups() {
            std::vector<std::vector<int>> ret;
            ret.resize(m_groupCnt);
            std::vector<int> index(m_parent.size());
            for (int i = 0, j = 0; i < m_parent.size(); i++)
                if (isHead(i)) {
                    ret[j].reserve(m_size[i]);
                    index[i] = j++;
                }
            for (int i = 0; i < m_parent.size(); i++)
                ret[index[find(i)]].push_back(i);
            return ret;
        }
    };
}

#endif