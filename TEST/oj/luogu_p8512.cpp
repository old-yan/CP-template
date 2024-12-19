#include "DS/ChthollyZkwTree.h"
#include "DS/LinkBucket.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P8512 [Ynoi Easy Round 2021] TEST_152](https://www.luogu.com.cn/problem/P8512)
*/
/**
 * 离线处理，用珂朵莉树求每个操作的贡献
 *
 */

void solve_czkw() {
    uint32_t n, m, q;
    cin >> n >> m >> q;
    struct Op {
        uint32_t l, r, v;
    };
    std::vector<Op> ops(n);
    for (auto &[l, r, v] : ops) cin >> l >> r >> v, l--, r--;

    OY::LBC::LinkBucket<std::tuple<uint32_t, uint32_t>> qs(n, q);
    for (uint32_t qi = 0; qi != q; qi++) {
        uint32_t ql, qr;
        cin >> ql >> qr;
        qs[qr - 1].push_front({qi, ql - 1});
    }

    OY::WSumTree<uint64_t> S(n);

    // 珂朵莉树求出每个颜色的剩余贡献
    OY::ChthollyZkwTree<uint64_t> remain(m, -1);

    std::vector<uint64_t> ans(q);
    for (uint32_t qr = 0; qr != n; qr++) {
        auto &[l, r, v] = ops[qr];
        remain.enumerate(l, r, [&](auto l, auto r, auto c) {
            if (~c) S.add(c, -(uint64_t(ops[c].v) * (r - l + 1)));
        });
        remain.modify(l, r, qr);
        S.add(qr, uint64_t(v) * (r - l + 1));
        for (auto &[qi, ql] : qs[qr]) ans[qi] = S.query(ql, qr);
    }

    for (uint32_t qi = 0; qi != q; qi++) cout << ans[qi] << endl;
}

int main() {
    solve_czkw();
}