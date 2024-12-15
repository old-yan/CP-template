#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P3740 [HAOI2014] 贴海报](https://www.luogu.com.cn/problem/P3740)
*/
/**
 * 本题为 RangeManager 模板题
 * 倒序插入每个区间，如果某个区间插入时，该区间已经全部存在，说明这个区间最终被完全覆盖
 */

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<std::pair<uint32_t, uint32_t>> colors(m);
    for (uint32_t i = 0; i < m; i++) {
        auto &[l, r] = colors[i];
        cin >> l >> r;
    }
    OY::RangeManager<uint32_t> rm; // 41ms
    uint32_t ans = 0;
    for (uint32_t i = m - 1; ~i; i--)
        if (rm.all_of(colors[i]) == rm.end()) {
            ans++;
            rm.add_range(colors[i]);
        }
    cout << ans;
}

void solve_cseg() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<std::pair<uint32_t, uint32_t>> colors(m);
    uint32_t mx = 0;
    for (uint32_t i = 0; i < m; i++) {
        auto &[l, r] = colors[i];
        cin >> l >> r;
        mx = std::max(mx, r);
    }

    OY::VectorChthollySegTree<bool, uint32_t> S(mx + 1); // 47ms
    // OY::ChthollyZkwTree<bool> S(mx + 1); // 347ms
    uint32_t ans = 0;
    for (uint32_t i = m - 1; ~i; i--) {
        auto [l, r] = colors[i];
        auto val = S.query(l, r);
        if (!(val.get() && !val.has_change())) {
            ans++;
            S.modify(l, r, true);
        }
    }
    cout << ans;
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<std::pair<uint32_t, uint32_t>> colors(m);
    uint32_t mx = 0;
    for (uint32_t i = 0; i < m; i++) {
        auto &[l, r] = colors[i];
        cin >> l >> r;
        mx = std::max(mx, r);
    }

    OY::VectorLazyBitset<uint32_t> S(mx + 1); // 45ms
    uint32_t ans = 0;
    for (uint32_t i = m - 1; ~i; i--) {
        auto [l, r] = colors[i];
        if (S.count(l, r) != r - l + 1) {
            ans++;
            S.set(l, r);
        }
    }
    cout << ans;
}

int main() {
    solve_rm();
    // solve_cseg();
    // solve_bitset();
}