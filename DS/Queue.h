/*
最后修改:
20240911
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_QUEUE__
#define __OY_QUEUE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace QUE {
        using size_type = uint32_t;
        template <typename Tp>
        struct NoOp {
            using value_type = Tp;
        };
        template <typename ValueType, typename SumType, typename Operation>
        struct BaseSemiGroup {
            using value_type = ValueType;
            using sum_type = SumType;
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
        template <typename ValueType, typename SumType>
        struct InfoPair {
            ValueType m_val;
            SumType m_sum;
        };
        template <typename ValueType>
        struct InfoPair<ValueType, void> {
            ValueType m_val;
        };
        template <typename Tp>
        struct VectorContainer {
            static constexpr bool is_global = false;
            struct type : std::vector<Tp> {
                const Tp &top() const { return std::vector<Tp>::back(); }
            };
            using type1 = type;
            using type2 = type;
        };
        template <size_t N>
        struct GlobalContainer {
            template <typename Tp>
            struct type {
                static constexpr bool is_global = true;
                static Tp s_buf[N];
                struct type1 {
                    Tp *m_l = s_buf, *m_r = s_buf;
                    bool empty() const { return m_l == m_r; }
                    size_type size() const { return m_r - m_l; }
                    void pop_back() { m_l++; }
                    const Tp &top() const { return *m_l; }
                    Tp &operator[](size_type i) const { return *(m_r - i - 1); }
                };
                struct type2 {
                    Tp *m_l = s_buf, *m_r = s_buf;
                    bool empty() const { return m_l == m_r; }
                    size_type size() const { return m_r - m_l; }
                    void push_back(const Tp &x) { *m_r++ = x; }
                    const Tp &top() const { return *(m_r - 1); }
                    const Tp &operator[](size_type i) const { return m_l[i]; }
                };
            };
        };
        template <size_t N>
        template <typename Tp>
        Tp GlobalContainer<N>::type<Tp>::s_buf[N];
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
        template <typename SemiGroup, template <typename> typename Container = VectorContainer>
        class Queue {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
            using sum_type = typename Has_Sum_Type<group, value_type>::type;
            static constexpr bool has_op = Has_Op<group, sum_type>::value;
            using info_type = InfoPair<value_type, typename std::conditional<has_op, sum_type, void>::type>;
            using container_type = Container<info_type>;
        private:
            mutable typename container_type::type1 m_left;
            mutable typename container_type::type2 m_right;
            void _trans() const {
                if constexpr (container_type::is_global) {
                    m_left.m_l = m_right.m_l, m_left.m_r = m_right.m_l = m_right.m_r;
                    if constexpr (has_op) {
                        m_left[0].m_sum = m_left[0].m_val;
                        for (size_type i = 1, sz = m_left.size(); i != sz; i++) m_left[i].m_sum = group::op(m_left[i].m_val, m_left[i - 1].m_sum);
                    }
                } else {
                    if constexpr (has_op)
                        m_left.push_back({m_right.top().m_val, sum_type(m_right.top().m_val)});
                    else
                        m_left.push_back({m_right.top().m_val});
                    m_right.pop_back();
                    size_type sz = m_right.size();
                    while (sz--) {
                        if constexpr (has_op)
                            m_left.push_back({m_right.top().m_val, group::op(m_right.top().m_val, m_left.top().m_sum)});
                        else
                            m_left.push_back({m_right.top().m_val});
                        m_right.pop_back();
                    }
                }
            }
        public:
            void push(value_type x) {
                if constexpr (!has_op)
                    m_right.push_back({x});
                else if (m_right.empty())
                    m_right.push_back({x, sum_type(x)});
                else
                    m_right.push_back({x, group::op(m_right.top().m_sum, x)});
            }
            void pop() {
                if (m_left.empty()) _trans();
                m_left.pop_back();
            }
            const value_type &front() const {
                if (m_left.empty()) _trans();
                return m_left.top().m_val;
            }
            const value_type &back() const { return m_right.empty() ? m_left[0].m_val : m_right.top().m_val; }
            bool empty() const { return m_left.empty() && m_right.empty(); }
            size_type size() const { return m_left.size() + m_right.size(); }
            sum_type query_all() const {
                if (m_left.empty())
                    return m_right.top().m_sum;
                else if (m_right.empty())
                    return m_left.top().m_sum;
                else
                    return group::op(m_left.top().m_sum, m_right.top().m_sum);
            }
            const value_type &operator[](size_type i) const { return i < m_left.size() ? m_left[m_left.size() - 1 - i].m_val : m_right[i - m_left.size()].m_val; }
        };
        template <typename Ostream, typename SemiGroup>
        Ostream &operator<<(Ostream &out, const Queue<SemiGroup> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x[i];
            }
            return out << "]";
        }
    }
    template <typename SemiGroup, size_t N>
    using GlobalQueue = QUE::Queue<SemiGroup, QUE::GlobalContainer<N>::template type>;
    template <typename SemiGroup>
    using VectorQueue = QUE::Queue<SemiGroup, QUE::VectorContainer>;
    template <typename Tp>
    using Queue = QUE::Queue<QUE::NoOp<Tp>>;
    template <typename Tp>
    using MaxQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, QUE::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp>
    using MinQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, QUE::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using GcdQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, QUE::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using LcmQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, QUE::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp>
    using BitAndQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, std::bit_and<Tp>>>;
    template <typename Tp>
    using BitOrQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, std::bit_or<Tp>>>;
    template <typename Tp>
    using BitXorQueue = QUE::Queue<QUE::BaseSemiGroup<Tp, Tp, std::bit_xor<Tp>>>;
    template <typename ValueType, typename SumType = ValueType>
    using SumQueue = QUE::Queue<QUE::BaseSemiGroup<ValueType, SumType, std::plus<SumType>>>;
}

#endif