/*
最后修改:
20230829
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LICHAOSEGTREE__
#define __OY_LICHAOSEGTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace LichaoSeg {
        using index_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            Tp calc(Tp i) const { return m_k * i + m_b; }
        };
        template <typename Line>
        struct BaseLess {
            template <typename SizeType>
            bool operator()(const Line &x, const Line &y, SizeType i) const { return x.calc(i) < y.calc(i); }
        };
        template <typename Line = BaseLine<double>, typename Compare = BaseLess<Line>, typename SizeType = uint64_t, index_type MAX_NODE = 1 << 20>
        struct Tree {
            static struct node {
                Line m_line;
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            } s_buffer[MAX_NODE];
            static index_type s_use_count;
            index_type m_root;
            SizeType m_size;
            Compare m_comp;
            Line m_default_line;
            index_type _newnode() {
                s_buffer[s_use_count].m_line = m_default_line;
                return s_use_count++;
            }
            node *_root() const { return s_buffer + m_root; }
            node *_lchild(node *cur) {
                if (!cur->m_lchild) cur->m_lchild = _newnode();
                return cur->lchild();
            }
            node *_rchild(node *cur) {
                if (!cur->m_rchild) cur->m_rchild = _newnode();
                return cur->rchild();
            }
            void _add(node *cur, SizeType floor, SizeType ceil, Line line) {
                SizeType mid = (floor + ceil) >> 1;
                if (m_comp(cur->m_line, line, mid)) std::swap(cur->m_line, line);
                if (floor < ceil)
                    if (m_comp(cur->m_line, line, floor)) {
                        _add(_lchild(cur), floor, mid, line);
                    } else if (m_comp(cur->m_line, line, ceil))
                        _add(_rchild(cur), mid + 1, ceil, line);
            }
            void _add(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Line line) {
                if (left <= floor && right >= ceil)
                    _add(cur, floor, ceil, line);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _add(_lchild(cur), floor, mid, left, right, line);
                    if (right > mid) _add(_rchild(cur), mid + 1, ceil, left, right, line);
                }
            }
            void _add(node *cur, SizeType floor, SizeType ceil, SizeType i, Line line) {
                if (floor == ceil) {
                    if (m_comp(cur->m_line, line, i)) cur->m_line = line;
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _add(_lchild(cur), floor, mid, i, line);
                    else
                        _add(_rchild(cur), mid + 1, ceil, i, line);
                }
            }
            Line _query(node *cur, SizeType floor, SizeType ceil, SizeType i) const {
                if (floor == ceil) return cur->m_line;
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid) {
                    if (!cur->m_lchild) return cur->m_line;
                    Line line = _query(cur->lchild(), floor, mid, i);
                    return m_comp(cur->m_line, line, i) ? line : cur->m_line;
                } else {
                    if (!cur->m_rchild) return cur->m_line;
                    Line line = _query(cur->rchild(), mid + 1, ceil, i);
                    return m_comp(cur->m_line, line, i) ? line : cur->m_line;
                }
            }
            Tree(SizeType length = 0, Compare comp = Compare(), Line default_line = Line()) : m_comp(comp), m_default_line(default_line) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = _newnode();
            }
            void add(SizeType i, const Line &line) { _add(_root(), 0, m_size - 1, i, line); }
            void add(SizeType left, SizeType right, const Line &line) { _add(_root(), 0, m_size - 1, left, right, line); }
            Line query(SizeType i) const { return _query(_root(), 0, m_size - 1, i); }
        };
        template <typename Node, typename Compare, typename SizeType, index_type MAX_NODE>
        typename Tree<Node, Compare, SizeType, MAX_NODE>::node Tree<Node, Compare, SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename Compare, typename SizeType, index_type MAX_NODE>
        index_type Tree<Node, Compare, SizeType, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp, typename SizeType = uint64_t, LichaoSeg::index_type MAX_NODE = 1 << 20>
    using LichaoSlopeSegTree = LichaoSeg::Tree<LichaoSeg::BaseLine<Tp>, LichaoSeg::BaseLess<LichaoSeg::BaseLine<Tp>>, SizeType, MAX_NODE>;
}

#endif