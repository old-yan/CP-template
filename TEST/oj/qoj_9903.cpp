#include "DS/LinkBucket.h"
#include "GRAPH/DynamicFloyd.h"
#include "IO/FastIO.h"

/*
[C. 最短路径](https://qoj.ac/contest/1875/problem/9903)
*/
/**
 * 以每个点作为屏障，跑一次 floyd 需要 O(n^4) 的时间复杂度
 * 考虑到，每次 floyd 之间都有大量的重复内容，所以分治一下，通过保留副本的方法实现 floyd 的回滚
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    using T = OY::StaticDynamicFloyd<OY::DFLOYD::AddGroup<uint64_t>, false, 300>;
    T cur(n);
    for (uint32_t i = 0; i != n; i++)
        for (uint32_t j = 0; j != n; j++) {
            uint64_t w;
            cin >> w;
            cur.add_edge(i, j, w);
        }

    OY::LBC::LinkBucket<std::tuple<uint32_t, uint32_t, uint32_t>> qs(n, q);
    std::vector<uint64_t> ans(q);
    for (uint32_t qi = 0; qi != q; qi++) {
        uint32_t s, t, p;
        cin >> s >> t >> p;
        qs[p - 1].push_front({qi, s - 1, t - 1});
    }

    auto dfs = [&](auto &&self, uint32_t l, uint32_t r, auto &&cur) -> void {
        if (l + 1 == r)
            for (auto &[qi, s, t] : qs[l]) ans[qi] = cur.query(s, t);
        else {
            auto mid = l + r >> 1;
            auto next = cur;
            for (uint32_t k = l; k != mid; k++) next.update_by_vertex(k);
            self(self, mid, r, next);
            for (uint32_t k = mid; k != r; k++) cur.update_by_vertex(k);
            self(self, l, mid, cur);
        }
    };
    dfs(dfs, 0, n, cur);

    for (uint32_t qi = 0; qi != q; qi++) cout << ans[qi] << endl;
}