#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"

/*
[P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
*/
/**
 * 显然，本题可以用线段树解决
 * 考虑到区间查询的右边界始终在右移，所以也可以用单调栈+二分解决
*/

static constexpr uint32_t N = 200000;
struct {
    uint32_t id, val;
} stack[N + 1];
void solve_monostack() {
    uint32_t m, D, lst{};
    cin >> m >> D;
    using mint = OY::DynamicModInt32<0>;
    mint::set_mod(D);
    uint32_t n = 0, sn = 0;
    stack[sn++] = {n++, D};
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            int64_t x;
            cin >> x;
            uint32_t val = mint(x + lst).val();
            while (stack[sn - 1].val <= val) sn--;
            stack[sn++] = {n++, val};
        } else {
            uint32_t l;
            cin >> l;
            uint32_t low = 0, high = sn - 1;
            while (low < high) {
                auto mid = (low + high) / 2;
                if (stack[mid].id >= n - l)
                    high = mid;
                else
                    low = mid + 1;
            }
            lst = stack[low].val;
            cout << lst << endl;
        }
    }
}

void solve_ds() {
    uint32_t m, D, lst{};
    cin >> m >> D;
    using mint = OY::DynamicModInt32<0>;
    mint::set_mod(D);
    auto S = OY::make_ZkwTree<uint32_t, 1 << 19>(m, [](auto x, auto y) { return x > y ? x : y; });
    // auto S = OY::make_SegTree<uint32_t, true, OY::Seg::Ignore, 1 << 19, uint32_t>(m, [](auto x, auto y) { return x > y ? x : y; });
    uint32_t n = 0;
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            int64_t x;
            cin >> x;
            uint32_t val = mint(x + lst).val();
            S.modify(n++, val);
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