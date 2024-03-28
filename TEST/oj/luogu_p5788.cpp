#include "IO/FastIO.h"

/*
[P5788 【模板】单调栈](https://www.luogu.com.cn/problem/P5788)
*/
/**
 * 本题为经典单调栈
 */

static constexpr uint32_t N = 3000000;
struct {
    uint32_t id, val;
} stack[N + 1];
uint32_t len, res[N + 1];
void solve_monostack() {
    uint32_t n;
    cin >> n;
    stack[len++] = {0, 1999999999};
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        while (stack[len - 1].val < x) res[stack[--len].id] = i;
        stack[len++] = {i, x};
    }
    for (uint32_t i = 1; i <= n; i++) cout << res[i] << ' ';
}

int main() {
    solve_monostack();
}