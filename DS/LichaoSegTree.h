#ifndef __OY_LICHAOSEGTREE__
#define __OY_LICHAOSEGTREE__

#include <algorithm>
#include <cstdint>

namespace OY {
    struct LichaoSegLine {
        double m_k, m_b;
        LichaoSegLine() = default;
        LichaoSegLine(double k, double b) : m_k(k), m_b(b) {}
        double calc(int i) const { return m_k * i + m_b; }
    };
    template <typename Line>
    struct LichaoSegLess {
        bool operator()(const Line &x, const Line &y, int i) const { return x.calc(i) < y.calc(i); }
    };
    template <typename Line = LichaoSegLine, typename Compare = LichaoSegLess<Line>>
    struct LichaoSegTree {
        struct LineNode {
            Line m_line;
            LineNode *m_lchild, *m_rchild;
            LineNode(Line line, LineNode *lchild = nullptr, LineNode *rchild = nullptr) : m_line(line), m_lchild(lchild), m_rchild(rchild) {}
        };
        Line m_defaultLine;
        uint64_t m_length;
        Compare m_comp;
        LineNode *m_root;
        void _clear(LineNode *p) {
            // if (p->m_lchild) _clear(p->m_lchild);
            // if (p->m_rchild) _clear(p->m_rchild);
            // delete p;
        }
        LichaoSegTree(uint64_t length = 0, Compare comp = Compare(), Line defaultLine = Line()) : m_comp(comp), m_defaultLine(defaultLine), m_root(nullptr) { resize(length); }
        ~LichaoSegTree() {
            if (m_root) _clear(m_root);
        }
        void resize(uint64_t length) {
            if (m_root) _clear(m_root);
            m_root = (m_length = length) ? new LineNode(m_defaultLine, nullptr, nullptr) : nullptr;
        }
        void add(uint64_t left, uint64_t right, const Line &line) {
            if (right >= m_length) right = m_length - 1;
            if (left > right) return;
            auto dfs = [&](auto self, LineNode *&cur, uint64_t lbound, uint64_t rbound, Line line) -> void {
                if (!cur) cur = new LineNode(m_defaultLine, nullptr, nullptr);
                if (uint64_t mid = (lbound + rbound) / 2; left <= lbound && right >= rbound) {
                    if (m_comp(cur->m_line, line, mid)) std::swap(cur->m_line, line);
                    if (lbound < rbound)
                        if (m_comp(cur->m_line, line, lbound))
                            self(self, cur->m_lchild, lbound, mid, line);
                        else if (m_comp(cur->m_line, line, rbound))
                            self(self, cur->m_rchild, mid + 1, rbound, line);
                } else {
                    if (left <= mid) self(self, cur->m_lchild, lbound, mid, line);
                    if (right > mid) self(self, cur->m_rchild, mid + 1, rbound, line);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1, line);
        }
        Line query(uint64_t i) const {
            Line res(m_defaultLine);
            auto dfs = [&](auto self, LineNode *cur, uint64_t lbound, uint64_t rbound) -> void {
                if (!cur) return;
                if (m_comp(res, cur->m_line, i))
                    res = cur->m_line;
                if (lbound == rbound) return;
                if (uint64_t mid = (lbound + rbound) / 2; i <= mid)
                    self(self, cur->m_lchild, lbound, mid);
                else
                    self(self, cur->m_rchild, mid + 1, rbound);
            };
            dfs(dfs, m_root, 0, m_length - 1);
            return res;
        }
    };
    template <typename Line = LichaoSegLine, typename Compare = LichaoSegLess<Line>>
    LichaoSegTree(uint64_t = 0, Compare = Compare(), Line = Line()) -> LichaoSegTree<Line, Compare>;
}

#endif