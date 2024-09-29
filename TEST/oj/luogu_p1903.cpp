#include "DS/LeveledBITManipulator.h"
#include "DS/LeveledSegManipulator.h"
#include "DS/LeveledZkwManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <map>
#include <set>

/*
[P1903 [国家集训队] 数颜色 / 维护队列](https://www.luogu.com.cn/problem/P1903)
*/
/**
 * 按照静态数颜色的思路维护贡献
 * 静态数颜色，需要一个可持久化权值线段树
 * 那么带修就需要树套树，以对过去的某个旧版本做修改
 */

struct Info {
    uint32_t color, pre, nxt;
} info[133334];
void solve_lbm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint64_t, true, false, false>;
    T0::_reserve(5000000);
    OY::LBM::Table<T0> S(n + 1);

    std::vector<std::set<uint32_t>> poss(1000001);
    for (uint32_t i = 1; i <= n; i++) {
        cin >> info[i].color;
        auto &st = poss[info[i].color];
        if (st.size()) {
            int pre = *st.rbegin();
            info[pre].nxt = i;
            info[i].pre = pre;
            S.modify_in_tables(i, [&](T0 &table) {
                table.add(pre, 1);
            });
        } else
            S.modify_in_tables(i, [&](T0 &table) {
                table.add(0, 1);
            });
        st.insert(st.end(), i);
        info[i].nxt = n + 1;
    }

    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t l, r;
            cin >> l >> r;
            uint32_t ans = 0;
            auto plus_call = [&](const T0 &table) {
                ans += table.query(0, l - 1);
            };
            auto minus_call = [&](const T0 &table) {
                ans -= table.query(0, l - 1);
            };
            S.query_in_tables(l, r, plus_call, minus_call);
            cout << ans << endl;
        } else {
            uint32_t i, x;
            cin >> i >> x;
            {
                // 对旧颜色的前驱后继关系做处理，撇清贡献
                auto &st = poss[info[i].color];
                int pre = info[i].pre, nxt = info[i].nxt;
                S.modify_in_tables(i, [&](T0 &table) {
                    table.add(pre, -1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](T0 &table) {
                    table.add(pre, 1);
                    table.add(i, -1);
                });
                if (pre) info[pre].nxt = nxt;
                if (nxt <= n) info[nxt].pre = pre;
                st.erase(i);
            }
            {
                // 对新颜色的前驱后继关系做处理，计算贡献
                auto &st = poss[info[i].color = x];
                int pre = 0, nxt = n + 1;
                auto it = st.insert(i).first;
                if (it != st.begin()) pre = *std::prev(it);
                if (++it != st.end()) nxt = *it;
                S.modify_in_tables(i, [&](T0 &table) {
                    table.add(pre, 1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](T0 &table) {
                    table.add(pre, -1);
                    table.add(i, 1);
                });
                if (pre) info[pre].nxt = i;
                info[i].pre = pre, info[i].nxt = nxt;
                if (nxt <= n) info[nxt].pre = i;
            }
        }
    }
}

void solve_lzm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint64_t, true, false, false>;
    T0::_reserve(5000000);
    OY::LZM::Table<T0> S(n + 1);

    std::vector<std::set<uint32_t>> poss(1000001);
    for (uint32_t i = 1; i <= n; i++) {
        cin >> info[i].color;
        auto &st = poss[info[i].color];
        if (st.size()) {
            int pre = *st.rbegin();
            info[pre].nxt = i;
            info[i].pre = pre;
            S.modify_in_tables(i, [&](T0 &table) {
                table.add(pre, 1);
            });
        } else
            S.modify_in_tables(i, [&](T0 &table) {
                table.add(0, 1);
            });
        st.insert(st.end(), i);
        info[i].nxt = n + 1;
    }

    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t l, r;
            cin >> l >> r;
            uint32_t ans = 0;
            S.query_in_tables(l, r, [&](const T0 &table) {
                ans += table.query(0, l - 1);
            });
            cout << ans << endl;
        } else {
            uint32_t i, x;
            cin >> i >> x;
            {
                // 对旧颜色的前驱后继关系做处理，撇清贡献
                auto &st = poss[info[i].color];
                int pre = info[i].pre, nxt = info[i].nxt;
                S.modify_in_tables(i, [&](T0 &table) {
                    table.add(pre, -1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](T0 &table) {
                    table.add(pre, 1);
                    table.add(i, -1);
                });
                if (pre) info[pre].nxt = nxt;
                if (nxt <= n) info[nxt].pre = pre;
                st.erase(i);
            }
            {
                // 对新颜色的前驱后继关系做处理，计算贡献
                auto &st = poss[info[i].color = x];
                int pre = 0, nxt = n + 1;
                auto it = st.insert(i).first;
                if (it != st.begin()) pre = *std::prev(it);
                if (++it != st.end()) nxt = *it;
                S.modify_in_tables(i, [&](T0 &table) {
                    table.add(pre, 1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](T0 &table) {
                    table.add(pre, -1);
                    table.add(i, 1);
                });
                if (pre) info[pre].nxt = i;
                info[i].pre = pre, info[i].nxt = nxt;
                if (nxt <= n) info[nxt].pre = i;
            }
        }
    }
}

void solve_lsm() {
    uint32_t n, m;
    cin >> n >> m;

    using T0 = OY::SEGCNT::Table<uint32_t, uint64_t, true, false, false>;
    T0::_reserve(5000000);
    OY::LSM32 S(n + 1);

    std::vector<std::set<uint32_t>> poss(1000001);
    std::map<std::pair<uint32_t, uint32_t>, T0> table_mp;
    for (uint32_t i = 1; i <= n; i++) {
        cin >> info[i].color;
        auto &st = poss[info[i].color];
        if (st.size()) {
            int pre = *st.rbegin();
            info[pre].nxt = i;
            info[i].pre = pre;
            S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
                auto &table = table_mp[{L, R}];
                table.add(pre, 1);
            });
        } else
            S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
                auto &table = table_mp[{L, R}];
                table.add(0, 1);
            });
        st.insert(st.end(), i);
        info[i].nxt = n + 1;
    }

    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t l, r;
            cin >> l >> r;
            uint32_t ans = 0;
            S.query_in_tables(l, r, [&](uint32_t L, uint32_t R) {
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    ans += it->second.query(0, l - 1);
            });
            cout << ans << endl;
        } else {
            uint32_t i, x;
            cin >> i >> x;
            {
                // 对旧颜色的前驱后继关系做处理，撇清贡献
                auto &st = poss[info[i].color];
                int pre = info[i].pre, nxt = info[i].nxt;
                S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
                    auto &table = table_mp[{L, R}];
                    table.add(pre, -1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](uint32_t L, uint32_t R) {
                    auto &table = table_mp[{L, R}];
                    table.add(pre, 1);
                    table.add(i, -1);
                });
                if (pre) info[pre].nxt = nxt;
                if (nxt <= n) info[nxt].pre = pre;
                st.erase(i);
            }
            {
                // 对新颜色的前驱后继关系做处理，计算贡献
                auto &st = poss[info[i].color = x];
                int pre = 0, nxt = n + 1;
                auto it = st.insert(i).first;
                if (it != st.begin()) pre = *std::prev(it);
                if (++it != st.end()) nxt = *it;
                S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
                    auto &table = table_mp[{L, R}];
                    table.add(pre, 1);
                });
                if (nxt <= n) S.modify_in_tables(nxt, [&](uint32_t L, uint32_t R) {
                    auto &table = table_mp[{L, R}];
                    table.add(pre, -1);
                    table.add(i, 1);
                });
                if (pre) info[pre].nxt = i;
                info[i].pre = pre, info[i].nxt = nxt;
                if (nxt <= n) info[nxt].pre = i;
            }
        }
    }
}

int main() {
    solve_lbm();
    // solve_lzm();
    // solve_lsm();
}