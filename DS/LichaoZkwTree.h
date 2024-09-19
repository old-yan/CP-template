/*
最后修改:
20240915
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
    namespace LCZKW {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            Tp calc(Tp i) const { return m_k * i + m_b; }
        };
        struct BaseLess {
            template <typename Tp>
            bool operator()(const Tp &x, const Tp &y) const { return x < y; }
        };
        template <typename Line = BaseLine<double>, typename Compare = BaseLess>
        class Tree {
        public:
            using value_type = decltype(std::declval<Line>().calc(0));
        private:
            std::vector<Line> m_lines;
            size_type m_size, m_cap;
            Line m_default_line;
            void _add(size_type i, size_type floor, size_type ceil, Line line) {
                size_type mid = (floor + ceil) >> 1;
                if (Compare()(m_lines[i].calc(mid), line.calc(mid))) std::swap(m_lines[i], line);
                if (floor < ceil)
                    if (Compare()(m_lines[i].calc(floor), line.calc(floor)))
                        _add(i * 2, floor, mid, line);
                    else if (Compare()(m_lines[i].calc(ceil), line.calc(ceil)))
                        _add(i * 2 + 1, mid + 1, ceil, line);
            }
        public:
            Tree(size_type length = 0, const Line &default_line = Line()) : m_default_line(default_line) { resize(length); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size);
                m_lines.assign(m_cap << 1, m_default_line);
            }
            void add(size_type i, const Line &line) {
                if (Compare()(m_lines[m_cap + i].calc(i), line.calc(i))) m_lines[m_cap + i] = line;
            }
            void add(size_type left, size_type right, const Line &line) {
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        _add((m_cap + left) >> j, left, left2 - 1, line);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    _add((m_cap + left) >> (j - 1), left, left2 - 1, line);
                    left = left2;
                }
            }
            Line query(size_type i) const {
                Line res = m_lines[m_cap + i];
                for (size_type d = std::countr_zero(m_cap); d; d--)
                    if (Compare()(res.calc(i), m_lines[(m_cap + i) >> d].calc(i))) res = m_lines[(m_cap + i) >> d];
                return res;
            }
        };
    }
    template <typename Tp>
    using LichaoSlopeChmaxZkwTree = LCZKW::Tree<LCZKW::BaseLine<Tp>, std::less<Tp>>;
    template <typename Tp>
    using LichaoSlopeChminZkwTree = LCZKW::Tree<LCZKW::BaseLine<Tp>, std::greater<Tp>>;
}

#endif