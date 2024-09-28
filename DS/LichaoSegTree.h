/*
最后修改:
20240920
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

#include "VectorBufferWithCollect.h"

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
        template <typename Line = BaseLine<double>, typename Compare = BaseLess, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using tree_type = Tree<Line, Compare, SizeType, BufferType>;
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
            size_type m_root{};
            SizeType m_size{};
            Line m_default_line;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static void _collect(size_type x) { _ptr(x)->m_lc = _ptr(x)->m_rc = 0, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            size_type _create() const {
                size_type c = _newnode();
                _ptr(c)->m_line = m_default_line;
                return c;
            }
            size_type _lchild(size_type cur) const {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _create();
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            size_type _rchild(size_type cur) const {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _create();
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, Line line) const {
                SizeType mid = (floor + ceil) >> 1;
                node *p = _ptr(cur);
                if (Compare()(p->m_line.calc(mid), line.calc(mid))) std::swap(p->m_line, line);
                if (floor < ceil)
                    if (Compare()(p->m_line.calc(floor), line.calc(floor))) {
                        _add(_lchild(cur), floor, mid, line);
                    } else if (Compare()(p->m_line.calc(ceil), line.calc(ceil)))
                        _add(_rchild(cur), mid + 1, ceil, line);
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Line line) const {
                if (left <= floor && right >= ceil)
                    _add(cur, floor, ceil, line);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _add(_lchild(cur), floor, mid, left, right, line);
                    if (right > mid) _add(_rchild(cur), mid + 1, ceil, left, right, line);
                }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType i, Line line) const {
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
            size_type _merge(size_type x, size_type y, SizeType floor, SizeType ceil) const {
                if (!x || !y) return x | y;
                if (floor != ceil) {
                    SizeType mid = (floor + ceil) >> 1;
                    size_type lc = _merge(_ptr(x)->m_lc, _ptr(y)->m_lc, floor, mid), rc = _merge(_ptr(x)->m_rc, _ptr(y)->m_rc, mid + 1, ceil);
                    _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc, _add(x, floor, ceil, _ptr(y)->m_line), _collect(y);
                } else if (Compare()(_ptr(x)->m_line.calc(floor), _ptr(y)->m_line.calc(floor)))
                    _ptr(x)->m_line = _ptr(y)->m_line, _collect(y);
                return x;
            }
        public:
            Tree() = default;
            Tree(SizeType length, Line default_line) { resize(length, default_line); }
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size), std::swap(m_default_line, rhs.m_default_line); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size), std::swap(m_default_line, rhs.m_default_line);
                return *this;
            }
            SizeType size() const { return m_size; }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0, m_size = 0;
            }
            void resize(SizeType length, Line default_line) {
                clear(), m_default_line = default_line;
                if (m_size = length) m_root = _create();
            }
            void add(SizeType i, const Line &line) { _add(m_root, 0, m_size - 1, i, line); }
            void add(SizeType left, SizeType right, const Line &line) { _add(m_root, 0, m_size - 1, left, right, line); }
            Line query(SizeType i) const { return _query(m_root, 0, m_size - 1, i); }
            void merge(tree_type &rhs) { m_root = _merge(m_root, rhs.m_root, 0, m_size - 1), rhs.m_root = rhs.m_size = 0; }
            void merge(tree_type &&rhs) { merge(rhs); }
        };
    }
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChmaxSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::less<Tp>, SizeType, VectorBufferWithCollect>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChminSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::greater<Tp>, SizeType, VectorBufferWithCollect>;
}

#endif