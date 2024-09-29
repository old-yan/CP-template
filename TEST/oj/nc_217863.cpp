#include "DS/LeveledBITManipulator.h"
#include "DS/LeveledSegManipulator.h"
#include "DS/LeveledZkwManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <map>
/*
[速度即转发](https://ac.nowcoder.com/acm/problem/217863)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/11161/E
 * 显然的，树套树二分
 * 注意，如果用树状数组来套，那么需要对权值做一下水平翻转
 */

uint32_t arr[100000];
void solve_lbm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt;
        uint64_t m_val;
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
    struct T0 : OY::SEGCNT::Table<uint32_t, Pair, true, false, false> {
        uint32_t m_bound;
    };
    T0::_reserve(4000000);
    auto init = [](T0 &table, auto l, auto r) {
        table.m_bound = r;
    };
    OY::LBM::Table<T0> S(100001, init);
    for (uint32_t i = 0; i != n; i++) {
        cin >> arr[i];
        arr[i] = 100000 - arr[i];
        S.modify_in_tables(arr[i], [&](T0 &table) {
            table.add(i, {1, arr[i]});
        });
    }
    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            uint64_t k;
            cin >> l >> r >> k, l--, r--;
            Pair smaller{};
            auto pos = S.max_right([&](const T0 &table) {
                Pair res = smaller;
                res += table.query(l, r);
                if (1ull * res.m_cnt * table.m_bound - res.m_val >= k) return false;
                smaller = res;
                return true;
            });
            cout << int(99999 - pos) << endl;
        } else {
            uint32_t p, k;
            cin >> p >> k, p--, k = 100000 - k;
            S.modify_in_tables(arr[p], [&](T0 &table) {
                table.add(p, {-1u, -uint64_t(arr[p])});
            });
            S.modify_in_tables(arr[p] = k, [&](T0 &table) {
                table.add(p, {1, arr[p]});
            });
        }
    }
}

void solve_lzm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt;
        uint64_t m_val;
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
    struct T0 : OY::SEGCNT::Table<uint32_t, Pair, true, false, false> {
        uint32_t m_bound;
    };
    T0::_reserve(4000000);
    auto init = [](T0 &table, auto l, auto r) {
        table.m_bound = l;
    };
    OY::LZM::Table<T0> S(100001, init);
    for (uint32_t i = 0; i != n; i++) {
        cin >> arr[i];
        S.modify_in_tables(arr[i], [&](T0 &table) {
            table.add(i, {1, arr[i]});
        });
    }
    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            uint64_t k;
            cin >> l >> r >> k, l--, r--;
            Pair bigger{};
            uint32_t bound = 0;
            auto pos = S.min_left(100000, [&](const T0 &table) {
                Pair res = bigger;
                res += table.query(l, r);
                if (res.m_val - res.m_cnt * table.m_bound >= k) return false;
                bigger = res, bound = table.m_bound;
                return true;
            }) - 1;
            cout << int(pos) << endl;
        } else {
            uint32_t p, k;
            cin >> p >> k, p--;
            S.modify_in_tables(arr[p], [&](T0 &table) {
                table.add(p, {-1u, -uint64_t(arr[p])});
            });
            S.modify_in_tables(arr[p] = k, [&](T0 &table) {
                table.add(p, {1, arr[p]});
            });
        }
    }
}

void solve_lsm() {
    uint32_t n, m;
    cin >> n >> m;

    struct Pair {
        uint32_t m_cnt;
        uint64_t m_val;
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
    T0::_reserve(4000000);
    OY::LSM32 S(100001);
    std::map<std::pair<uint32_t, uint32_t>, T0> table_mp;

    for (uint32_t i = 0; i != n; i++) {
        cin >> arr[i];
        S.modify_in_tables(arr[i], [&](uint32_t L, uint32_t R) {
            table_mp[{L, R}].add(i, {1, arr[i]});
        });
    }
    for (uint32_t _ = 0; _ != m; _++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            uint64_t k;
            cin >> l >> r >> k, l--, r--;
            Pair bigger{};
            uint32_t bound = 0;
            auto pos = S.min_left(100000, [&](uint32_t L, uint32_t R) {
                Pair res = bigger;
                auto it = table_mp.find({L, R});
                if (it != table_mp.end())
                    res += it->second.query(l, r);
                if (res.m_val - res.m_cnt * L >= k) return false;
                bigger = res, bound = L;
                return true;
            }) - 1;
            cout << int(pos) << endl;
        } else {
            uint32_t p, k;
            cin >> p >> k, p--;
            S.modify_in_tables(arr[p], [&](uint32_t L, uint32_t R) {
                table_mp[{L, R}].add(p, {-1u, -uint64_t(arr[p])});
            });
            S.modify_in_tables(arr[p] = k, [&](uint32_t L, uint32_t R) {
                table_mp[{L, R}].add(p, {1, arr[p]});
            });
        }
    }
}

int main() {
    solve_lbm();
    // solve_lzm();
    // solve_lsm();
}