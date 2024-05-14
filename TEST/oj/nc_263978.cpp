#include "DS/AdjDiff.h"
#include "DS/MergeSortTree.h"
// #include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[fsl 的背包](https://ac.nowcoder.com/acm/problem/263978)
*/
/**
 * 对每个物品判断其作为价值中位数的可能性
 * 在比其价值低的所有物品里，选一半物品，且体积尽量小
 * 在比其价值高的所有物品里，选一半物品，且体积尽量小
 * 只要能放得下，那么该物品就具备作为中位数可能性
 */

struct SumTable : OY::AdjDiff::Table<uint64_t, false> {
    using Base = OY::AdjDiff::Table<uint64_t, false>;
    template <typename Iterator>
    void reset(Iterator first, Iterator last) {
        Base::reset(first, last);
        Base::switch_to_presum();
    }
    auto query(uint32_t pos) const { return Base::query(pos, pos); }
    auto query(uint32_t left, uint32_t right) const { return Base::query(left, right); }
};
int main() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    // first 为价值，second 为体积
    std::vector<std::pair<uint32_t, uint32_t>> a(n);
    for (uint32_t i = 0; i < n; i++) cin >> a[i].second;
    for (uint32_t i = 0; i < n; i++) cin >> a[i].first;
    std::sort(a.begin(), a.end());

    OY::WaveLet::Table<uint32_t, SumTable> S(n, [&](uint32_t i) { return a[i].second; });
    // OY::MS::Tree<uint32_t, std::less<uint32_t>, SumTable> S(n, [&](uint32_t i) { return a[i].second; });
    uint32_t half = k / 2;
    int ans = -1;
    for (uint32_t i = half; i < n - half; i++) {
        uint64_t V = a[i].second;
        if (half) S.do_for_rank_range(0, i - 1, 0, half - 1, [&](uint64_t val) {
            V += val;
        });
        if (half) S.do_for_rank_range(i + 1, n - 1, 0, half - 1, [&](uint64_t val) {
            V += val;
        });
        if (V <= m) ans = a[i].first;
    }
    cout << ans;
}