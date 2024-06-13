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
#include <vector>

namespace OY {
    namespace LichaoSeg {
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
            template <typename SizeType>
            bool operator()(const Line &x, const Line &y, SizeType i) const { return x.calc(i) < y.calc(i); }
        };
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_use_cnt++; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Line = BaseLine<double>, typename Compare = BaseLess<Line>, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBuffer>
        struct Tree {
            struct node {
                Line m_line;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            size_type m_root;
            SizeType m_size;
            Compare m_comp;
            Line m_default_line;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            size_type _newnode() {
                size_type c = buffer_type::newnode();
                _ptr(c)->m_line = m_default_line;
                return c;
            }
            size_type _lchild(size_type cur) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            size_type _rchild(size_type cur) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, Line line) {
                SizeType mid = (floor + ceil) >> 1;
                node *p = _ptr(cur);
                if (m_comp(p->m_line, line, mid)) std::swap(p->m_line, line);
                if (floor < ceil)
                    if (m_comp(p->m_line, line, floor)) {
                        _add(_lchild(cur), floor, mid, line);
                    } else if (m_comp(p->m_line, line, ceil))
                        _add(_rchild(cur), mid + 1, ceil, line);
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Line line) {
                if (left <= floor && right >= ceil)
                    _add(cur, floor, ceil, line);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _add(_lchild(cur), floor, mid, left, right, line);
                    if (right > mid) _add(_rchild(cur), mid + 1, ceil, left, right, line);
                }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType i, Line line) {
                if (floor == ceil) {
                    if (m_comp(_ptr(cur)->m_line, line, i)) _ptr(cur)->m_line = line;
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _add(_lchild(cur), floor, mid, i, line);
                    else
                        _add(_rchild(cur), mid + 1, ceil, i, line);
                }
            }
            Line _query(size_type cur, SizeType floor, SizeType ceil, SizeType i) const {
                node *p = _ptr(cur);
                if (floor == ceil) return p->m_line;
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid) {
                    if (!p->m_lc) return p->m_line;
                    Line line = _query(p->m_lc, floor, mid, i);
                    return m_comp(p->m_line, line, i) ? line : p->m_line;
                } else {
                    if (!p->m_rc) return p->m_line;
                    Line line = _query(p->m_rc, mid + 1, ceil, i);
                    return m_comp(p->m_line, line, i) ? line : p->m_line;
                }
            }
            Tree(SizeType length = 0, Compare comp = Compare(), Line default_line = Line()) : m_comp(comp), m_default_line(default_line) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = _newnode();
            }
            void add(SizeType i, const Line &line) { _add(m_root, 0, m_size - 1, i, line); }
            void add(SizeType left, SizeType right, const Line &line) { _add(m_root, 0, m_size - 1, left, right, line); }
            Line query(SizeType i) const { return _query(m_root, 0, m_size - 1, i); }
        };
    }
    template <typename Tp, typename SizeType = uint64_t, LichaoSeg::size_type BUFFER = 1 << 20>
    using StaticLichaoSlopeSegTree = LichaoSeg::Tree<LichaoSeg::BaseLine<Tp>, LichaoSeg::BaseLess<LichaoSeg::BaseLine<Tp>>, SizeType, LichaoSeg::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeSegTree = LichaoSeg::Tree<LichaoSeg::BaseLine<Tp>, LichaoSeg::BaseLess<LichaoSeg::BaseLine<Tp>>, SizeType, LichaoSeg::VectorBuffer>;
}

#endif