#include "DS/LeveledBITManipulator.h"
#include "DS/LeveledSegManipulator.h"
#include "DS/LeveledZkwManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <map>
/*
[P3157 [CQOI2011] 动态逆序对](https://www.luogu.com.cn/problem/P3157)
*/
/**
 * 删除元素时，需要把该元素造成的贡献拿掉，所以本质上是一个带修的二维数点
 */

uint32_t pos[100000];
void solve_lbm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint32_t, true, false, false>;
    T0::_reserve(4000000);
    OY::LBM::Table<T0> S(n);

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        pos[--x] = i;
        ans += presum.query(x + 1, n);
        presum.add(x, 1);
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(x, 1);
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x;
        cin >> x;
        uint32_t i = pos[--x];
        uint32_t to_delete = 0;
        if (i) {
            auto plus_call = [&](const T0 &table) { to_delete += table.query(x + 1, n); };
            auto minus_call = [&](const T0 &table) { to_delete -= table.query(x + 1, n); };
            S.query_in_tables(0, i - 1, plus_call, minus_call);
        }
        if (i + 1 < n && x) {
            auto plus_call = [&](const T0 &table) { to_delete += table.presum(x - 1); };
            auto minus_call = [&](const T0 &table) { to_delete -= table.presum(x - 1); };
            S.query_in_tables(i + 1, n - 1, plus_call, minus_call);
        }
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(x, -1);
        });
        cout << ans << endl;
        ans -= to_delete;
    }
}

void solve_lzm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint32_t, true, false, false>;
    T0::_reserve(6000000);
    OY::LZM::Table<T0> S(n);

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        pos[--x] = i;
        ans += presum.query(x + 1, n);
        presum.add(x, 1);
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(x, 1);
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x;
        cin >> x;
        uint32_t i = pos[--x];
        uint32_t to_delete = 0;
        if (i) S.query_in_tables(0, i - 1, [&](const T0 &table) {
            to_delete += table.query(x + 1, n);
        });
        if (i + 1 < n && x) S.query_in_tables(i + 1, n - 1, [&](const T0 &table) {
            to_delete += table.presum(x - 1);
        });
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(x, -1);
        });
        cout << ans << endl;
        ans -= to_delete;
    }
}

void solve_lsm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint32_t, true, false, false>;
    T0::_reserve(6000000);
    OY::LSM32 S(n);
    std::map<std::pair<uint32_t, uint32_t>, T0> table_mp;

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        pos[--x] = i;
        ans += presum.query(x + 1, n);
        presum.add(x, 1);
        S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
            table_mp[{L, R}].add(x, 1);
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x;
        cin >> x;
        uint32_t i = pos[--x];
        uint32_t to_delete = 0;
        if (i) S.query_in_tables(0, i - 1, [&](uint32_t L, uint32_t R) {
            auto it = table_mp.find({L, R});
            if (it != table_mp.end())
                to_delete += it->second.query(x + 1, n);
        });
        if (i + 1 < n && x) S.query_in_tables(i + 1, n - 1, [&](uint32_t L, uint32_t R) {
            auto it = table_mp.find({L, R});
            if (it != table_mp.end())
                to_delete += it->second.presum(x - 1);
        });
        S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
            table_mp[{L, R}].add(x, -1);
        });
        cout << ans << endl;
        ans -= to_delete;
    }
}

int main() {
    solve_lbm();
    // solve_lzm();
    // solve_lsm();
}