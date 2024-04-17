#include "DS/MonoStack.h"
#include "IO/FastIO.h"

/*
[P5788 【模板】单调栈](https://www.luogu.com.cn/problem/P5788)
*/
/**
 * 本题为经典单调栈
 */

static constexpr uint32_t N = 3000000;
uint32_t res[N];
void solve_monostack() {
    uint32_t n;
    cin >> n;

    auto getter = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto pop_call = [&](uint32_t popped, uint32_t left_smaller) {
        res[popped] = left_smaller + 1;
    };
    // 本来的 pop_call 是在每个元素 a 被后来的大于等于 a 的值淘汰掉时调用的回调
    // 如果传递比较运算符为 less_equal，就可以在每个元素 a 被后来的大于 a 的值淘汰掉时调用回调
    OY::MONOSTACK::Table<uint32_t, std::less_equal<uint32_t>, decltype(getter), decltype(pop_call)> S(n, {}, getter, pop_call, 1999999999, true);
    for (uint32_t i = 0; i != n; i++) S.extend_right();
    for (uint32_t i = 0; i != n; i++) cout << res[i] << ' ';
}

int main() {
    solve_monostack();
}