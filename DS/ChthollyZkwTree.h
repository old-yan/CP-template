/*
最后修改:
20241201
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CHTHOLLYZKWTREE__
#define __OY_CHTHOLLYZKWTREE__

#include "AssignZkwTree.h"

namespace OY {
    namespace ChthollyZkw {
        using size_type = ASZKW::size_type;
        template <typename Tp>
        struct BaseSegment {
            using value_type = Tp;
            Tp m_front, m_back;
            bool m_change;
            BaseSegment() = default;
            explicit BaseSegment(const Tp &val) : m_front(val), m_back(val), m_change(false) {}
            BaseSegment(const Tp &front, const Tp &back, bool change) : m_front(front), m_back(back), m_change(change) {}
            BaseSegment<Tp> operator+(const BaseSegment<Tp> &rhs) const { return BaseSegment<Tp>(m_front, rhs.m_back, m_change || rhs.m_change || m_back != rhs.m_front); }
            bool has_change() const { return m_change; }
            Tp get() const { return m_front; }
        };
        template <typename Segment>
        struct ValLazySemiGroup {
            using value_type = Segment;
            static constexpr bool val_is_lazy = true;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        template <typename Segment>
        struct LazySemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        template <typename Segment>
        struct FastPowSemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type pow(const value_type &x, size_type n) { return x * n; }
        };
        template <typename SemiGroup, size_type BATCH = 1 << 17>
        class Tree {
        public:
            using group = SemiGroup;
            using segment = typename group::value_type;
            using value_type = typename segment::value_type;
            using inner_type = ASZKW::Tree<group, BATCH>;
        private:
            inner_type m_inner_tree;
            static segment _make_segment(const value_type &val) { return segment(val); }
        public:
            Tree() = default;
            Tree(size_type length, value_type init_val = value_type()) { resize(length, init_val); }
            void resize(size_type length, value_type init_val = value_type()) { m_inner_tree.resize(length, _make_segment(init_val)); }
            size_type size() const { return m_inner_tree.size(); }
            void modify(size_type left, size_type right, const value_type &val) { m_inner_tree.modify(left, right, _make_segment(val)); }
            void modify(size_type i, const value_type &val) { m_inner_tree.modify(i, _make_segment(val)); }
            value_type query(size_type i) const { return m_inner_tree.query(i).get(); }
            segment query(size_type left, size_type right) const { return m_inner_tree.query(left, right); }
            segment query_all() const { return m_inner_tree.query_all(); }
            std::pair<size_type, size_type> belong(size_type i) const {
                size_type l = m_inner_tree.min_left(i, [&](const segment &seg) { return !seg.has_change(); });
                size_type r = m_inner_tree.max_right(i, [&](const segment &seg) { return !seg.has_change(); });
                return std::make_pair(l, r);
            }
            template <typename Callback>
            void enumerate(size_type left, size_type right, Callback &&call) {
                for (size_type cur = left, next; cur != right + 1; cur = next) {
                    next = std::min(right, m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); })) + 1;
                    call(cur, next - 1, m_inner_tree.query(cur).get());
                }
            }
            template <typename Callback>
            void enumerate_all(Callback &&call) {
                for (size_type cur = 0, next; cur != size(); cur = next) {
                    next = m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); }) + 1;
                    call(cur, next - 1, m_inner_tree.query(cur).get());
                }
            }
        };
        template <typename Ostream, typename SemiGroup, size_type BATCH>
        Ostream &operator<<(Ostream &out, const Tree<SemiGroup, BATCH> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyZkwTree = ChthollyZkw::Tree<ChthollyZkw::ValLazySemiGroup<ChthollyZkw::BaseSegment<Tp>>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyValLazyZkwTree = ChthollyZkw::Tree<ChthollyZkw::ValLazySemiGroup<Segment>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyLazyZkwTree = ChthollyZkw::Tree<ChthollyZkw::LazySemiGroup<Segment>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyFastPowZkwTree = ChthollyZkw::Tree<ChthollyZkw::FastPowSemiGroup<Segment>, BATCH>;
}

#endif