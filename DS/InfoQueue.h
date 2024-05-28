/*
最后修改:
20240526
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_INFOQUEUE__
#define __OY_INFOQUEUE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace INFOQUEUE {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename Operation>
        struct InfoPair {
            using sum_type = Tp;
            Tp m_val, m_sum;
        };
        template <typename Tp>
        struct InfoPair<Tp, Ignore> {
            using sum_type = void;
            Tp m_val;
        };
        template <typename Tp>
        struct VectorAdapter : std::vector<Tp> {
            static constexpr bool is_special = false;
            const Tp &top() const { return std::vector<Tp>::back(); }
        };
        template <typename Tp, size_t N>
        struct StaticAdapter {
            static Tp s_buf[N];
            struct type1 {
                static constexpr bool is_special = true;
                Tp *m_l = s_buf, *m_r = s_buf;
                bool empty() const { return m_l == m_r; }
                size_type size() const { return m_r - m_l; }
                void pop_back() { m_l++; }
                const Tp &top() const { return *m_l; }
                Tp &operator[](size_type i) const { return *(m_r - i - 1); }
            };
            struct type2 {
                static constexpr bool is_special = true;
                Tp *m_l = s_buf, *m_r = s_buf;
                bool empty() const { return m_l == m_r; }
                size_type size() const { return m_r - m_l; }
                void push_back(const Tp &x) { *m_r++ = x; }
                const Tp &top() const { return *(m_r - 1); }
                const Tp &operator[](size_type i) const { return m_l[i]; }
            };
        };
        template <typename Tp, size_t N>
        Tp StaticAdapter<Tp, N>::s_buf[N];
        template <typename Tp, typename Operation = Ignore, typename Adapter1 = VectorAdapter<InfoPair<Tp, Operation>>, typename Adapter2 = VectorAdapter<InfoPair<Tp, Operation>>>
        struct Queue {
            static constexpr bool is_ignore = std::is_same<Operation, Ignore>::value;
            mutable Adapter1 m_left;
            mutable Adapter2 m_right;
            Operation m_op;
            void _trans() const {
                if constexpr (Adapter1::is_special) {
                    m_left.m_l = m_right.m_l, m_left.m_r = m_right.m_l = m_right.m_r;
                    if constexpr (!is_ignore) {
                        m_left[0].m_sum = m_left[0].m_val;
                        for (size_type i = 1, sz = m_left.size(); i != sz; i++) m_left[i].m_sum = m_op(m_left[i].m_val, m_left[i - 1].m_sum);
                    }
                } else {
                    if constexpr (is_ignore)
                        m_left.push_back({m_right.top().m_val});
                    else
                        m_left.push_back({m_right.top().m_val, m_right.top().m_val});
                    m_right.pop_back();
                    size_type sz = m_right.size();
                    while (sz--) {
                        if constexpr (is_ignore)
                            m_left.push_back({m_right.top().m_val});
                        else
                            m_left.push_back({m_right.top().m_val, m_op(m_right.top().m_val, m_left.top().m_sum)});
                        m_right.pop_back();
                    }
                }
            }
            Queue(Operation op = Operation()) : m_op(op) {}
            void push(const Tp &x) {
                if constexpr (is_ignore)
                    m_right.push_back({x});
                else if (m_right.empty())
                    m_right.push_back({x, x});
                else
                    m_right.push_back({x, m_op(m_right.top().m_sum, x)});
            }
            void pop() {
                if (m_left.empty()) _trans();
                m_left.pop_back();
            }
            const Tp &front() const {
                if (m_left.empty()) _trans();
                return m_left.top().m_val;
            }
            bool empty() const { return m_left.empty() && m_right.empty(); }
            size_type size() const { return m_left.size() + m_right.size(); }
            typename InfoPair<Tp, Operation>::sum_type query_all() const {
                static_assert(!is_ignore, "Operation Mustn't Be Ignore");
                if (m_left.empty())
                    return m_right.top().m_sum;
                else if (m_right.empty())
                    return m_left.top().m_sum;
                else
                    return m_op(m_left.top().m_sum, m_right.top().m_sum);
            }
            const Tp &operator[](size_type i) const { return i < m_left.size() ? m_left[m_left.size() - 1 - i].m_val : m_right[i - m_left.size()].m_val; }
        };
    }
    template <typename Tp, typename Operation, size_t N>
    using GlobalInfoQueue = INFOQUEUE::Queue<Tp, Operation, typename INFOQUEUE::StaticAdapter<INFOQUEUE::InfoPair<Tp, Operation>, N>::type1, typename INFOQUEUE::StaticAdapter<INFOQUEUE::InfoPair<Tp, Operation>, N>::type2>;
    template <typename Tp, typename Operation = INFOQUEUE::Ignore>
    using VectorInfoQueue = INFOQUEUE::Queue<Tp, Operation, INFOQUEUE::VectorAdapter<INFOQUEUE::InfoPair<Tp, Operation>>, INFOQUEUE::VectorAdapter<INFOQUEUE::InfoPair<Tp, Operation>>>;
}

#endif