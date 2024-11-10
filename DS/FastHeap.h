/*
最后修改:
20241027
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FASTHEAP__
#define __OY_FASTHEAP__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace FHeap {
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
            void _set(size_type x, size_type pos) { m_pos[m_heap[pos] = x] = pos; }
        public:
            Heap(size_type length, Mapping map, Compare comp = Compare()) : m_map(map), m_comp(comp) { resize(length); }
            void resize(size_type length) { m_heap.resize(length * 2), m_pos.assign(length, -1), m_size = 0; }
            void insert(size_type x) {
                _set(x, m_size * 2 + 1);
                if (m_size) {
                    _set(m_heap[m_size], m_size * 2);
                    auto &&cur_value = m_map(x);
                    for (size_type pos = m_size * 2; (pos >>= 1) && m_comp(m_map(m_heap[pos]), cur_value); m_heap[pos] = x) {}
                }
                m_size++;
            }
            void sift_up(size_type x) {
                auto &&cur_value = m_map(x);
                size_type pos = m_pos[x];
                while ((pos >>= 1) && (m_heap[pos] == x || m_comp(m_map(m_heap[pos]), cur_value))) m_heap[pos] = x;
            }
            void sift_down(size_type x) {
                auto &&cur_value = m_map(x);
                size_type pos = m_pos[x];
                while ((pos >>= 1) && m_heap[pos] == x) m_heap[pos] = std::max(m_heap[pos * 2], m_heap[pos * 2 + 1], [&](size_type x, size_type y) { return m_comp(m_map(x), m_map(y)); });
            }
            void clear() {
                std::fill_n(m_pos.data(), m_pos.size(), -1);
                m_size = 0;
            }
            void push(size_type x) { contains(x) ? sift_up(x) : insert(x); }
            size_type top() const { return m_heap[1]; }
            void pop() {
                size_type x = m_heap[1], pos = m_pos[x];
                m_pos[x] = -1;
                if (--m_size) {
                    if ((pos >> 1) == m_size)
                        _set(m_heap[pos ^ 1], m_size), pos = m_size;
                    else
                        _set(m_heap[m_size] == m_heap[m_size * 2] ? m_heap[m_size * 2 + 1] : m_heap[m_size * 2], pos), m_pos[m_heap[m_size]] = m_size;
                    while (pos >>= 1) m_heap[pos] = std::max(m_heap[pos * 2], m_heap[pos * 2 + 1], [&](size_type x, size_type y) { return m_comp(m_map(x), m_map(y)); });
                }
            }
            bool empty() const { return !m_size; }
            size_type size() const { return m_size; }
            bool contains(size_type x) const { return ~m_pos[x]; }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = FHeap::Heap<FHeap::Getter<std::vector<Tp>>, Compare>>
    auto make_FastHeap(FHeap::size_type length, std::vector<Tp> &items, Compare comp = Compare()) -> HeapType { return HeapType(length, FHeap::Getter<std::vector<Tp>>(items), comp); }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = FHeap::Heap<FHeap::Getter<Tp *>, Compare>>
    auto make_FastHeap(FHeap::size_type length, Tp *items, Compare comp = Compare()) -> HeapType { return HeapType(length, FHeap::Getter<Tp *>(items), comp); }
    template <typename Mapping, typename Compare = std::less<void>>
    using FastHeap = FHeap::Heap<Mapping, Compare>;
}

#endif