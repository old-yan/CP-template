/*
最后修改:
20240915
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

#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace LCSEG {
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
        template <typename Line = BaseLine<double>, typename Compare = BaseLess, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using value_type = decltype(std::declval<Line>().calc(0));
            struct node {
                Line m_line;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_root;
            SizeType m_size;
            Line m_default_line;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
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
                if (Compare()(p->m_line.calc(mid), line.calc(mid))) std::swap(p->m_line, line);
                if (floor < ceil)
                    if (Compare()(p->m_line.calc(floor), line.calc(floor))) {
                        _add(_lchild(cur), floor, mid, line);
                    } else if (Compare()(p->m_line.calc(ceil), line.calc(ceil)))
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
                    if (Compare()(_ptr(cur)->m_line.calc(i), line.calc(i))) _ptr(cur)->m_line = line;
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
                    return Compare()(p->m_line.calc(i), line.calc(i)) ? line : p->m_line;
                } else {
                    if (!p->m_rc) return p->m_line;
                    Line line = _query(p->m_rc, mid + 1, ceil, i);
                    return Compare()(p->m_line.calc(i), line.calc(i)) ? line : p->m_line;
                }
            }
        public:
            Tree(SizeType length = 0, Line default_line = Line()) : m_default_line(default_line) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = _newnode();
            }
            void add(SizeType i, const Line &line) { _add(m_root, 0, m_size - 1, i, line); }
            void add(SizeType left, SizeType right, const Line &line) { _add(m_root, 0, m_size - 1, left, right, line); }
            Line query(SizeType i) const { return _query(m_root, 0, m_size - 1, i); }
        };
    }
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChmaxSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::less<Tp>, SizeType, VectorBufferWithoutCollect>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChminSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::greater<Tp>, SizeType, VectorBufferWithoutCollect>;
}

#endif