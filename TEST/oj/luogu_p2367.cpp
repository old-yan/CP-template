#include "DS/AdjDiff.h"
#include "IO/FastIO.h"

/*
[P2367 语文成绩](https://www.luogu.com.cn/problem/P2367)
*/
/**
 * 本题为差分/前缀和模板题
 */

using Tree = OY::AdjDiff::Table<int, false>;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });

    S.switch_to_difference();
    for (uint32_t i = 0; i < m; i++) {
        uint32_t x, y, z;
        cin >> x >> y >> z;
        S.add(x - 1, y - 1, z);
    }

    S.switch_to_value();
    int mi = 999999999;
    for (uint32_t i = 0; i < n; i++) mi = std::min(mi, S.query(i));
    cout << mi;
}