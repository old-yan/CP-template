/*
最后修改:
20241015
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOPAIRHEAP__
#define __OY_MONOPAIRHEAP__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace MONOPH {
        using size_type = uint32_t;
        struct VoidInfo {};
        template <typename Tp>
        struct NoOp {
            using value_type = Tp;
        };
        template <typename ValueType, typename SumType, ValueType Identity, typename Operation>
        struct BaseMonoid {
            using value_type = ValueType;
            using sum_type = SumType;
            static constexpr value_type identity() { return Identity; }
            static sum_type op(const sum_type &x, const sum_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeMonoid, typename value_type, typename sum_type>
        struct NodeWrapper {
            template <typename Node>
            struct type {
                value_type m_val;
                sum_type m_sum;
                const sum_type &get_sum() const { return m_sum; }
                void pushup(Node *lchild, Node *rchild) { m_sum = CommutativeMonoid::op(CommutativeMonoid::op(lchild->m_sum, m_val), rchild->m_sum); }
            };
        };
        template <typename CommutativeMonoid, typename value_type>
        struct NodeWrapper<CommutativeMonoid, value_type, void> {
            template <typename Node>
            struct type {
                value_type m_val;
            };
        };
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Sum_Type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_Sum_Type<Tp, ValueType, void_t<typename Tp::sum_type>> : std::true_type {
            using type = typename Tp::sum_type;
        };
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename CommutativeMonoid, typename Compare = std::less<typename CommutativeMonoid::value_type>, template <typename> typename BufferType = VectorBufferWithCollect>
        class Heap {
        public:
            using group = CommutativeMonoid;
            using heap_type = Heap<CommutativeMonoid, Compare, BufferType>;
            using value_type = typename group::value_type;
            using sum_type = typename Has_Sum_Type<group, value_type>::type;
            static constexpr bool has_op = Has_Op<group, sum_type>::value;
            struct node : NodeWrapper<group, value_type, typename std::conditional<has_op, sum_type, void>::type>::template type<node> {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() { return _ptr(m_lc); }
                node *rchild() { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            struct Initializer {
                Initializer() {
                    if constexpr (has_op) _ptr(0)->m_val = group::identity(), _ptr(0)->m_sum = group::identity();
                }
            };
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _collect(size_type x) { *_ptr(x) = node{}, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            static size_type _newnode(const value_type &val) {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_val = val;
                if constexpr (has_op) _ptr(x)->m_sum = val;
                return x;
            }
            static void _pushup(node *p) {
                if constexpr (has_op) p->pushup(p->lchild(), p->rchild());
            }
            static size_type _merge(size_type x, size_type y) {
                node *p = _ptr(x), *q = _ptr(y);
                if (Compare()(p->m_val, q->m_val)) std::swap(x, y), std::swap(p, q);
                if (p->m_lc) q->m_rc = p->m_lc, _pushup(q);
                p->m_lc = y, _pushup(p);
                return x;
            }
            static size_type _merges(size_type x) {
                node *p = _ptr(x);
                if (!x || !p->m_rc) return x;
                size_type a = p->m_rc, b = _ptr(a)->m_rc;
                p->m_rc = _ptr(a)->m_rc = 0;
                _pushup(p), _pushup(_ptr(a));
                return b ? _merge(_merge(x, a), _merges(b)) : _merge(x, a);
            }
        public:
            Heap() { static Initializer _init; }
            Heap(const heap_type &rhs) = delete;
            Heap(heap_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt); }
            ~Heap() { clear(); }
            heap_type &operator=(const heap_type &rhs) = delete;
            heap_type &operator=(heap_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt);
                return *this;
            }
            const node *root() const { return _ptr(m_rt); }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            bool empty() const { return !m_rt; }
            void push(const value_type &val) { m_rt = m_rt ? _merge(m_rt, _newnode(val)) : _newnode(val); }
            const value_type &top() const { return root()->m_val; }
            void pop() {
                size_type tmp = m_rt;
                m_rt = _merges(root()->m_lc), _collect(tmp);
            }
            void join(heap_type &rhs) {
                if (!rhs.empty()) m_rt = m_rt ? _merge(m_rt, rhs.m_rt) : rhs.m_rt, rhs.m_rt = 0;
            }
            void join(heap_type &&rhs) { join(rhs); }
            sum_type query_all() const { return root()->m_sum; }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect>
    using MonoPairHeap = MONOPH::Heap<MONOPH::NoOp<Tp>, Compare, BufferType>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename Compare = std::less<Tp>>
    using VectorMonoMaxPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, Minimum, MONOPH::ChoiceByCompare<Tp, std::less<Tp>>>, Compare>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename Compare = std::less<Tp>>
    using VectorMonoMinPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, Maximum, MONOPH::ChoiceByCompare<Tp, std::greater<Tp>>>, Compare>;
    template <typename Tp, typename Compare = std::less<Tp>>
    using VectorMonoGcdPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, 0, MONOPH::FpTransfer<Tp, std::gcd<Tp>>>, Compare>;
    template <typename Tp, typename Compare = std::less<Tp>>
    using VectorMonoLcmPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, 1, MONOPH::FpTransfer<Tp, std::lcm<Tp>>>, Compare>;
    template <typename Tp, Tp OneMask = Tp(-1), typename Compare = std::less<Tp>>
    using VectorMonoBitAndPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>>, Compare>;
    template <typename Tp, Tp ZeroMask = 0, typename Compare = std::less<Tp>>
    using VectorMonoBitOrPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>>, Compare>;
    template <typename Tp, Tp ZeroMask = 0, typename Compare = std::less<Tp>>
    using VectorMonoBitXorPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>>, Compare>;
    template <typename ValueType, typename SumType = ValueType, ValueType Zero = ValueType(), typename Compare = std::less<ValueType>>
    using VectorMonoSumPairHeap = MONOPH::Heap<MONOPH::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>>, Compare>;
}

#endif