#include "DS/LeveledBITManipulator.h"
#include "DS/LeveledZkwManipulator.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[P3332 [ZJOI2013] K大数查询](https://www.luogu.com.cn/problem/P3332)
*/
/**
 * 树套树模板
 * 注意值域可能为负
 */

void solve_lbm() {
    uint32_t n, m;
    cin >> n >> m;
    using T0 = OY::VectorSegLazySumTree<uint64_t, uint32_t>;
    T0::_reserve(12000000);
    auto init = [&](T0 &table, uint32_t l, uint32_t r) {
        table.resize(n);
    };
    OY::LBM::Table<T0> S(n * 2 + 1, init);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int c;
            cin >> c;
            S.modify_in_tables(n - c, [&](T0 &table) {
                table.add(l - 1, r - 1, 1);
            });
        } else {
            uint64_t c, sum = 0;
            cin >> c;
            auto pos = S.max_right([&](const T0 &table) {
                auto cnt = table.query(l - 1, r - 1);
                if (sum + cnt < c) return sum += cnt, true;
                return false;
            }) + 1;
            cout << int(n - pos) << endl;
        }
    }
}

void solve_lzm() {
    uint32_t n, m;
    cin >> n >> m;
    using T0 = OY::VectorSegLazySumTree<uint64_t, uint32_t>;
    T0::_reserve(20000000);
    auto init = [&](T0 &table, uint32_t l, uint32_t r) {
        table.resize(n);
    };
    OY::LZM::Table<T0> S(n * 2 + 1, init);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int c;
            cin >> c;
            S.modify_in_tables(c + n, [&](T0 &table) {
                table.add(l - 1, r - 1, 1);
            });
        } else {
            uint64_t c, sum = 0;
            cin >> c;
            auto pos = S.min_left(n * 2, [&](const T0 &table) {
                auto cnt = table.query(l - 1, r - 1);
                if (sum + cnt < c) return sum += cnt, true;
                return false;
            }) - 1;
            cout << int(pos - n) << endl;
        }
    }
}

int main() {
    solve_lbm();
    // solve_lzm();
}