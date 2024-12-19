/*
最后修改:
20241201
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CHTHOLLYSEGTREE__
#define __OY_CHTHOLLYSEGTREE__

#include "AssignSegTree.h"

namespace OY {
    namespace ChthollySeg {
        using size_type = ASSEG::size_type;
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
            template <typename SizeType>
            static value_type pow(const value_type &x, SizeType n) { return x * n; }
        };
        template <typename SizeType, typename ValueType>
        struct RangeCollector {
            std::vector<std::tuple<SizeType, SizeType, ValueType>> m_data;
            template <typename... Args>
            void operator()(Args &&...args) { m_data.emplace_back(std::forward<Args>(args)...); }
        };
        template <typename SemiGroup, typename SizeType = uint64_t, size_type BATCH = 1 << 17, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = SemiGroup;
            using segment = typename group::value_type;
            using value_type = typename segment::value_type;
            using inner_type = ASSEG::Tree<group, SizeType, BATCH, BufferType>;
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
        private:
            inner_type m_inner_tree;
            static segment _make_segment(const value_type &val) { return segment(val); }
        public:
            Tree() = default;
            Tree(SizeType length, value_type init_val = value_type()) { resize(length, init_val); }
            void resize(SizeType length, value_type init_val = value_type()) { m_inner_tree.resize(length, _make_segment(init_val)); }
            SizeType size() const { return m_inner_tree.size(); }
            void modify(SizeType left, SizeType right, const value_type &val) { m_inner_tree.modify(left, right, _make_segment(val)); }
            void modify(SizeType i, const value_type &val) { m_inner_tree.modify(i, _make_segment(val)); }
            value_type query(SizeType i) const { return m_inner_tree.query(i).get(); }
            segment query(SizeType left, SizeType right) const { return m_inner_tree.query(left, right); }
            segment query_all() const { return m_inner_tree.query_all(); }
            std::pair<SizeType, SizeType> belong(SizeType i) const {
                SizeType l = m_inner_tree.min_left(i, [&](const segment &seg) { return !seg.has_change(); });
                SizeType r = m_inner_tree.max_right(i, [&](const segment &seg) { return !seg.has_change(); });
                return std::make_pair(l, r);
            }
            template <typename Callback>
            void enumerate(SizeType left, SizeType right, Callback &&call) {
                for (SizeType cur = left, next; cur != right + 1; cur = next) {
                    next = std::min(right, m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); })) + 1;
                    call(cur, next - 1, m_inner_tree.query(cur).get());
                }
            }
            template <typename Callback>
            void enumerate_all(Callback &&call) {
                for (SizeType cur = 0, next; cur != size(); cur = next) {
                    next = m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); }) + 1;
                    call(cur, next - 1, m_inner_tree.query(cur).get());
                }
            }
            std::vector<std::tuple<SizeType, SizeType, value_type>> get_ranges(SizeType left, SizeType right) {
                RangeCollector<SizeType, value_type> rc;
                enumerate(left, right, rc);
                return rc.m_data;
            }
            std::vector<std::tuple<SizeType, SizeType, value_type>> get_ranges_all() {
                RangeCollector<SizeType, value_type> rc;
                enumerate_all(rc);
                return rc.m_data;
            }
        };
        template <typename Ostream, typename SemiGroup, typename SizeType, size_type BATCH, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<SemiGroup, SizeType, BATCH, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollySegTree = ChthollySeg::Tree<ChthollySeg::ValLazySemiGroup<ChthollySeg::BaseSegment<Tp>>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyValLazyTree = ChthollySeg::Tree<ChthollySeg::ValLazySemiGroup<Segment>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyLazySegTree = ChthollySeg::Tree<ChthollySeg::LazySemiGroup<Segment>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyFastPowSegTree = ChthollySeg::Tree<ChthollySeg::FastPowSemiGroup<Segment>, SizeType, BATCH>;
}

#endif