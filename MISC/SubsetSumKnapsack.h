/*
最后修改:
20240824
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SUBSETSUMKNAPSACK__
#define __OY_SUBSETSUMKNAPSACK__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace SSK {
        using size_type = uint32_t;
        template <typename Table, typename MaterialContainer>
        inline void solve_01(Table &x, MaterialContainer &&materials, size_type target = -1) {
            std::vector<size_type> freq(x.size());
            for (auto a : materials)
                if (a && a < x.size()) freq[a]++;
            size_type s = 1, tot = 1;
            for (; s <= tot && tot < x.size(); tot += s * freq[s], s++) {}
            tot = std::min(tot, x.size());
            x.set(0, tot - 1);
            if (~target && tot > target) return;
            if (tot == x.size()) return;
            for (; s != x.size(); s++)
                if (freq[s]) {
                    if ((s << 1) < x.size()) freq[s << 1] += (freq[s] - 1) >> 1;
                    freq[s] = ((freq[s] - 1) & 1) + 1;
                    if (freq[s]) x.bitor_lshift(s, tot), tot += s;
                    if (~target && x[target]) return;
                    if (freq[s] > 1) x.bitor_lshift(s, tot), tot += s;
                    if (~target && x[target]) return;
                    tot = std::min(tot, x.size());
                }
        }
        template <typename Table, typename MaterialContainer>
        inline void solve_unbounded(Table &x, MaterialContainer &&materials, size_type target = -1) {
            if (x.size() == 1 || !target) return void(x.set(0));
            std::vector<bool> vis(x.size());
            for (auto a : materials)
                if (a && a < x.size()) vis[a] = true;
            if (vis[1]) return void(x.set());
            x.set(0);
            for (size_type s = 2; s != x.size(); s++)
                if (vis[s]) {
                    x.bitor_lshift_unbounded(s);
                    if (~target && x[target]) return;
                    for (size_type i = s; i < x.size(); i += s) vis[i] = false;
                }
        }
    }
}

#endif