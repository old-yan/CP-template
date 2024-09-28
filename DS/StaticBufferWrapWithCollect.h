/*
最后修改:
20240906
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICBUFFERWRAPWITHCOLLECT__
#define __OY_STATICBUFFERWRAPWITHCOLLECT__

#include <algorithm>
#include <cstdint>

namespace OY {
    namespace StaticBufferWrapWithCollect_imp {
        using size_type = uint32_t;
        template <size_type BUFFER>
        struct StaticBufferWrapWithCollect {
            template <typename Node>
            struct type {
                static constexpr bool is_vector_buffer = false, is_collect = true;
                static Node s_buf[BUFFER + 1];
                static size_type s_gc[BUFFER], s_use_cnt, s_gc_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++; }
                static size_type newnode(size_type length) {
                    s_use_cnt += length;
                    return s_use_cnt - length;
                }
                static void collect(size_type x) { s_buf[x] = {}, s_gc[s_gc_cnt++] = x; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrapWithCollect<BUFFER>::type<Node>::s_buf[BUFFER + 1];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrapWithCollect<BUFFER>::type<Node>::s_gc[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrapWithCollect<BUFFER>::type<Node>::s_use_cnt = 1;
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrapWithCollect<BUFFER>::type<Node>::s_gc_cnt = 0;
    }
    using StaticBufferWrapWithCollect_imp::StaticBufferWrapWithCollect;
}

#endif