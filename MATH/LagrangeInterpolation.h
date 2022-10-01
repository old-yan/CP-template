#ifndef __OY_LAGRANGEINTERPOLATION__
#define __OY_LAGRANGEINTERPOLATION__

#include <algorithm>

namespace OY {
    template <typename _Tp>
    struct LagrangeInterpolation {
        struct _Point {
            _Tp x, y;
        };
        std::vector<_Point> m_points;
        std::vector<_Tp> m_coef;
        LagrangeInterpolation(uint32_t __pointNum = 0) { m_points.reserve(__pointNum); }
        void addPoint(_Tp __x, _Tp __y) { m_points.push_back({__x, __y}); }
        void prepare() {
            m_coef.reserve(m_points.size());
            for (auto &[xi, yi] : m_points) {
                _Tp denominator = 1;
                for (auto &[xj, yj] : m_points)
                    if (xi != xj) denominator *= xi - xj;
                m_coef.push_back(yi / denominator);
            }
        }
        void prepareIfSpecialX() {
            _Tp fac[m_points.size()], facInv[m_points.size() + 1];
            fac[0] = 1;
            for (uint32_t i = 1; i < m_points.size(); i++) fac[i] = fac[i - 1] * i;
            facInv[m_points.size()] = 1;
            for (uint32_t i = m_points.size() - 1; ~i; i--) facInv[i] = facInv[i + 1] * fac[i];
            _Tp prod = facInv[0], inv = 1 / prod;
            for (uint32_t i = 0; i < m_points.size(); i++) {
                facInv[i] = inv * facInv[i + 1];
                inv *= fac[i];
            }
            m_coef.reserve(m_points.size());
            for (uint32_t i = 0; i < m_points.size(); i++) {
                m_coef.push_back(m_points[i].y * facInv[i] * facInv[m_points.size() - 1 - i]);
                if ((m_points.size() - i - 1) % 2) m_coef.back() = -m_coef.back();
            }
        }
        _Tp calc(_Tp __x) const {
            for (auto &[xi, yi] : m_points)
                if (__x == xi) return yi;
            _Tp dif[m_points.size() + 1];
            dif[m_points.size()] = 1;
            for (uint32_t i = m_points.size() - 1; ~i; i--) dif[i] = dif[i + 1] * (__x - m_points[i].x);
            _Tp res = 0, prod = dif[0], inv = 1 / prod;
            for (uint32_t i = 0; i < m_points.size(); i++) {
                res += m_coef[i] * prod * inv * dif[i + 1];
                inv *= __x - m_points[i].x;
            }
            return res;
        }
    };
}

#endif