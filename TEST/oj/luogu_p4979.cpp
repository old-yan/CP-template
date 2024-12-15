#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P4979 矿洞：坍塌](https://www.luogu.com.cn/problem/P4979)
*/
/**
 * 本题为 RangeManager 模板题
 * 由于只有三种颜色，所以只需要三个表
 * 当对某个区间进行赋值时，在一张表里进行添加区间，另外两张表移除区间
 */

void solve_czkw() {
    uint32_t n, m;
    std::string s;
    cin >> n >> s >> m;
    OY::ChthollyZkwTree<char> S(n, 'A'); // 339ms
    // OY::VectorChthollySegTree<char, uint32_t> S(n, 'A'); // 687ms
    for (uint32_t i = 0; i != n; i++)
        if (s[i] != 'A') S.modify(i, s[i]);
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            uint32_t l, r;
            char x;
            cin >> l >> r >> x;
            S.modify(l - 1, r - 1, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            if (S.query(l - 1, r - 1).has_change())
                cout << "No\n";
            else if (l - 1 >= 1 && r + 1 <= n && S.query(l - 2) == S.query(r))
                cout << "No\n";
            else
                cout << "Yes\n";
        }
    }
}

void solve_rm() {
    uint32_t n, m;
    std::string s;
    cin >> n >> s >> m;
    OY::RangeManager<uint32_t> rm[3]; // 398ms
    for (uint32_t i = 0; i < n; i++) rm[s[i] - 'A'].add_range({i + 1, i + 1});
    auto get_color = [&](uint32_t pos) {
        if (rm[0].any_of({pos, pos}) != rm[0].end()) return 0;
        if (rm[1].any_of({pos, pos}) != rm[1].end()) return 1;
        return 2;
    };
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            uint32_t l, r;
            char x;
            cin >> l >> r >> x;
            for (uint32_t i : {0, 1, 2}) {
                if (i == x - 'A')
                    rm[i].add_range({l, r});
                else
                    rm[i].remove_range({l, r});
            }
        } else {
            uint32_t l, r;
            cin >> l >> r;
            uint32_t color = get_color(l);
            if (rm[color].all_of({l, r}) == rm[color].end())
                cout << "No\n";
            else if (l == 1 || r == n || get_color(l - 1) != get_color(r + 1))
                cout << "Yes\n";
            else
                cout << "No\n";
        }
    }
}

void solve_bitset() {
    uint32_t n, m;
    std::string s;
    cin >> n >> s >> m;
    OY::ZkwTreeBitset<> S[3]; // 611ms
    // OY::VectorLazyBitset<uint32_t> S[3]; // 776ms

    for (auto &_ : S) _.resize(n);
    for (uint32_t i = 0; i < n; i++) S[s[i] - 'A'].set(i);
    auto get_color = [&](uint32_t pos) {
        if (S[0][pos]) return 0;
        if (S[1][pos]) return 1;
        return 2;
    };
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            uint32_t l, r;
            char x;
            cin >> l >> r >> x;
            for (uint32_t i : {0, 1, 2}) {
                if (i == x - 'A')
                    S[i].set(l - 1, r - 1);
                else
                    S[i].reset(l - 1, r - 1);
            }
        } else {
            uint32_t l, r;
            cin >> l >> r;
            uint32_t color = get_color(l - 1);
            if (!S[color].all(l - 1, r - 1))
                cout << "No\n";
            else if (l == 1 || r == n || get_color(l - 2) != get_color(r))
                cout << "Yes\n";
            else
                cout << "No\n";
        }
    }
}

int main() {
    solve_czkw();
    // solve_rm();
    // solve_bitset();
}