#include "DS/AdjDiff.h"
#include "DS/WaveLet.h"
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

int main() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    // first 为价值，second 为体积
    std::vector<std::pair<uint32_t, uint32_t>> a(n);
    for (uint32_t i = 0; i < n; i++) cin >> a[i].second;
    for (uint32_t i = 0; i < n; i++) cin >> a[i].first;
    std::sort(a.begin(), a.end());

    OY::WaveLet::Table<uint32_t, OY::AdjDiff::Table<uint64_t, true>> S(n, [&](uint32_t i) { return a[i].second; });
    uint32_t half = k / 2;
    int ans = -1;
    for (uint32_t i = half; i < n - half; i++) {
        uint64_t V = a[i].second;
        if (half) S.do_for_ksmallest(0, i - 1, half, [&](uint64_t val) {
            V += val;
        });
        if (half) S.do_for_ksmallest(i + 1, n - 1, half, [&](uint64_t val) {
            V += val;
        });
        if (V <= m) ans = a[i].first;
    }
    cout << ans;
}