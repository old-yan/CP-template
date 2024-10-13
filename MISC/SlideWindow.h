/*
最后修改:
20241013
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_SLIDEWINDOW__
#define __OY_SLIDEWINDOW__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace WINDOW {
        using size_type = uint32_t;
        template <typename Callback, typename ShortenLeft, typename LengthenRight>
        void solve(size_type length, size_type window_len, Callback &&call, ShortenLeft &&left_call, LengthenRight &&right_call) {
            size_type l = 0, r = 0;
            while (r != window_len) right_call(r++);
            call(l, r - 1);
            while (r != length) {
                left_call(l++);
                right_call(r++);
                call(l, r - 1);
            }
        }
    }
}

#endif
