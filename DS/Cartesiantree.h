/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CARTESIANTREE__
#define __OY_CARTESIANTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace Cartesian {
        using size_type = uint32_t;
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Tp>
            struct type {
                static Tp s_buf[BUFFER + 1];
                static void reserve(size_type) {}
                static constexpr Tp *data() { return s_buf; }
            };
        };
        template <size_type BUFFER>
        template <typename Tp>
        Tp StaticBufferWrap<BUFFER>::template type<Tp>::s_buf[BUFFER + 1];
        template <typename Tp>
        struct DynamicBuffer {
            static Tp *s_buf;
            static void reserve(size_type length) {
                if (s_buf) delete[] s_buf;
                s_buf = new Tp[length + 1];
            }
            static constexpr Tp *data() { return s_buf; }
        };
        template <typename Tp>
        Tp *DynamicBuffer<Tp>::s_buf;
        template <typename Tp, template <typename> typename BufferType = DynamicBuffer, typename InitMapping, typename LchildCallback, typename RchildCallback, typename Compare = std::less<Tp>>
        size_type solve(size_type length, InitMapping mapping, LchildCallback &&lchild_call, RchildCallback &&rchild_call, Compare &&comp = Compare(), const Tp &max = std::numeric_limits<Tp>::max()) {
            struct node {
                size_type m_index, m_rc;
                Tp m_value;
            };
            using buffer_type = BufferType<node>;
            buffer_type::reserve(length);
            buffer_type::data()[0].m_index = -1;
            buffer_type::data()[0].m_value = max;
            size_type len = 1;
            for (size_type i = 0; i != length; i++) {
                Tp x = mapping(i);
                size_type last = -1;
                while (comp(buffer_type::data()[len - 1].m_value, x)) {
                    last = buffer_type::data()[--len].m_index;
                    rchild_call(last, buffer_type::data()[len].m_rc);
                }
                buffer_type::data()[len - 1].m_rc = i;
                lchild_call(i, last);
                buffer_type::data()[len].m_index = i;
                buffer_type::data()[len].m_value = x;
                buffer_type::data()[len].m_rc = -1;
                len++;
            }
            while (len > 1) {
                size_type last = buffer_type::data()[--len].m_index;
                rchild_call(last, buffer_type::data()[len].m_rc);
            }
            return buffer_type::data()[0].m_rc;
        }
        template <template <typename> typename BufferType = DynamicBuffer, typename Iterator, typename LchildCallback, typename RchildCallback, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>>
        size_type solve(Iterator first, Iterator last, LchildCallback &&lchild_call, RchildCallback &&rchild_call, Compare &&comp = Compare(), const Tp &max = std::numeric_limits<Tp>::max()) {
            return solve<Tp, BufferType>(last - first, [&](size_type i) { return *(first + i); }, lchild_call, rchild_call, comp, max);
        }
    }
}

#endif