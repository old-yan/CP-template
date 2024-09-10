/*
最后修改:
20240424
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
        class Heap {
            std::vector<size_type> m_heap, m_pos;
            size_type m_size;
            Mapping m_map;
            Compare m_comp;
        public:
            Heap(size_type length, Mapping map, Compare comp = Compare()) : m_map(map), m_comp(comp) { resize(length); }
            void resize(size_type length) {
                m_heap.resize(length), m_pos.assign(length, -1);
                m_size = 0;
            }
            void sift_up(size_type i) {
                size_type *pos = m_pos.data(), *heap = m_heap.data();
                size_type curpos = pos[i], p;
                auto &&curvalue = m_map(i);
                for (; curpos; curpos = (curpos - 1) >> 1) {
                    if (!m_comp(m_map(p = heap[(curpos - 1) >> 1]), curvalue)) break;
                    heap[pos[p] = curpos] = p;
                }
                heap[pos[i] = curpos] = i;
            }
            void sift_down(size_type i) {
                size_type *pos = m_pos.data(), *heap = m_heap.data();
                size_type curpos = pos[i], c;
                auto &&curvalue = m_map(i);
                for (; (c = curpos * 2 + 1) < m_size; curpos = c) {
                    if (c + 1 < m_size && m_comp(m_map(heap[c]), m_map(heap[c + 1]))) c++;
                    if (!m_comp(curvalue, m_map(heap[c]))) break;
                    pos[heap[curpos] = heap[c]] = curpos;
                }
                heap[pos[i] = curpos] = i;
            }
            void clear() {
                std::fill_n(m_pos.data(), m_pos.size(), -1);
                m_size = 0;
            }
            void push(size_type i) {
                if (!~m_pos[i]) {
                    m_pos[i] = m_size;
                    m_heap[m_size++] = i;
                }
                sift_up(i);
            }
            size_type top() const { return m_heap[0]; }
            void pop() {
                m_pos[m_heap[0]] = -1;
                if (--m_size) {
                    m_pos[m_heap[m_size]] = 0;
                    m_heap[0] = m_heap[m_size];
                    sift_down(m_heap[0]);
                }
            }
            bool empty() const { return !m_size; }
            size_type size() const { return m_size; }
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