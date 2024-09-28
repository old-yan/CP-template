/*
最后修改:
20240906
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICBUFFERWRAPWITHOUTCOLLECT__
#define __OY_STATICBUFFERWRAPWITHOUTCOLLECT__

#include <algorithm>
#include <cstdint>

namespace OY {
    namespace StaticBufferWrapWithoutCollect_imp {
        using size_type = uint32_t;
        template <size_type BUFFER>
        struct StaticBufferWrapWithoutCollect {
            template <typename Node>
            struct type {
                static constexpr bool is_vector_buffer = false, is_collect = false;
                static Node s_buf[BUFFER + 1];
                static size_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_use_cnt++; }
                static size_type newnode(size_type length) {
                    s_use_cnt += length;
                    return s_use_cnt - length;
                }
                static void collect(size_type) {}
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrapWithoutCollect<BUFFER>::type<Node>::s_buf[BUFFER + 1];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrapWithoutCollect<BUFFER>::type<Node>::s_use_cnt = 1;
    }
    using StaticBufferWrapWithoutCollect_imp::StaticBufferWrapWithoutCollect;
}

#endif