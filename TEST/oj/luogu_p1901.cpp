#include "DS/MonoStack.h"
#include "IO/FastIO.h"

/*
[P1901 发射站](https://www.luogu.com.cn/problem/P1901)
*/
/**
 * 要找“最近的比自己大的元素”
 * 可以使用单调栈解决
 */

static constexpr uint32_t N = 1000000;
uint32_t H[N], V[N];
uint64_t receive[N], Max;
void solve_monostack() {
    uint32_t n;
    cin >> n;
    auto getter = [](uint32_t index) {
        cin >> H[index] >> V[index];
        return H[index];
    };
    auto pop_call = [](uint32_t popped, uint32_t by_who) {
        receive[by_who] += V[popped];
        Max = std::max(Max, receive[by_who]);
    };
    auto S1 = OY::MonoMaxStack<uint32_t, decltype(getter), decltype(pop_call)>(n, {}, getter, pop_call, 2000000001, true);
    for (uint32_t i = 0; i != n; i++) S1.extend_right();
    auto S2 = OY::make_MonoMaxStack(n, H, pop_call, 2000000001u, false);
    for (uint32_t i = 0; i != n; i++) S2.extend_left();

    cout << Max;
}

int main() {
    solve_monostack();
}