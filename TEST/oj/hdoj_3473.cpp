#include "DS/AdjDiff.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[Minimum Sum](https://acm.hdu.edu.cn/showproblem.php?pid=3473)
*/
/**
 * 显然，将 x 设置到中位数即可保证从 x 到每个点的距离之和最小
 * 当把 x 设置到中位数之后，比 x 大的数都是正贡献，比 x 小的数都是负贡献。
 * 需要一个能查询区间前 k 大的数的和的数据结构
 */

int main() {
    uint32_t t;
    cin >> t;

    for (uint32_t i = 1; i <= t; i++) {
        uint32_t n;
        cin >> n;
        std::vector<uint32_t> a(n);
        for (auto &e : a) cin >> e;
        using SumTable = OY::AdjDiff::Table<uint64_t, true>;
        OY::WaveLet::Table<uint32_t, SumTable> S(a.begin(), a.end());

        cout << "Case #" << i << ":\n";
        uint32_t q;
        cin >> q;
        while (q--) {
            uint32_t l, r;
            cin >> l >> r;
            uint64_t res = 0;
            S.do_for_klargest(l, r, (r - l + 1) / 2, [&](uint64_t val) {
                res += val;
            });
            S.do_for_ksmallest(l, r, (r - l + 1) / 2, [&](uint64_t val) {
                res -= val;
            });
            cout << res << endl;
        }
        cout << endl;
    }
}