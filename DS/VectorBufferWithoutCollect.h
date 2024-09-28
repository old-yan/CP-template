/*
最后修改:
20240906
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_VECTORBUFFERWITHOUTCOLLECT__
#define __OY_VECTORBUFFERWITHOUTCOLLECT__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace VectorBufferWithoutCollect_imp {
        using size_type = uint32_t;
        template <typename Node>
        struct VectorBufferWithoutCollect {
            static constexpr bool is_vector_buffer = true, is_collect = false;
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static size_type newnode(size_type length) {
                s_buf.resize(s_buf.size() + length);
                return s_buf.size() - length;
            }
            static void collect(size_type) {}
        };
        template <typename Node>
        std::vector<Node> VectorBufferWithoutCollect<Node>::s_buf{Node{}};
    }
    using VectorBufferWithoutCollect_imp::VectorBufferWithoutCollect;
}

#endif