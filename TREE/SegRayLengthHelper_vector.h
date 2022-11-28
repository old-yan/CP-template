#ifndef __OY_SEGRAYLENGTHHELPER_VECTOR__
#define __OY_SEGRAYLENGTHHELPER_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct SegRayLengthSolver_vector {
        using _Tp = typename _Tree::distance_type;
        struct _RaySeg {
            _Tp ray, seg;
        };
        _Tree &m_tree;
        std::vector<std::array<_Tp, 3>> m_ray;
        std::vector<std::array<_Tp, 2>> m_seg;
        std::vector<_Tp> m_downRay, m_upRay, m_downSeg, m_upSeg;
        SegRayLengthSolver_vector(_Tree &__tree) : m_tree(__tree), m_ray(__tree.m_vertexNum), m_seg(__tree.m_vertexNum), m_downRay(__tree.m_vertexNum), m_upRay(__tree.m_vertexNum), m_downSeg(__tree.m_vertexNum), m_upSeg(__tree.m_vertexNum) {
            auto dfs1 = [&](auto self, uint32_t i) -> void {
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        self(self, to);
                        _addDownRay(i, m_downRay[to] + adj.getDis());
                        _addDownSeg(i, m_downSeg[to]);
                    }
                chmax(m_downSeg[i], m_ray[i][0] + m_ray[i][1]);
            };
            dfs1(dfs1, m_tree.m_root);
            auto dfs2 = [&](auto self, uint32_t i, _Tp upRay, _Tp upSeg) -> void {
                _setUpRay(i, upRay);
                _setUpSeg(i, upSeg);
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        auto [ray, seg] = maxRaySeg(i, m_downRay[to] + adj.getDis(), m_downSeg[to]);
                        _addSeg(to, seg);
                        chmax(seg, ray += adj.getDis());
                        self(self, to, ray, seg);
                    }
            };
            dfs2(dfs2, m_tree.m_root, 0, 0);
        }
        void _addRay(uint32_t __i, _Tp __ray) {
            if (__ray > m_ray[__i][0]) {
                m_ray[__i][2] = m_ray[__i][1];
                m_ray[__i][1] = m_ray[__i][0];
                m_ray[__i][0] = __ray;
            } else if (__ray > m_ray[__i][1]) {
                m_ray[__i][2] = m_ray[__i][1];
                m_ray[__i][1] = __ray;
            } else if (__ray > m_ray[__i][2])
                m_ray[__i][2] = __ray;
        }
        void _addSeg(uint32_t __i, _Tp __seg) {
            if (__seg > m_seg[__i][0]) {
                m_seg[__i][1] = m_seg[__i][0];
                m_seg[__i][0] = __seg;
            } else if (__seg > m_seg[__i][1])
                m_seg[__i][1] = __seg;
        }
        void _addDownRay(uint32_t __i, _Tp __ray) {
            chmax(m_downRay[__i], __ray);
            _addRay(__i, __ray);
        }
        void _addDownSeg(uint32_t __i, _Tp __seg) {
            chmax(m_downSeg[__i], __seg);
            _addSeg(__i, __seg);
        }
        void _setUpRay(uint32_t __i, _Tp __ray) {
            m_upRay[__i] = __ray;
            _addRay(__i, __ray);
        }
        void _setUpSeg(uint32_t __i, _Tp __seg) { m_upSeg[__i] = __seg; }
        _RaySeg maxRaySeg(uint32_t __i, _Tp __exceptRay, _Tp __exceptSeg) const {
            _Tp ray = __exceptRay == m_ray[__i][0] ? m_ray[__i][1] : m_ray[__i][0];
            _Tp seg = __exceptSeg == m_seg[__i][0] ? m_seg[__i][1] : m_seg[__i][0];
            _Tp rays = __exceptRay == m_ray[__i][0] ? m_ray[__i][1] + m_ray[__i][2] : (__exceptRay == m_ray[__i][1] ? m_ray[__i][0] + m_ray[__i][2] : m_ray[__i][0] + m_ray[__i][1]);
            return _RaySeg{ray, std::max(seg, rays)};
        }
        _RaySeg maxRaySeg(uint32_t __i, uint32_t __exceptSonIndex) const {
            auto &adj = m_tree.m_adj[__i][__exceptSonIndex];
            return maxRaySeg(__i, m_downRay[adj.to] + adj.getDis(), m_downSeg[adj.to]);
        }
    };
}

#endif