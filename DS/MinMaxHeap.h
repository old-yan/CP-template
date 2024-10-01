/*
最后修改:
20240929
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MINMAXHEAP__
#define __OY_MINMAXHEAP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace MMHeap {
        using size_type = uint32_t;
        template <typename Tp, typename Compare = std::less<Tp>>
        class Heap {
            std::vector<Tp> m_vec;
            static constexpr size_type _parent(size_type k) { return ((k >> 1) - 1) & ~1; }
            void _make_heap() {
                auto p = m_vec.data();
                for (size_type i = m_vec.size(); i--;) {
                    if ((i & 1) && Compare()(p[i - 1], p[i])) std::swap(p[i - 1], p[i]);
                    _sift_up(_sift_down(i), i);
                }
            }
            size_type _sift_down(size_type k) {
                size_type n = m_vec.size();
                auto p = m_vec.data();
                if (k & 1)
                    while (k * 2 + 1 < n) {
                        size_type c = k * 2 + 3;
                        if (c >= n || Compare()(p[c - 2], p[c])) c -= 2;
                        if (c >= n || !Compare()(p[c], p[k])) break;
                        std::swap(p[k], p[c]), k = c;
                    }
                else
                    while (k * 2 + 2 < n) {
                        size_type c = k * 2 + 4;
                        if (c >= n || Compare()(p[c], p[c - 2])) c -= 2;
                        if (c >= n || !Compare()(p[k], p[c])) break;
                        std::swap(p[k], p[c]), k = c;
                    }
                return k;
            }
            void _sift_up(size_type k, size_type rt) {
                size_type n = m_vec.size();
                auto p = m_vec.data();
                if ((k | 1) < n && Compare()(p[k & ~1], p[k | 1])) std::swap(p[k & ~1], p[k | 1]), k ^= 1;
                while (rt < k) {
                    size_type fa = _parent(k);
                    if (!Compare()(p[fa], p[k])) break;
                    std::swap(p[fa], p[k]), k = fa;
                }
                while (rt < k) {
                    size_type fa = _parent(k) | 1;
                    if (!Compare()(p[k], p[fa])) break;
                    std::swap(p[fa], p[k]), k = fa;
                }
            }
        public:
            Heap() = default;
            template <typename InitMapping>
            Heap(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Heap(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_vec.clear(), m_vec.reserve(length);
                for (size_type i = 0; i != length; i++) m_vec.push_back(mapping(i));
                _make_heap();
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type cap) { m_vec.reserve(cap); }
            void clear() { m_vec.clear(); }
            size_type size() const { return m_vec.size(); }
            bool empty() const { return m_vec.empty(); }
            void push(const Tp &x) {
                m_vec.push_back(x);
                _sift_up(m_vec.size() - 1, 1);
            }
            void pop_min() {
                if (m_vec.size() <= 2) return m_vec.pop_back();
                std::swap(m_vec[1], m_vec.back());
                m_vec.pop_back();
                _sift_up(_sift_down(1), 1);
            }
            void pop_max() {
                if (m_vec.size() == 1) return m_vec.pop_back();
                std::swap(m_vec[0], m_vec.back());
                m_vec.pop_back();
                _sift_up(_sift_down(0), 1);
            }
            const Tp &top_min() const { return m_vec.size() == 1 ? m_vec[0] : m_vec[1]; }
            const Tp &top_max() const { return m_vec[0]; }
        };
    }
}

#endif