/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LICHAOZKWTREE__
#define __OY_LICHAOZKWTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LichaoZkw {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            Tp calc(Tp i) const { return m_k * i + m_b; }
        };
        template <typename Line>
        struct BaseLess {
            bool operator()(const Line &x, const Line &y, size_type i) const { return x.calc(i) < y.calc(i); }
        };
        template <typename Line = BaseLine<double>, typename Compare = BaseLess<Line>>
        struct Tree {
            std::vector<Line> m_lines;
            size_type m_depth, m_size, m_capacity;
            Compare m_comp;
            Line m_default_line;
            void _add(size_type i, size_type floor, size_type ceil, Line line) {
                size_type mid = (floor + ceil) >> 1;
                if (m_comp(m_lines[i], line, mid)) std::swap(m_lines[i], line);
                if (floor < ceil)
                    if (m_comp(m_lines[i], line, floor))
                        _add(i * 2, floor, mid, line);
                    else if (m_comp(m_lines[i], line, ceil))
                        _add(i * 2 + 1, mid + 1, ceil, line);
            }
            Tree(size_type length = 0, Compare comp = Compare(), const Line &default_line = Line()) : m_comp(comp), m_default_line(default_line) { resize(length); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth;
                m_lines.assign(1 << (m_depth + 1), m_default_line);
            }
            void add(size_type i, const Line &line) {
                if (m_comp(m_lines[m_capacity + i], line, i)) m_lines[m_capacity + i] = line;
            }
            void add(size_type left, size_type right, const Line &line) {
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        _add((m_capacity + left) >> j, left, left2 - 1, line);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    _add((m_capacity + left) >> (j - 1), left, left2 - 1, line);
                    left = left2;
                }
            }
            Line query(size_type i) const {
                Line res = m_lines[i + (1 << m_depth)];
                for (size_type j = i + (1 << m_depth); j >>= 1;)
                    if (m_comp(res, m_lines[j], i)) res = m_lines[j];
                return res;
            }
        };
    }
    template <typename Tp>
    using LichaoSlopeZkwTree = LichaoZkw::Tree<LichaoZkw::BaseLine<Tp>, LichaoZkw::BaseLess<LichaoZkw::BaseLine<Tp>>>;
}

#endif