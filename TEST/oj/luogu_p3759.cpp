#include "DS/LeveledBITManipulator.h"
#include "DS/LeveledSegManipulator.h"
#include "DS/LeveledZkwManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <map>
/*
[P3759 [TJOI2017] 不勤劳的图书管理员](https://www.luogu.com.cn/problem/P3759)
*/
/**
 * 删除元素时，需要把该元素造成的贡献拿掉，所以本质上是一个带修的二维数点
 */

struct Info {
    uint32_t pos, val;
} info[50000];
void solve_lbm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt, m_val;
        Pair &operator+=(const Pair &rhs) {
            m_cnt += rhs.m_cnt, m_val += rhs.m_val;
            return *this;
        }
        Pair &operator-=(const Pair &rhs) {
            m_cnt -= rhs.m_cnt, m_val -= rhs.m_val;
            return *this;
        }
        Pair operator+(const Pair &rhs) const {
            return {m_cnt + rhs.m_cnt, m_val + rhs.m_val};
        }
        Pair operator-(const Pair &rhs) const {
            return {m_cnt - rhs.m_cnt, m_val - rhs.m_val};
        }
        operator bool() const { return m_cnt; }
    };
    using T0 = OY::SEGCNT::Table<uint32_t, Pair, true, false, false>;
    T0::_reserve(2000000);
    OY::LBM::Table<T0> S(n);

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        auto &[pos, val] = info[i];
        cin >> pos >> val, pos--;
        auto pre = presum.query(pos + 1, n);
        ans += 1ull * pre.m_cnt * val + pre.m_val;
        presum.add(pos, {1, val});
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(pos, {1, val});
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x, y;
        cin >> x >> y, x--, y--;
        if (x > y) std::swap(x, y);
        auto remove = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_delete{};
            if (idx && pos + 1 < n) {
                auto plus_call = [&](const T0 &table) { to_delete += table.query(pos + 1, n - 1); };
                auto minus_call = [&](const T0 &table) { to_delete -= table.query(pos + 1, n - 1); };
                S.query_in_tables(0, idx - 1, plus_call, minus_call);
            }
            if (idx + 1 < n && pos) {
                auto plus_call = [&](const T0 &table) { to_delete += table.query(0, pos - 1); };
                auto minus_call = [&](const T0 &table) { to_delete -= table.query(0, pos - 1); };
                S.query_in_tables(idx + 1, n - 1, plus_call, minus_call);
            }
            S.modify_in_tables(idx, [&](T0 &table) {
                table.add(pos, Pair{uint32_t(-1), -val});
            });
            ans -= 1ull * to_delete.m_cnt * val + to_delete.m_val;
        };
        auto add = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_add{};
            if (idx && pos + 1 < n) {
                auto plus_call = [&](const T0 &table) { to_add += table.query(pos + 1, n - 1); };
                auto minus_call = [&](const T0 &table) { to_add -= table.query(pos + 1, n - 1); };
                S.query_in_tables(0, idx - 1, plus_call, minus_call);
            }
            if (idx + 1 < n && pos) {
                auto plus_call = [&](const T0 &table) { to_add += table.query(0, pos - 1); };
                auto minus_call = [&](const T0 &table) { to_add -= table.query(0, pos - 1); };
                S.query_in_tables(idx + 1, n - 1, plus_call, minus_call);
            }
            S.modify_in_tables(idx, [&](T0 &table) {
                table.add(pos, {1, val});
            });
            ans += 1ull * to_add.m_cnt * val + to_add.m_val;
        };
        remove(x), remove(y);
        std::swap(info[x], info[y]);
        add(x), add(y);
        cout << ans % 1000000007 << endl;
    }
}

void solve_lzm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt, m_val;
        Pair &operator+=(const Pair &rhs) {
            m_cnt += rhs.m_cnt, m_val += rhs.m_val;
            return *this;
        }
        Pair &operator-=(const Pair &rhs) {
            m_cnt -= rhs.m_cnt, m_val -= rhs.m_val;
            return *this;
        }
        Pair operator+(const Pair &rhs) const {
            return {m_cnt + rhs.m_cnt, m_val + rhs.m_val};
        }
        Pair operator-(const Pair &rhs) const {
            return {m_cnt - rhs.m_cnt, m_val - rhs.m_val};
        }
        operator bool() const { return m_cnt; }
    };
    using T0 = OY::SEGCNT::Table<uint32_t, Pair, true, false, false>;
    T0::_reserve(2000000);
    OY::LZM::Table<T0> S(n);

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        auto &[pos, val] = info[i];
        cin >> pos >> val, pos--;
        auto pre = presum.query(pos + 1, n);
        ans += 1ull * pre.m_cnt * val + pre.m_val;
        presum.add(pos, {1, val});
        S.modify_in_tables(i, [&](T0 &table) {
            table.add(pos, {1, val});
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x, y;
        cin >> x >> y, x--, y--;
        if (x > y) std::swap(x, y);
        auto remove = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_delete{};
            if (idx && pos + 1 < n) S.query_in_tables(0, idx - 1, [&](const T0 &table) {
                to_delete += table.query(pos + 1, n - 1);
            });
            if (idx + 1 < n && pos) S.query_in_tables(idx + 1, n - 1, [&](const T0 &table) {
                to_delete += table.query(0, pos - 1);
            });
            S.modify_in_tables(idx, [&](T0 &table) {
                table.add(pos, Pair{uint32_t(-1), -val});
            });
            ans -= 1ull * to_delete.m_cnt * val + to_delete.m_val;
        };
        auto add = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_add{};
            if (idx && pos + 1 < n) S.query_in_tables(0, idx - 1, [&](const T0 &table) {
                to_add += table.query(pos + 1, n - 1);
            });
            if (idx + 1 < n && pos) S.query_in_tables(idx + 1, n - 1, [&](const T0 &table) {
                to_add += table.query(0, pos - 1);
            });
            S.modify_in_tables(idx, [&](T0 &table) {
                table.add(pos, {1, val});
            });
            ans += 1ull * to_add.m_cnt * val + to_add.m_val;
        };
        remove(x), remove(y);
        std::swap(info[x], info[y]);
        add(x), add(y);
        cout << ans % 1000000007 << endl;
    }
}

void solve_lsm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt, m_val;
        Pair &operator+=(const Pair &rhs) {
            m_cnt += rhs.m_cnt, m_val += rhs.m_val;
            return *this;
        }
        Pair &operator-=(const Pair &rhs) {
            m_cnt -= rhs.m_cnt, m_val -= rhs.m_val;
            return *this;
        }
        Pair operator+(const Pair &rhs) const {
            return {m_cnt + rhs.m_cnt, m_val + rhs.m_val};
        }
        Pair operator-(const Pair &rhs) const {
            return {m_cnt - rhs.m_cnt, m_val - rhs.m_val};
        }
        operator bool() const { return m_cnt; }
    };
    using T0 = OY::SEGCNT::Table<uint32_t, Pair, true, false, false>;
    T0::_reserve(2000000);
    OY::LSM32 S(n);
    std::map<std::pair<uint32_t, uint32_t>, T0> table_mp;

    T0 presum;
    uint64_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        auto &[pos, val] = info[i];
        cin >> pos >> val, pos--;
        auto pre = presum.query(pos + 1, n);
        ans += 1ull * pre.m_cnt * val + pre.m_val;
        presum.add(pos, {1, val});
        S.modify_in_tables(i, [&](uint32_t L, uint32_t R) {
            table_mp[{L, R}].add(pos, {1, val});
        });
    }

    for (uint32_t _ = 0; _ != m; _++) {
        uint32_t x, y;
        cin >> x >> y, x--, y--;
        if (x > y) std::swap(x, y);
        auto remove = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_delete{};
            if (idx && pos + 1 < n) S.query_in_tables(0, idx - 1, [&](uint32_t L, uint32_t R) {
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    to_delete += it->second.query(pos + 1, n - 1);
            });
            if (idx + 1 < n && pos) S.query_in_tables(idx + 1, n - 1, [&](uint32_t L, uint32_t R) {
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    to_delete += it->second.query(0, pos - 1);
            });
            S.modify_in_tables(idx, [&](uint32_t L, uint32_t R) {
                table_mp[{L, R}].add(pos, Pair{uint32_t(-1), -val});
            });
            ans -= 1ull * to_delete.m_cnt * val + to_delete.m_val;
        };
        auto add = [&](uint32_t idx) {
            auto [pos, val] = info[idx];
            Pair to_add{};
            if (idx && pos + 1 < n) S.query_in_tables(0, idx - 1, [&](uint32_t L, uint32_t R) {
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    to_add += it->second.query(pos + 1, n - 1);
            });
            if (idx + 1 < n && pos) S.query_in_tables(idx + 1, n - 1, [&](uint32_t L, uint32_t R) {
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    to_add += it->second.query(0, pos - 1);
            });
            S.modify_in_tables(idx, [&](uint32_t L, uint32_t R) {
                table_mp[{L, R}].add(pos, {1, val});
            });
            ans += 1ull * to_add.m_cnt * val + to_add.m_val;
        };
        remove(x), remove(y);
        std::swap(info[x], info[y]);
        add(x), add(y);
        cout << ans % 1000000007 << endl;
    }
}

int main() {
    solve_lbm();
    // solve_lzm();
    // solve_lsm();
}