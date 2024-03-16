#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P4979 矿洞：坍塌](https://www.luogu.com.cn/problem/P4979)
*/
/**
 * 本题为 RangeManager 模板题
 * 由于只有三种颜色，所以只需要三个表
 * 当对某个区间进行赋值时，在一张表里进行添加区间，另外两张表移除区间
 */

void solve_rm() {
    uint32_t n, m;
    std::string s;
    cin >> n >> s >> m;
    OY::RangeManager<uint32_t> rm[3];
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

int main() {
    solve_rm();
}