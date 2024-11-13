/*
最后修改:
20241111
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LINKBUCKETHEAP__
#define __OY_LINKBUCKETHEAP__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LBHeap {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
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
        template <size_type Cap, typename Mapping, bool SmallCap>
        class HeapBase {
            static constexpr size_type w = (Cap + MASK_SIZE - 1) >> MASK_WIDTH;
            struct link {
                size_type m_prev, m_next;
                void set(size_type x) { m_prev = m_next = x; }
            };
            mask_type m_masks[w];
            size_type m_size;
            mutable size_type m_min;
            std::vector<link> m_link;
            Mapping m_map;
        public:
            HeapBase(size_type length, Mapping map) : m_map(map) { resize(length); }
            void resize(size_type length) {
                std::fill_n(m_masks, w, 0);
                m_size = m_min = 0;
                m_link.resize(Cap + length);
                for (size_type i = 0; i != Cap; i++) m_link[i].set(i);
                for (size_type i = 0; i != length; i++) m_link[Cap + i].set(-1);
            }
            void insert(size_type x) {
                size_type val = m_map(x), pos = val % Cap;
                if (val >= m_min + Cap) m_min = val - Cap + 1;
                if (m_link[pos].m_next == pos) {
                    m_masks[pos >> MASK_WIDTH] |= mask_type(1) << (pos & (MASK_SIZE - 1)), m_size++;
                    m_link[pos].set(Cap + x), m_link[Cap + x].set(pos);
                } else
                    m_link[m_link[Cap + x].m_prev = m_link[pos].m_prev].m_next = Cap + x, m_link[Cap + x].m_next = pos, m_link[pos].m_prev = Cap + x;
            }
            void sift_up(size_type x) {
                size_type prev = m_link[Cap + x].m_prev, next = m_link[Cap + x].m_next;
                if (prev == next) {
                    size_type pos = prev;
                    m_masks[pos >> MASK_WIDTH] -= mask_type(1) << (pos & (MASK_SIZE - 1)), m_size--, m_link[pos].set(pos);
                } else
                    m_link[next].m_prev = prev, m_link[prev].m_next = next;
                insert(x);
            }
            void clear() {
                std::fill_n(m_masks, w, 0);
                m_size = m_min = 0;
                for (size_type i = 0; i != Cap; i++) m_link[i].m_prev = i;
                for (size_type i = 0, len = m_link.size() - Cap; i != len; i++) m_link[Cap + i].set(-1);
            }
            void push(size_type x) { (~m_link[Cap + x].m_next) ? sift_up(x) : insert(x); }
            bool empty() const { return !m_size; }
            size_type size() const { return m_size; }
            size_type top() const {
                size_type i = m_min % Cap, j = i >> MASK_WIDTH;
                if (m_masks[j] >> (i & (MASK_SIZE - 1)) & 1) return m_link[i].m_next - Cap;
                if (m_masks[j] >> (i & (MASK_SIZE - 1))) {
                    size_type ctz = std::countr_zero(m_masks[j] >> (i & (MASK_SIZE - 1)));
                    return m_min += ctz, m_link[i + ctz].m_next - Cap;
                }
                for (size_type k = j + 1; k != (Cap + MASK_SIZE - 1) >> MASK_WIDTH; k++)
                    if (m_masks[k]) {
                        size_type d = (k - j) * MASK_SIZE - (i & (MASK_SIZE - 1)) + std::countr_zero(m_masks[k]);
                        return m_min += d, m_link[i + d].m_next - Cap;
                    }
                for (size_type k = 0; k <= j; k++)
                    if (m_masks[k]) {
                        size_type d = k * MASK_SIZE + Cap - i + std::countr_zero(m_masks[k]);
                        return m_min += d, m_link[i + d - Cap].m_next - Cap;
                    }
                return -1;
            }
            void pop() {
                size_type x = top(), i = m_link[Cap + x].m_prev, j = m_link[Cap + x].m_next;
                m_link[i].m_next = j, m_link[j].m_prev = i, m_link[Cap + x].m_next = -1;
                if (i == j) m_masks[i >> MASK_WIDTH] -= mask_type(1) << (i & (MASK_SIZE - 1)), m_size--;
            }
            bool contains(size_type x) const { return ~m_link[Cap + x].m_next; }
        };
        template <size_type Cap, typename Mapping>
        class HeapBase<Cap, Mapping, true> {
            struct link {
                size_type m_prev, m_next;
                void set(size_type x) { m_prev = m_next = x; }
            };
            mask_type m_mask;
            size_type m_size;
            mutable size_type m_min;
            std::vector<link> m_link;
            Mapping m_map;
        public:
            HeapBase(size_type length, Mapping map) : m_map(map) { resize(length); }
            void resize(size_type length) {
                m_mask = 0, m_size = m_min = 0;
                m_link.resize(Cap + length);
                for (size_type i = 0; i != Cap; i++) m_link[i].set(i);
                for (size_type i = 0; i != length; i++) m_link[Cap + i].set(-1);
            }
            void insert(size_type x) {
                size_type val = m_map(x), pos = val % Cap;
                if (val >= m_min + Cap) m_min = val - Cap + 1;
                if (m_link[pos].m_next == pos) {
                    m_mask |= mask_type(1) << pos, m_size++;
                    m_link[pos].set(Cap + x), m_link[Cap + x].set(pos);
                } else
                    m_link[m_link[Cap + x].m_prev = m_link[pos].m_prev].m_next = Cap + x, m_link[Cap + x].m_next = pos, m_link[pos].m_prev = Cap + x;
            }
            void sift_up(size_type x) {
                size_type prev = m_link[Cap + x].m_prev, next = m_link[Cap + x].m_next;
                if (prev == next) {
                    size_type pos = prev;
                    m_mask -= mask_type(1) << pos, m_size--, m_link[pos].set(pos);
                } else
                    m_link[next].m_prev = prev, m_link[prev].m_next = next;
                insert(x);
            }
            void clear() {
                m_mask = 0, m_size = m_min = 0;
                for (size_type i = 0; i != Cap; i++) m_link[i].m_prev = i;
                for (size_type i = 0, len = m_link.size() - Cap; i != len; i++) m_link[Cap + i].set(-1);
            }
            void push(size_type x) { (~m_link[Cap + x].m_next) ? sift_up(x) : insert(x); }
            bool empty() const { return !m_size; }
            size_type size() const { return m_size; }
            size_type top() const {
                size_type i = m_min % Cap;
                if (m_mask >> i & 1) return m_link[i].m_next - Cap;
                if (m_mask >> i) {
                    size_type ctz = std::countr_zero(m_mask >> i);
                    return m_min += ctz, m_link[i + ctz].m_next - Cap;
                }
                size_type d = Cap - i + std::countr_zero(m_mask);
                return m_min += d, m_link[i + d - Cap].m_next - Cap;
            }
            void pop() {
                size_type x = top(), i = m_link[Cap + x].m_prev, j = m_link[Cap + x].m_next;
                m_link[i].m_next = j, m_link[j].m_prev = i, m_link[Cap + x].m_next = -1;
                if (i == j) m_mask -= mask_type(1) << i, m_size--;
            }
            bool contains(size_type x) const { return ~m_link[Cap + x].m_next; }
        };
        template <size_type Cap, typename Mapping>
        using Heap = HeapBase<Cap, Mapping, Cap <= MASK_SIZE>;
        template <size_type Cap>
        struct HeapWrap {
            template <typename Getter, typename Compare>
            using type = Heap<Cap, Getter>;
        };
    }
    template <LBHeap::size_type Cap, typename Mapping>
    using LinkBucketHeap = LBHeap::Heap<Cap, Mapping>;
    template <typename Getter, typename Compare>
    using LinkBucketHeap01 = LBHeap::HeapWrap<2>::template type<Getter, Compare>;
}

#endif