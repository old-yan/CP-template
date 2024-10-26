/*
最后修改:
20241020
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ERASABLEMONOPAIRHEAP__
#define __OY_ERASABLEMONOPAIRHEAP__

#include "MonoPairHeap.h"

namespace OY {
    namespace EMONOPH {
        using size_type = MONOPH::size_type;
        template <typename Tp>
        struct AddCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp inverse(const Tp &x) { return -x; }
        };
        template <typename Tp>
        struct BitxorCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp inverse(const Tp &x) { return x; }
        };
        template <typename CommutativeGroup>
        struct Trait {
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
            using sum_type = typename MONOPH::Has_Sum_Type<group, value_type>::type;
            static constexpr bool has_op = MONOPH::Has_Op<group, sum_type>::value;
        };
        template <typename CommutativeGroup, bool HasOp = Trait<CommutativeGroup>::has_op>
        struct HeapBase : Trait<CommutativeGroup> {
            typename Trait<CommutativeGroup>::sum_type m_prod;
            HeapBase() : m_prod(CommutativeGroup::identity()) {}
        };
        template <typename CommutativeGroup>
        struct HeapBase<CommutativeGroup, false> : Trait<CommutativeGroup> {};
        template <typename CommutativeGroup, typename Compare = std::less<typename CommutativeGroup::value_type>, template <typename> typename BufferType = VectorBufferWithCollect>
        class Heap : HeapBase<CommutativeGroup> {
        public:
            using typename HeapBase<CommutativeGroup>::group;
            using typename HeapBase<CommutativeGroup>::value_type;
            using typename HeapBase<CommutativeGroup>::sum_type;
            using HeapBase<CommutativeGroup>::has_op;
            using heap_type = Heap<CommutativeGroup, Compare, BufferType>;
            using inner_type = MONOPH::Heap<MONOPH::NoOp<value_type>, Compare, BufferType>;
            using buffer_type = typename inner_type::buffer_type;
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
        private:
            mutable inner_type m_data, m_lazy;
            size_type m_sz;
        public:
            Heap() : m_sz() {}
            void clear() {
                m_data.clear(), m_lazy.clear(), m_sz = 0;
                if constexpr (has_op) this->m_prod = group::identity();
            }
            size_type size() const { return m_sz; }
            bool empty() const { return !m_sz; }
            void push(const value_type &x) {
                m_data.push(x), m_sz++;
                if constexpr (has_op) this->m_prod = group::op(this->m_prod, x);
            }
            void erase(const value_type &x) {
                m_lazy.push(x), m_sz--;
                if constexpr (has_op) this->m_prod = group::op(this->m_prod, group::inverse(x));
            }
            void pop() {
                while (!m_lazy.empty() && !Compare()(m_lazy.top(), m_data.top())) m_data.pop(), m_lazy.pop();
                if constexpr (has_op) this->m_prod = group::op(this->m_prod, group::inverse(m_data.top()));
                m_data.pop(), m_sz--;
            }
            const value_type &top() const {
                while (!m_lazy.empty() && !Compare()(m_lazy.top(), m_data.top())) m_data.pop(), m_lazy.pop();
                return m_data.top();
            }
            void join(heap_type &rhs) {
                if (!rhs.empty()) {
                    m_data.join(rhs.m_data), m_lazy.join(rhs.m_lazy), rhs.m_sz = 0;
                    if constexpr (has_op) this->m_prod = group::op(this->m_prod, rhs.m_prod), rhs.m_prod = group::identity();
                }
            }
            void join(heap_type &&rhs) { join(rhs); }
            const sum_type &query_all() const { return this->m_prod; }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect>
    using ErasableMonoPairHeap = EMONOPH::Heap<MONOPH::NoOp<Tp>, Compare, BufferType>;
    template <typename Tp, typename Compare = std::less<Tp>>
    using VectorErasableMonoSumPairHeap = EMONOPH::Heap<EMONOPH::AddCommutativeGroup<Tp>, Compare>;
    template <typename Tp, typename Compare = std::less<Tp>>
    using VectorErasableMonoBitXorPairHeap = EMONOPH::Heap<EMONOPH::BitxorCommutativeGroup<Tp>, Compare>;
}

#endif