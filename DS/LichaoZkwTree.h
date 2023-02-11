#ifndef __OY_LICHAOZKWTREE__
#define __OY_LICHAOZKWTREE__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    struct LichaoZkwLine {
        double m_k, m_b;
        LichaoZkwLine() = default;
        LichaoZkwLine(double k, double b) : m_k(k), m_b(b) {}
        double calc(int i) const { return m_k * i + m_b; }
    };
    template <typename Line>
    struct LichaoZkwLess {
        bool operator()(const Line &x, const Line &y, int i) const { return x.calc(i) < y.calc(i); }
    };
    template <typename Line = LichaoZkwLine, typename Compare = LichaoZkwLess<Line>>
    struct LichaoZkwTree {
        std::vector<Line> m_lines;
        uint32_t m_depth, m_length;
        Line m_defaultLine;
        Compare m_comp;
        LichaoZkwTree(uint32_t length = 0, Compare comp = Compare(), Line defaultLine = Line()) : m_comp(comp), m_defaultLine(defaultLine) {
            resize(length);
        }
        void resize(uint32_t length) {
            if (!(m_length = length)) return;
            m_depth = 32 - std::__countl_zero(m_length - 1);
            m_lines.assign(1 << (m_depth + 1), m_defaultLine);
        }
        void add(uint32_t left, uint32_t right, const Line &line) {
            if (right >= m_length) right = m_length - 1;
            if (left > right) return;
            auto dfs = [&](auto self, uint32_t i, uint32_t lbound, uint32_t rbound, Line line) -> void {
                if (uint32_t mid = (lbound + rbound) / 2; left <= lbound && right >= rbound) {
                    if (m_comp(m_lines[i], line, mid)) std::swap(m_lines[i], line);
                    if (lbound < rbound)
                        if (m_comp(m_lines[i], line, lbound))
                            self(self, i * 2, lbound, mid, line);
                        else if (m_comp(m_lines[i], line, rbound))
                            self(self, i * 2 + 1, mid + 1, rbound, line);
                } else {
                    if (left <= mid) self(self, i * 2, lbound, mid, line);
                    if (right > mid) self(self, i * 2 + 1, mid + 1, rbound, line);
                }
            };
            dfs(dfs, 1, 0, (1 << m_depth) - 1, line);
        }
        Line query(uint32_t i) const {
            Line res(m_defaultLine);
            for (uint32_t j = i + (1 << m_depth); j; j >>= 1)
                if (m_comp(res, m_lines[j], i)) res = m_lines[j];
            return res;
        }
    };
    template <typename Line = LichaoZkwLine, typename Compare = LichaoZkwLess<Line>>
    LichaoZkwTree(uint32_t = 0, Compare = Compare(), Line = Line()) -> LichaoZkwTree<Line, Compare>;
}

#endif