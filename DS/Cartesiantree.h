#ifndef __OY_CARTESIANTREE__
#define __OY_CARTESIANTREE__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Compare = std::less<void>, uint32_t _MAXN = 1 << 20>
    struct Cartesiantree {
        static inline uint32_t s_stack[_MAXN];
        uint32_t m_length, m_root;
        std::vector<uint32_t> m_lchild, m_rchild, m_size, m_linkParent;
        _Compare m_comp;
        template <typename _Iterator>
        Cartesiantree(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_length(__last - __first), m_lchild(__last - __first, -1), m_rchild(__last - __first, -1), m_size(__last - __first), m_linkParent(__last - __first), m_comp(__comp) {
            uint32_t len = 0;
            for (uint32_t i = 0; i < m_length; i++) {
                uint32_t popped = -1;
                while (true) {
                    if (!len) break;
                    if (!m_comp(__first[s_stack[len - 1]], __first[i])) {
                        m_rchild[s_stack[len - 1]] = i;
                        break;
                    }
                    popped = s_stack[--len];
                }
                m_lchild[i] = popped;
                s_stack[len++] = i;
            }
            auto dfs = [&](auto self, uint32_t cur) -> uint32_t {
                if (~m_lchild[cur]) m_size[cur] += self(self, m_lchild[cur]);
                if (~m_rchild[cur]) m_size[cur] += self(self, m_rchild[cur]);
                return ++m_size[cur];
            };
            dfs(dfs, m_root = s_stack[0]);
            auto dfs2 = [&](auto self, uint32_t cur, uint32_t p) -> void {
                m_linkParent[cur] = p;
                uint32_t lsize = ~m_lchild[cur] ? m_size[m_lchild[cur]] : 0, rsize = ~m_rchild[cur] ? m_size[m_rchild[cur]] : 0;
                if (!lsize && !rsize) return;
                if (lsize > rsize) {
                    self(self, m_lchild[cur], p);
                    if (rsize) self(self, m_rchild[cur], cur);
                } else {
                    self(self, m_rchild[cur], p);
                    if (lsize) self(self, m_lchild[cur], cur);
                }
            };
            dfs2(dfs2, m_root, -1);
        }
        uint32_t query(uint32_t __a, uint32_t __b) const {
            while (m_linkParent[__a] != m_linkParent[__b])
                if (!~m_linkParent[__b] || (~m_linkParent[__a] && m_size[m_linkParent[__a]] < m_size[m_linkParent[__b]]))
                    __a = m_linkParent[__a];
                else
                    __b = m_linkParent[__b];
            return m_size[__a] < m_size[__b] ? __b : __a;
        }
    };
}

#endif