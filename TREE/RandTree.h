/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RANDTREE__
#define __OY_RANDTREE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

namespace OY {
    namespace RandTree {
        using size_type = uint32_t;
        template <typename Callback, typename Generator = std::mt19937>
        void solve(size_type vertex_cnt, Callback &&call, Generator &&gen = Generator()) {
            if (vertex_cnt <= 1) return;
            if (vertex_cnt == 2) {
                call(0, 1);
                return;
            }
            std::vector<uint32_t> prufer_code(vertex_cnt - 2), deg(vertex_cnt);
            std::uniform_int_distribution<uint32_t> dis(0, vertex_cnt - 1);
            for (auto &a : prufer_code) deg[a = dis(gen)]++;
            auto it = prufer_code.begin(), end = prufer_code.end();
            for (size_type i = 0; i < vertex_cnt; i++)
                if (!deg[i]) {
                    size_type cur = i;
                    do
                        if (it == end)
                            call(cur, vertex_cnt - 1), cur = vertex_cnt - 1;
                        else {
                            call(cur, *it), cur = *it;
                            ++it;
                        }
                    while (!--deg[cur] && cur <= i);
                }
        }
        template <typename Tree, typename Generator = std::mt19937>
        Tree make_tree(size_type vertex_cnt, Generator &&gen = Generator()) {
            Tree res(vertex_cnt);
            solve(
                vertex_cnt, [&](size_type a, size_type b) { res.add_edge(a, b); }, gen);
            res.prepare();
            return res;
        }
    }
}

#endif