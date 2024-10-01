/*
最后修改:
20240930
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ERASABLEMINMAXHEAP__
#define __OY_ERASABLEMINMAXHEAP__

#include "MinMaxHeap.h"

namespace OY {
    namespace EMMHeap {
        using size_type = MMHeap::size_type;
        template <typename Tp, typename Compare = std::less<Tp>>
        class Heap {
            mutable MMHeap::Heap<Tp, Compare> m_data, m_lazy;
        public:
            Heap() = default;
            template <typename InitMapping>
            Heap(size_type length, InitMapping mapping) : m_data(length, mapping) {}
            template <typename Iterator>
            Heap(Iterator first, Iterator last) : m_data(first, last) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) { m_data.resize(length, mapping), m_lazy.clear(); }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) { m_data.reset(first, last), m_lazy.clear(); }
            void reserve(size_type cap, size_type lazy_cap = 0) { m_data.reserve(cap), m_lazy.reserve(lazy_cap); }
            void clear() { m_data.clear(), m_lazy.clear(); }
            size_type size() const { return m_data.size() - m_lazy.size(); }
            bool empty() const { return m_data.size() == m_lazy.size(); }
            void push(const Tp &x) { m_data.push(x); }
            void erase(const Tp &x) { m_lazy.push(x); }
            void pop_min() {
                while (!m_lazy.empty() && !Compare()(m_data.top_min(), m_lazy.top_min())) m_data.pop_min(), m_lazy.pop_min();
                m_data.pop_min();
            }
            void pop_max() {
                while (!m_lazy.empty() && !Compare()(m_lazy.top_max(), m_data.top_max())) m_data.pop_max(), m_lazy.pop_max();
                m_data.pop_max();
            }
            const Tp &top_min() const {
                while (!m_lazy.empty() && !Compare()(m_data.top_min(), m_lazy.top_min())) m_data.pop_min(), m_lazy.pop_min();
                return m_data.top_min();
            }
            const Tp &top_max() const {
                while (!m_lazy.empty() && !Compare()(m_lazy.top_max(), m_data.top_max())) m_data.pop_max(), m_lazy.pop_max();
                return m_data.top_max();
            }
        };
    }
}

#endif