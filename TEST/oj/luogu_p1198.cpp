#include "DS/MonoStack.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "MATH/Lemire32.h"

/*
[P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
*/
/**
 * 显然，本题可以用线段树解决
 * 考虑到区间查询的右边界始终在右移，所以也可以用单调栈+二分解决
 */

static constexpr uint32_t N = 200000;
void solve_monostack() {
    uint32_t m, D;
    cin >> m >> D;

    uint32_t lst{};
    auto getter = [&, L = OY::Lemire32(D)](uint32_t) {
        int x;
        cin >> x;
        return (x + lst) % L;
    };
    OY::MonoMaxStack<uint32_t, decltype(getter)> S(m, {}, getter, {}, D, true);

    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            S.extend_right();
        } else {
            uint32_t len;
            cin >> len;
            lst = S.query_on_left(S.cursor() - len + 1, S.cursor())->m_value;
            cout << lst << endl;
        }
    }
}

void solve_ds() {
    uint32_t m, D, lst{};
    cin >> m >> D;
    OY::Lemire32 L(D);

    auto S = OY::make_ZkwTree<uint32_t, 1 << 19>(m, [](auto x, auto y) { return x > y ? x : y; });
    // auto S = OY::make_SegTree<uint32_t, true, OY::Seg::Ignore, 1 << 19, uint32_t>(m, [](auto x, auto y) { return x > y ? x : y; });
    uint32_t n = 0;
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            int x;
            cin >> x;
            S.modify(n++, (x + lst) % L);
        } else {
            uint32_t l;
            cin >> l;
            lst = S.query(n - l, n - 1);
            cout << lst << endl;
        }
    }
}

int main() {
    solve_monostack();
    // solve_ds();
}