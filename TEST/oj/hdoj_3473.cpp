#include "DS/AdjDiff.h"
#include "DS/MergeSortTree.h"
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
struct SumTable : OY::AdjSumTable<uint64_t, false> {
    using Base = OY::AdjSumTable<uint64_t, false>;
    template <typename Iterator>
    void reset(Iterator first, Iterator last) {
        Base::reset(first, last);
        Base::switch_to_presum();
    }
    auto query(uint32_t pos) const { return Base::query(pos, pos); }
    auto query(uint32_t left, uint32_t right) const { return Base::query(left, right); }
};
int main() {
    uint32_t t;
    cin >> t;

    for (uint32_t i = 1; i <= t; i++) {
        uint32_t n;
        cin >> n;
        std::vector<uint32_t> a(n);
        for (auto &e : a) cin >> e;
        OY::MS::Tree<uint32_t, std::less<uint32_t>, SumTable> S(a.begin(), a.end());
        // OY::WaveLet::Table<uint32_t, SumTable> S(a.begin(), a.end());

        cout << "Case #" << i << ":\n";
        uint32_t q;
        cin >> q;
        while (q--) {
            uint32_t l, r;
            cin >> l >> r;
            uint64_t res = 0;
            uint32_t k = (r - l + 1) / 2;
            if (k) {
                S.do_for_rank_range(l, r, r - l + 1 - k, r - l, [&](uint64_t val) {
                    res += val;
                });
                S.do_for_rank_range(l, r, 0, k - 1, [&](uint64_t val) {
                    res -= val;
                });
            }
            cout << res << endl;
        }
        cout << endl;
    }
}