#ifndef __OY_LONGSHORTDECOMPOSITION__
#define __OY_LONGSHORTDECOMPOSITION__

#include <bitset>
#include <functional>

#include "../DS/STtable.h"
#include "Tree.h"

namespace OY {
    template <typename _Tree, typename _Tp, typename _Maximum, template <typename...> typename _Solver = STTable>
    struct LongShortDecomposition {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        static inline std::bitset<_MAXN> s_visit;
        static inline _Tp s_buffer[_MAXN * 2];
        _Tree &m_tree;
        uint32_t m_height[_MAXN];
        uint32_t m_depth[_MAXN];
        uint32_t m_heavySon[_MAXN];
        uint32_t m_belongTop[_MAXN];
        uint32_t m_belongTopVal[_MAXN];
        uint32_t m_upDown[_MAXN * 2];
        uint32_t m_fa[32 - std::__countl_zero(_MAXN - 1)][_MAXN];
        _Tp m_val[_MAXN];
        _Tp m_sub[32 - std::__countl_zero(_MAXN - 1)][_MAXN];
        _Maximum m_maxi;
        _Solver<_Tp, _Maximum> m_sol;
        LongShortDecomposition(_Tree &__tree, _Maximum __maxi = std::max<_Tp>, _Tp __defaultValue = _Tp()) : m_tree(__tree), m_sol(0, __maxi, __defaultValue), m_maxi(__maxi) {}
        template <typename _Mapping>
        LongShortDecomposition(_Tree &__tree, _Mapping __map, _Maximum __maxi = std::max<_Tp>, _Tp __defaultValue = _Tp()) : LongShortDecomposition(__tree, __maxi, __defaultValue) { reset(__map); }
        template <typename _Mapping>
        void reset(_Mapping __map) {
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) m_val[i] = __map(i);
            auto getHeavy = [&](auto self, uint32_t i, uint32_t d) -> void {
                m_height[i] = 1;
                m_depth[i] = d++;
                m_heavySon[i] = -1;
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_tree.m_to[cur];
                    self(self, to, d);
                    m_fa[0][to] = i;
                    m_sub[0][to] = m_maxi(m_val[i], m_val[to]);
                    if (chmax(m_height[i], m_height[to] + 1)) m_heavySon[i] = to;
                }
            };
            getHeavy(getHeavy, m_tree.m_root, 0);
            uint32_t cursor = 0, level = 32 - std::__countl_zero(*std::max_element(m_depth, m_depth + m_tree.m_vertexNum));
            m_fa[0][m_tree.m_root] = -1;
            m_sub[0][m_tree.m_root] = m_val[m_tree.m_root];
            for (uint32_t j = 1; j < level; j++)
                for (uint32_t i = 0; i < m_tree.m_vertexNum; i++)
                    if (uint32_t p = m_fa[j - 1][i]; ~p)
                        m_fa[j][i] = m_fa[j - 1][p], m_sub[j][i] = m_maxi(m_sub[j - 1][p], m_sub[j - 1][i]);
                    else
                        m_fa[j][i] = -1, m_sub[j][i] = m_sub[j - 1][i];
            auto setTop = [&](uint32_t top) {
                uint32_t h = m_height[top];
                m_upDown[cursor] = m_upDown[cursor + 1] = -1;
                for (uint32_t k = cursor + h, kend = cursor + h * 2, cur = top; k < kend; k++, cur = m_heavySon[cur]) m_upDown[k] = cur;
                for (uint32_t k = cursor + h - 1, kend = cursor + 1, cur = top; k > kend && ~cur; k--) m_upDown[k] = cur = m_fa[0][cur];
                return h + cursor - m_depth[top];
            };
            auto getOrder = [&](auto self, uint32_t i, uint32_t top, uint32_t topVal) -> void {
                s_visit.set(i);
                m_belongTop[i] = top;
                m_belongTopVal[i] = topVal;
                cursor += 2;
                if (!~m_heavySon[i]) return;
                self(self, m_heavySon[i], top, topVal);
                for (uint32_t cur = m_tree.m_starts[i], end = m_tree.m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_tree.m_to[cur]; !s_visit[to]) self(self, to, to, setTop(to));
            };
            getOrder(getOrder, m_tree.m_root, m_tree.m_root, setTop(m_tree.m_root));
            for (uint32_t i = 0; i < m_tree.m_vertexNum * 2; i++) s_buffer[i] = ~m_upDown[i] ? m_val[m_upDown[i]] : m_sol.m_defaultValue;
            m_sol.reset(s_buffer, s_buffer + m_tree.m_vertexNum * 2);
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) s_visit.reset(i);
        }
        uint32_t getAncestor(uint32_t __a, uint32_t __n) const {
            if (!__n) return __a;
            return m_upDown[m_belongTopVal[m_fa[31 - std::__countl_zero(__n)][__a]] + (m_depth[__a] - __n)];
        }
        uint32_t lca(uint32_t __a, uint32_t __b) const {
            while (true) {
                uint32_t atop = m_belongTop[__a], btop = m_belongTop[__b];
                if (atop == btop) break;
                if (m_depth[atop] < m_depth[btop])
                    __b = m_fa[0][btop];
                else
                    __a = m_fa[0][atop];
            }
            return m_depth[__a] < m_depth[__b] ? __a : __b;
        }
        _Tp query(uint32_t __a) const { return m_val[__a]; }
        _Tp queryUp(uint32_t __a, uint32_t __n) const {
            if (!__n) return m_val[__a];
            uint32_t l = 31 - std::__countl_zero(__n), pos = m_belongTopVal[m_fa[l][__a]] + m_depth[__a] - __n;
            return m_maxi(m_sub[l][__a], m_sol.query(pos, pos + (__n - (1 << l))));
        }
        _Tp query(uint32_t __a, uint32_t __b) const { return query(__a, __b, lca(__a, __b)); }
        _Tp query(uint32_t __a, uint32_t __b, uint32_t __p) const {
            if (__p == __a)
                return queryUp(__b, m_depth[__b] - m_depth[__a]);
            else if (__p == __b)
                return queryUp(__a, m_depth[__a] - m_depth[__b]);
            else
                return m_maxi(queryUp(__a, m_depth[__a] - m_depth[__p]), queryUp(__b, m_depth[__b] - m_depth[__p]));
        }
    };
    template <typename _Tree, typename _Tp, typename _Maximum, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, _Maximum, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, _Maximum, _Solver>;
    template <typename _Tree, typename _Tp, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, const _Tp &(*)(const _Tp &, const _Tp &), _Solver>;
    template <typename _Tree, typename _Tp, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, _Tp (*)(_Tp, _Tp) = std::max<_Tp>, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, _Tp (*)(_Tp, _Tp), _Solver>;
    template <typename _Tree, typename _Mapping, typename _Maximum, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Mapping::operator()))::result_type>, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, _Mapping, _Maximum, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, _Maximum, _Solver>;
    template <typename _Tree, typename _Mapping, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Mapping::operator()))::result_type>, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, _Mapping, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, const _Tp &(*)(const _Tp &, const _Tp &), _Solver>;
    template <typename _Tree, typename _Mapping, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Mapping::operator()))::result_type>, template <typename...> typename _Solver = STTable>
    LongShortDecomposition(_Tree &, _Mapping, _Tp (*)(_Tp, _Tp) = std::max<_Tp>, _Tp = _Tp()) -> LongShortDecomposition<_Tree, _Tp, _Tp (*)(_Tp, _Tp), _Solver>;
}

#endif