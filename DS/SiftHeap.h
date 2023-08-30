/*
最后修改:
20230830
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SIFTHEAP__
#define __OY_SIFTHEAP__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace Sift {
        using size_type = uint32_t;
        template <typename Sequence>
        struct Getter;
        template <typename Tp>
        struct Getter<std::vector<Tp>> {
            std::vector<Tp> &m_sequence;
            Getter(std::vector<Tp> &sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index]; }
        };
        template <typename Tp>
        struct Getter<Tp *> {
            Tp *m_sequence;
            Getter(Tp *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return *(m_sequence + index); }
        };
        template <typename Mapping, typename Compare = std::less<void>>
        struct Heap {
            std::vector<size_type> m_heap, m_pos;
            Mapping m_map;
            Compare m_comp;
            Heap(size_type length, Mapping map, Compare comp = Compare()) : m_pos(length, -1), m_map(map), m_comp(comp) { m_heap.reserve(length); }
            void sift_up(size_type i) {
                size_type curpos = m_pos[i], p;
                auto &&curvalue = m_map(i);
                for (; curpos; curpos = (curpos - 1) >> 1) {
                    if (!m_comp(m_map(p = m_heap[(curpos - 1) >> 1]), curvalue)) break;
                    m_heap[m_pos[p] = curpos] = p;
                }
                m_heap[m_pos[i] = curpos] = i;
            }
            void sift_down(size_type i) {
                size_type curpos = m_pos[i], c;
                auto &&curvalue = m_map(i);
                for (; (c = curpos * 2 + 1) < m_heap.size(); curpos = c) {
                    if (c + 1 < m_heap.size() && m_comp(m_map(m_heap[c]), m_map(m_heap[c + 1]))) c++;
                    if (!m_comp(curvalue, m_map(m_heap[c]))) break;
                    m_pos[m_heap[curpos] = m_heap[c]] = curpos;
                }
                m_heap[m_pos[i] = curpos] = i;
            }
            void push(size_type i) {
                if (!~m_pos[i]) {
                    m_pos[i] = m_heap.size();
                    m_heap.push_back(i);
                }
                sift_up(i);
            }
            size_type top() const { return m_heap.front(); }
            void pop() {
                m_pos[m_heap.front()] = -1;
                if (m_heap.size() > 1) {
                    m_pos[m_heap.back()] = 0;
                    m_heap.front() = m_heap.back();
                    m_heap.pop_back();
                    sift_down(m_heap.front());
                } else
                    m_heap.pop_back();
            }
            bool empty() const { return m_heap.empty(); }
            size_type size() const { return m_heap.size(); }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = Sift::Heap<Sift::Getter<std::vector<Tp>>, Compare>>
    auto make_SiftHeap(Sift::size_type length, std::vector<Tp> &items, Compare comp = Compare()) -> HeapType { return HeapType(length, Sift::Getter<std::vector<Tp>>(items), comp); }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = Sift::Heap<Sift::Getter<Tp *>, Compare>>
    auto make_SiftHeap(Sift::size_type length, Tp *items, Compare comp = Compare()) -> HeapType { return HeapType(length, Sift::Getter<Tp *>(items), comp); }
    template <typename Sequence>
    using SiftGetter = Sift::Getter<Sequence>;
    template <typename Mapping, typename Compare = std::less<void>>
    using SiftHeap = Sift::Heap<Mapping, Compare>;
}

#endif