#ifndef __OY_LICHAOTREE__
#define __OY_LICHAOTREE__

#include <vector>

namespace OY {
    struct LichaoLine {
        double k;
        double b;
        LichaoLine() = default;
        LichaoLine(double _k, double _b) : k(_k), b(_b) {}
        double calc(int i) const { return k * i + b; }
    };
    template <typename _Line>
    struct LichaoLess {
        bool operator()(const _Line &x, const _Line &y, int i) const { return x.calc(i) < y.calc(i); }
    };
    template <typename _Line = LichaoLine, typename _Compare = LichaoLess<_Line>>
    class LichaoTree {
        std::vector<_Line> m_lines;
        int m_depth;
        int m_length;
        _Line m_defaultLine;
        _Compare m_comp;

    public:
        LichaoTree(int __n = 0, _Compare __comp = _Compare(), _Line __defaultLine = _Line()) : m_comp(__comp), m_defaultLine(__defaultLine) {
            resize(__n);
        }
        void resize(int __n) {
            if (!__n) return;
            m_length = __n;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero(m_length - 1) : 32);
            m_lines.assign(1 << (m_depth + 1), m_defaultLine);
        }
        void add(int __left, int __right, _Line __line) {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length - 1;
            if (__left > __right) return;
            auto dfs = [&](auto self, int i, int left, int right, _Line line) -> void {
                int mid = (left + right) / 2;
                if (__left <= left && __right >= right) {
                    if (m_comp(m_lines[i], line, mid)) std::swap(m_lines[i], line);
                    if (left < right) {
                        if (m_comp(m_lines[i], line, left))
                            self(self, i * 2, left, mid, line);
                        else if (m_comp(m_lines[i], line, right))
                            self(self, i * 2 + 1, mid + 1, right, line);
                    }
                } else {
                    if (__left <= mid) self(self, i * 2, left, mid, line);
                    if (__right > mid) self(self, i * 2 + 1, mid + 1, right, line);
                }
            };
            dfs(dfs, 1, 0, (1 << m_depth) - 1, __line);
        }
        _Line query(int i) const {
            _Line res = m_defaultLine;
            for (int j = i + (1 << m_depth); j; j >>= 1)
                if (m_comp(res, m_lines[j], i)) res = m_lines[j];
            return res;
        }
    };
    template <typename _Line = LichaoLine, typename _Compare = LichaoLess<_Line>>
    LichaoTree(int = 0, _Compare = _Compare(), _Line = _Line()) -> LichaoTree<_Line, _Compare>;
}

#endif