/*
最后修改:
20240906
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_VECTORBUFFERWITHCOLLECT__
#define __OY_VECTORBUFFERWITHCOLLECT__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace VectorBufferWithCollect_imp {
        using size_type = uint32_t;
        template <typename Node>
        struct VectorBufferWithCollect {
            static constexpr bool is_vector_buffer = true, is_collect = true;
            static std::vector<Node> s_buf;
            static std::vector<size_type> s_gc;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                if (!s_gc.empty()) {
                    size_type res = s_gc.back();
                    s_gc.pop_back();
                    return res;
                }
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static size_type newnode(size_type length) {
                s_buf.resize(s_buf.size() + length);
                return s_buf.size() - length;
            }
            static void collect(size_type x) { s_buf[x] = {}, s_gc.push_back(x); }
        };
        template <typename Node>
        std::vector<Node> VectorBufferWithCollect<Node>::s_buf{Node{}};
        template <typename Node>
        std::vector<size_type> VectorBufferWithCollect<Node>::s_gc;
    }
    using VectorBufferWithCollect_imp::VectorBufferWithCollect;
}

#endif