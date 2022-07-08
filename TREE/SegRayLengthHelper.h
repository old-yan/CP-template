#ifndef __OY_SEGRAYLENGTHHELPER__
#define __OY_SEGRAYLENGTHHELPER__

#include "Tree.h"

namespace OY {
    template <typename _Tree>
    struct SegRayLengthSolver {
        using _Tp = typename _Tree::distance_type;
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        _Tp m_ray[_MAXN][3];
        _Tp m_seg[_MAXN][2];
        _Tp m_downRay[_MAXN];
        _Tp m_downSeg[_MAXN];
        _Tp m_upRay[_MAXN];
        _Tp m_upSeg[_MAXN];
        SegRayLengthSolver(_Tree &__tree) : m_tree(__tree) {
            std::fill(m_ray[0],m_ray[m_tree.m_vertexNum],0);
            std::fill(m_seg[0],m_seg[m_tree.m_vertexNum],0);
            std::fill(m_downRay,m_downRay+m_tree.m_vertexNum,0);
            std::fill(m_downSeg,m_downSeg+m_tree.m_vertexNum,0);
            std::fill(m_upRay,m_upRay+m_tree.m_vertexNum,0);
            std::fill(m_upSeg,m_upSeg+m_tree.m_vertexNum,0);
            auto dfs1 = [&](auto self, uint32_t i) -> void {
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_tree.m_to[cur];
                    self(self, to);
                    addDownRay(i, m_downRay[to] + m_tree.m_distances[cur]);
                    addDownSeg(i, m_downSeg[to]);
                }
                chmax(m_downSeg[i], m_ray[i][0] + m_ray[i][1]);
            };
            dfs1(dfs1, m_tree.m_root);
            auto dfs2 = [&](auto self, uint32_t i, _Tp upRay, _Tp upSeg) -> void {
                setUpRay(i, upRay);
                setUpSeg(i, upSeg);
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_tree.m_to[cur];
                    _Tp distance = m_tree.m_distances[cur];
                    auto [ray, seg] = maxRaySeg(i, m_downRay[to] + distance, m_downSeg[to]);
                    chmax(seg, ray += distance);
                    self(self, to, ray, seg);
                }
            };
            dfs2(dfs2, m_tree.m_root, 0, 0);
        }
        void addRay(uint32_t __i, _Tp __ray) {
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
        void addSeg(uint32_t __i, _Tp __seg) {
            if (__seg > m_seg[__i][0]) {
                m_seg[__i][1] = m_seg[__i][0];
                m_seg[__i][0] = __seg;
            } else if (__seg > m_seg[__i][1])
                m_seg[__i][1] = __seg;
        }
        void addDownRay(uint32_t __i, _Tp __ray) {
            chmax(m_downRay[__i], __ray);
            addRay(__i, __ray);
        }
        void addDownSeg(uint32_t __i, _Tp __seg) {
            chmax(m_downSeg[__i], __seg);
            addSeg(__i, __seg);
        }
        void setUpRay(uint32_t __i, _Tp __ray) {
            m_upRay[__i] = __ray;
            addRay(__i, __ray);
        }
        void setUpSeg(uint32_t __i, _Tp __seg) {
            m_upSeg[__i] = __seg;
            addSeg(__i, __seg);
        }
        auto maxRaySeg(uint32_t __i, _Tp __exceptRay, _Tp __exceptSeg) const {
            struct _RaySeg {
                _Tp ray, seg;
            };
            _Tp ray = __exceptRay == m_ray[__i][0] ? m_ray[__i][1] : m_ray[__i][0];
            _Tp seg = __exceptSeg == m_seg[__i][0] ? m_seg[__i][1] : m_seg[__i][0];
            _Tp rays = __exceptRay == m_ray[__i][0] ? m_ray[__i][1] + m_ray[__i][2] : (__exceptRay == m_ray[__i][1] ? m_ray[__i][0] + m_ray[__i][2] : m_ray[__i][0] + m_ray[__i][1]);
            return _RaySeg{ray, std::max(seg, rays)};
        }
    };
}

#endif