#include "DS/MonoStack.h"
#include "IO/FastIO.h"

/*
[P2866 [USACO06NOV] Bad Hair Day S](https://www.luogu.com.cn/problem/P2866)
*/
/**
 * 所谓能看到的牛数，其实就是要找“最近的比自己大的元素”
 * 可以使用单调栈解决
 */

void solve_monostack() {
    uint32_t n;
    cin >> n;
    // 一开始假设所有牛都能看到前面的所有牛
    uint64_t ans = uint64_t(n) * (n - 1) / 2;
    // 当找到更高的牛时，把看不见的牛删掉
    auto pop_call = [&](uint32_t popped, uint32_t by_who) {
        ans -= n - by_who;
    };
    auto getter = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::MonoMaxStack<uint32_t, decltype(getter), decltype(pop_call)> S(n, {}, getter, pop_call, 1000000001, true);
    for (uint32_t i = 0; i < n; i++) S.extend_right();
    cout << ans;
}

int main() {
    solve_monostack();
}