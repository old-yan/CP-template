#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/MergeSortTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[T526317 帕鲁大陆染色的一天](https://www.luogu.com.cn/problem/T526317)
*/
/**
 * 如果打不开，请访问 https://www.luogu.com.cn/contest/212630
 */

void solve_czkw() {
    uint32_t n, m;
    cin >> n >> m;

    // 珂朵莉树求出每个颜色完全被覆盖掉的时间
    OY::ChthollyZkwTree<uint32_t> wall(n, 0);
    // OY::VectorChthollySegTree<uint32_t, uint32_t> wall(n, 0);
    std::vector<uint32_t> len(m + 1);
    std::vector<uint32_t> die(m + 1, m + 1);
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t l, r;
        cin >> l >> r, l--, r--;
        len[i] = r - l + 1;
        wall.enumerate(l, r, [&](auto l, auto r, auto c) {
            if (c) {
                len[c] -= r - l + 1;
                if (!len[c]) die[c] = i;
            }
        });
        wall.modify(l, r, i);
    }

    // 查询 [opl, opr] 就是查询这段操作 减去 这段操作在 opr 之前被覆盖的数量
    OY::WaveLet::Table<uint32_t> S(die.begin(), die.end(), std::bit_width(m + 1));
    // OY::MS::Tree<uint32_t> S(die.begin(), die.end());
    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << r - l + 1 - S.rank(l, r, r + 1) << endl;
    }
}

int main() {
    solve_czkw();
}